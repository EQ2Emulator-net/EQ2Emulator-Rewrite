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

bool ZoneTalk::GetAvailableZone(std::shared_ptr<Client> client, uint32_t char_id) {
	bool ret = false;

	uint32_t zone_id = database.GetZoneIDForCharacter(char_id);
	client->SetPendingZone(char_id, zone_id, 0);
	for (std::pair<SOCKET, std::shared_ptr<Stream> > kvp : Streams) {
		std::shared_ptr<ZoneStream> zs = std::static_pointer_cast<ZoneStream>(kvp.second);
		if (zs) {
			if (zs->HasZone(zone_id)) {
				ret = true;
				TransferClientToZone(zs, client, zone_id, 0);
			}
		}
	}

	// No zone found lets send a request to start it up
	// and add the client to a list to wait for the zone
	if (!ret) {
		// actually do some checks to find an appropriate zone to send the request to
		// for now grab the first zone connected.
		if (Streams.size() > 0) {
			pendingClientZones[zone_id].push_back(client);

			std::shared_ptr<ZoneStream> zs = std::static_pointer_cast<ZoneStream>(Streams.begin()->second);
			Emu_RequestZone_Packet* request = new Emu_RequestZone_Packet();
			request->zone_id = zone_id;
			zs->QueuePacket(request);
		}
		else {
			// no zone servers connected
			OP_PlayCharacterReplyMsg_Packet* p = new OP_PlayCharacterReplyMsg_Packet(client->GetVersion());
			p->response = PlayCharacterResponse::EZoneDown;
			client->QueuePacket(p);
		}

	}

	return ret;
}

void ZoneTalk::AddZone(std::shared_ptr<ZoneStream> zs, uint32_t zone_id, uint32_t instance_id) {
	LogError(LOG_ZONE, 0, "adding zone %u", zone_id);
	zs->AddZone(zone_id, instance_id);

	
	std::map<uint32_t, std::vector<std::weak_ptr<Client> > >::iterator itr = pendingClientZones.find(zone_id);
	if (itr != pendingClientZones.end()) {
		LogError(LOG_ZONE, 0, "zone found zone %u", zone_id);
		for (std::weak_ptr<Client> client : itr->second) {
			LogError(LOG_ZONE, 0, "sending clients to zone %u", zone_id);
			std::shared_ptr<Client> c = client.lock();
			if (c)
				TransferClientToZone(zs, c, zone_id, 0);
		}
		pendingClientZones.erase(zone_id);
	}
}

void ZoneTalk::TransferClientToZone(std::shared_ptr<ZoneStream> zs, std::shared_ptr<Client> client, uint32_t zone_id, uint32_t instance_id) {
	uint32_t access_code = MakeRandomNumber();

	// Send info to zone
	Emu_TransferClient_Packet* t = new Emu_TransferClient_Packet;
	t->account_id = client->GetAccountID();
	t->access_code = access_code;
	t->character_id = client->GetPendingCharacter();
	t->zone_id = client->GetPendingZone();
	t->instance_id = client->GetPendingInstance();
	client->pending_access_code = access_code;
	zs->QueuePacket(t);
}