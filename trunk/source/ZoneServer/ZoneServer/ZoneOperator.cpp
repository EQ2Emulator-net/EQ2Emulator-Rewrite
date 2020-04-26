#include "stdafx.h"

#include "ZoneOperator.h"
#include "Client.h"
#include "ZoneServer.h"

#include "../Packets/OP_LoginByNumRequestMsg_Packet.h"
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ClientLoginConfirm_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_ZoneTransferReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZoneTransfer_Packet.h"
#include "../Packets/OP_ChangeZoneMsg_Packet.h"
#include "../WorldTalk/WorldStream.h"
#include "../../common/Packets/EmuPackets/Emu_ClientSessionEnded_Packet.h"

ZoneOperator::ZoneOperator() {
}

ZoneOperator::~ZoneOperator() {
}

bool ZoneOperator::Process() {
	return ProcessClientWrite() && ProcessClients();
}

void ZoneOperator::AddStream(std::shared_ptr<Stream> stream, uint64_t key) {
	UDPServer::AddStream(stream, key);

	clients.insert(std::static_pointer_cast<Client>(stream));
}

bool ZoneOperator::ProcessClientWrite() {
	ReadLocker lock(streamLock);

	for (auto& itr : Streams) {
		std::static_pointer_cast<Client>(itr.second)->Write();
	}

	return true;
}

bool ZoneOperator::ProcessClients() {
	std::set<std::weak_ptr<Client>, std::owner_less<std::weak_ptr<Client> > > itr_copy = clients;

	for (auto& itr : itr_copy) {
		if (std::shared_ptr<Client> c = itr.lock()) {
			c->Process();		
		}
		else {
			clients.erase(itr);
		}
	}

	return true;
}

std::shared_ptr<Stream> ZoneOperator::GetNewStream(unsigned int ip, unsigned short port) {
	return std::make_shared<Client>(ip, port);
}

void ZoneOperator::ClientLogIn(std::shared_ptr<Client> client, OP_LoginByNumRequestMsg_Packet* packet) {

	client->SetAccountID(packet->account_id);
	std::map<uint32_t, PendingClient>::iterator itr = pending_clients.find(packet->account_id);
	if (itr != pending_clients.end()) {
		PendingClient pc = itr->second;
		pending_clients.erase(itr);

		if (pc.access_code != packet->access_code) {
			client->SendLoginReply(1);
			return;
		}

		auto ws = GetWorldStream();
		if (!ws) {
			return;
		}

		std::shared_ptr<ZoneServer> z = GetZone(pc.zone_id, pc.instance_id);
		if (!z) {
			client->SendLoginReply(3);
			return;
		}

		auto p = new Emu_ClientLoginConfirm_Packet;
		p->access_code = pc.access_code;
		p->sessionID = client->GetSessionID();
		p->zoneName = z->GetName();
		ws->QueuePacket(p);
		
		// Login error, 0 = accepted, 1 = invalid password, 2 = currently playing, 6 = bad version, every thing else = unknown reason
		client->SendLoginReply(0);
		client->SetCharacterID(pc.character_id);
		clients.erase(client);
		z->AddClient(client);
	}
	else {
		client->SendLoginReply(3);
	}
}

void ZoneOperator::AddPendingClient(uint32_t account_id, PendingClient pending_client) {
	pending_clients[account_id] = pending_client;
}

void ZoneOperator::RemovePendingClient(uint32_t access_code) {
	for (auto itr = pending_clients.begin(); itr != pending_clients.end(); itr++) {
		if (itr->second.access_code == access_code) {
			pending_clients.erase(itr);
			break;
		}
	}
}

std::shared_ptr<ZoneServer> ZoneOperator::AddNewZone(uint32_t zone_id, uint32_t instance_id) {
	std::shared_ptr<ZoneServer> ret;

	ret = GetZone(zone_id, instance_id);
	if (ret) {
		LogWarn(LOG_ZONE, 0, "Attempted to start a zone (%u) that was already running", zone_id);
		return ret;
	}

	// TODO: Make an instance
	std::shared_ptr<ZoneServer> z = make_shared<ZoneServer>(zone_id);
	if (z->Init()) {
		std::pair<uint32_t, uint32_t> key = std::make_pair(zone_id, instance_id);
		zones[key] = z;
		ret = z;
	}
	else {
		LogError(LOG_ZONE, 0, "Failed to start zone %u", zone_id);
	}

	return ret;
}

std::shared_ptr<ZoneServer> ZoneOperator::GetZone(uint32_t zone_id, uint32_t instance_id) {
	std::pair<uint32_t, uint32_t> key = std::make_pair(zone_id, instance_id);
	std::map<std::pair<uint32_t, uint32_t>, std::shared_ptr<ZoneServer> >::iterator itr = zones.find(key);
	if (itr != zones.end())
		return itr->second;

	return nullptr;
}

void ZoneOperator::SetWorldServerName(std::string name) {
	worldServerName = name;
}

std::string ZoneOperator::GetWorldServerName() {
	return worldServerName;
}

void ZoneOperator::SetWorldStream(const std::shared_ptr<WorldStream>& stream) {
	worldStream = stream;
}

std::shared_ptr<WorldStream> ZoneOperator::GetWorldStream() {
	return worldStream.lock();
}

void ZoneOperator::HandleZoneTransferReply(Emu_ZoneTransferReply_Packet* p) {
	WriteLocker lock(pendingZoneTransfers_lock);
	auto itr = pendingZoneTransfers.find(p->sessionID);
	if (itr != pendingZoneTransfers.end()) {
		auto client = itr->second.lock();
		pendingZoneTransfers.erase(itr);

		if (!client) {
			return;
		}

		if (p->bError) {
			client->chat.SendSimpleGameMessage("\\#FF0000Error requesting a zone change. Try again later!");
			return;
		}

		//Forward this client over to the new zone
		OP_ChangeZoneMsg_Packet out(client->GetVersion());
		out.ip_address = p->host;
		out.port = p->port;
		out.account_id = client->GetAccountID();
		out.key = p->access_code;
		client->QueuePacket(out);
	}
}

bool ZoneOperator::RequestZoneTransfer(const std::shared_ptr<Client>& client, uint32_t zoneID, uint32_t instanceID) {
	auto ws = GetWorldStream();
	if (!ws) {
		return false;
	}

	WriteLocker lock(pendingZoneTransfers_lock);

	uint32_t session = client->GetSessionID();
	if (pendingZoneTransfers.count(session) != 0) {
		return false;
	}

	pendingZoneTransfers[session] = client;

	lock.Unlock();

	//Send a request to the world server to transfer this client
	auto p = new Emu_RequestZoneTransfer_Packet;
	p->accountID = client->GetAccountID();
	p->characterID = client->GetCharacterID();
	p->zoneID = zoneID;
	p->instanceID = instanceID;
	p->sessionID = session;
	ws->QueuePacket(p);

	return true;
}

void ZoneOperator::StreamDisconnected(std::shared_ptr<Stream> stream) {
	auto ws = GetWorldStream();
	if (!ws) {
		return;
	}

	auto p = new Emu_ClientSessionEnded_Packet;
	p->sessionID = std::static_pointer_cast<Client>(stream)->GetSessionID();
	ws->QueuePacket(p);
}