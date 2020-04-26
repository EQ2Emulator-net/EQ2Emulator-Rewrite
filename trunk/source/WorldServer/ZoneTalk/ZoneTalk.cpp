#include "stdafx.h"

#include "ZoneTalk.h"
#include "ZoneStream.h"
#include "../Packets/OP_PlayCharacterReplyMsg_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZone_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_TransferClient_Packet.h"

#include "../Database/WorldDatabase.h"

extern WorldDatabase database;

ZoneTalk::ZoneTalk() : TCPServer(true) {
	
}

std::shared_ptr<Stream> ZoneTalk::GetNewStream(unsigned int ip, unsigned short port) {
	LogDebug(LOG_NET, 0, "New zoneserver connected from %u", ip);
	return std::make_shared<ZoneStream>(ip, port);
}

bool ZoneTalk::Process() {
	ReadLocker lock(streamLock);
	for (auto& itr : Streams) {
		itr.second->Process();
	}

	return true;
}

void ZoneTalk::StreamDisconnected(std::shared_ptr<Stream> stream) {
	
}

std::shared_ptr<ZoneStream> ZoneTalk::GetAvailableZone(uint32_t zone_id) {
	std::shared_ptr<ZoneStream> ret;

	for (std::pair<SOCKET, std::shared_ptr<Stream> > kvp : Streams) {
		std::shared_ptr<ZoneStream> zs = std::static_pointer_cast<ZoneStream>(kvp.second);
		if (zs) {
			if (zs->HasZone(zone_id)) {
				ret = zs;
			}
		}
	}

	if (!ret && pendingZones.count(zone_id) == 0) {
		// No zone found lets send a request to start it up
		// and add the client to a list to wait for the zone

		pendingZones.insert(zone_id);
		if (!Streams.empty()) {
			std::shared_ptr<ZoneStream> zs = std::static_pointer_cast<ZoneStream>(Streams.begin()->second);
			Emu_RequestZone_Packet* request = new Emu_RequestZone_Packet();
			request->zone_id = zone_id;
			zs->QueuePacket(request);
		}
	}

	return ret;
}

void ZoneTalk::AddZone(std::shared_ptr<ZoneStream> zs, uint32_t zone_id, uint32_t instance_id) {
	LogError(LOG_ZONE, 0, "adding zone %u", zone_id);
	zs->AddZone(zone_id, instance_id);
	
	{
		std::map<uint32_t, std::vector<std::weak_ptr<Client> > >::iterator itr = pendingZoneClients.find(zone_id);
		if (itr != pendingZoneClients.end()) {
			LogError(LOG_ZONE, 0, "zone found zone %u", zone_id);
			for (std::weak_ptr<Client> client : itr->second) {
				LogError(LOG_ZONE, 0, "sending clients to zone %u", zone_id);
				std::shared_ptr<Client> c = client.lock();
				if (c) {
					uint32_t access_code = TransferClientToZone(zs, c->GetPendingCharacter(), zone_id, c->GetAccountID(),
						c->GetPendingInstance(), false);
					c->pending_access_code = access_code;
				}
			}
			pendingZoneClients.erase(zone_id);
		}
	}

	auto itr = pendingZoneTransfers.find(zone_id);
	if (itr != pendingZoneTransfers.end()) {
		for (auto& itr : itr->second) {
			//TODO: Handle instances
			PendingSessionTransfer transfer;
			transfer.sessionID = itr.first;
			transfer.zs = zs;
			uint32_t access_code = TransferClientToZone(zs, itr.second.characterID, zone_id, itr.second.accountID, 0, true);
			pendingSessionTransfers[access_code] = transfer;
		}
		pendingZoneTransfers.erase(itr);
	}

	pendingZones.erase(zone_id);
}

uint32_t ZoneTalk::TransferClientToZone(std::shared_ptr<ZoneStream> zs, uint32_t character_id, uint32_t zone_id, uint32_t account_id,
	uint32_t instance_id, bool bFromZone) 
{
	uint32_t access_code = MakeRandomNumber();

	// Send info to zone
	Emu_TransferClient_Packet* t = new Emu_TransferClient_Packet;
	t->account_id = account_id;
	t->access_code = access_code;
	t->character_id = character_id;
	t->zone_id = zone_id;
	t->instance_id = instance_id;
	t->bFromZone = bFromZone;
	zs->QueuePacket(t);

	return access_code;
}

bool ZoneTalk::HasPendingZone(uint32_t zone_id) {
	return pendingZones.count(zone_id) != 0;
}

ZoneTalk::PendingSessionTransfer ZoneTalk::PopPendingSessionTransfer(uint32_t access_code) {
	PendingSessionTransfer ret;
	ret.sessionID = 0;

	auto itr = pendingSessionTransfers.find(access_code);
	if (itr != pendingSessionTransfers.end()) {
		ret = itr->second;
		pendingSessionTransfers.erase(itr);
	}
	return ret;
}

void ZoneTalk::AddPendingSessionTransfer(uint32_t access_code, uint32_t sessionID, const std::shared_ptr<ZoneStream>& fromZs) {
	PendingSessionTransfer transfer;
	transfer.sessionID = sessionID;
	transfer.zs = fromZs;
	pendingSessionTransfers[access_code] = transfer;
}

void ZoneTalk::AddPendingZoneTransfer(uint32_t characterID, uint32_t zoneID, uint32_t sessionID, uint32_t accountID) {
	PendingZoneTransfer pzt;
	pzt.accountID = accountID;
	pzt.characterID = characterID;

	pendingZoneTransfers[zoneID][sessionID] = pzt;
}

void ZoneTalk::AddPendingZoneClient(uint32_t zoneID, const std::shared_ptr<Client>& client) {
	pendingZoneClients[zoneID].emplace_back(client);
}