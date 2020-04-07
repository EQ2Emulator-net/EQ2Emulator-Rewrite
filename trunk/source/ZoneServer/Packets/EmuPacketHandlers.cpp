#include "stdafx.h"
#include "../WorldTalk/WorldStream.h"
#include "../WorldTalk/WorldTalk.h"

#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServerReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZone_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RequestZoneReply_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_TransferClient_Packet.h"

#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator z;

void Emu_RegisterZoneServerReply_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	if (reply == 1) {
		w->SetAuthentication(Authentication::EAuthGranted);
		LogDebug(LOG_NET, 0, "Zoneserver is now authenticated.");
	}
	else {
		w->SetAuthentication(Authentication::EAuthDenied);
		LogDebug(LOG_NET, 0, "World server rejected this zoneserver.");
	}
}

void Emu_RequestZone_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	Emu_RequestZoneReply_Packet* p = new Emu_RequestZoneReply_Packet();

	if (zone_id == 0) {
		LogError(LOG_ZONE, 0, "Zone request packet received with 0 for the zone id.");	
		p->reply = 0;
		w->QueuePacket(p);
		return;
	}

	std::shared_ptr<ZoneServer> zone = z.AddNewZone(zone_id, instance_id);
	if (zone) {
		p->reply = zone->GetID();
		w->QueuePacket(p);
	}
	else {
		p->reply = 0;
		w->QueuePacket(p);
	}
}

void Emu_TransferClient_Packet::HandlePacket(std::shared_ptr<WorldStream> w) {
	PendingClient pc;
	pc.access_code = access_code;
	pc.character_id = character_id;
	pc.zone_id = zone_id;
	pc.instance_id = instance_id;

	z.AddPendingClient(account_id, pc);
}