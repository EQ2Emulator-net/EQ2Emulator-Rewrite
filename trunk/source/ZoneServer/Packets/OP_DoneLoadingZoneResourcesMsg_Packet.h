#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"

#include "OP_MoveableObjectPlacementCriteri_Packet.h"
#include "OP_DoneSendingInitialEntitiesMsg_Packet.h"
#include "OP_GameWorldTimeMsg_Packet.h"

#include "../../common/timer.h"

class OP_DoneLoadingZoneResourcesMsg_Packet : public EQ2Packet {
public:
	OP_DoneLoadingZoneResourcesMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		unknown = 0;
	}

	void HandlePacket(std::shared_ptr<Client> client) {
		// Send OP_MoveableObjectPlacementCriteria
		OP_MoveableObjectPlacementCriteri_Packet* pmove = new OP_MoveableObjectPlacementCriteri_Packet(client->GetVersion());
		pmove->unknown = 9999;
		pmove->unknown1 = 9999;
		pmove->unknown2 = 9999;
		client->QueuePacket(pmove);

		// Send Skill slot mapping
		// Send Game world time
		OP_GameWorldTimeMsg_Packet* ptime = new OP_GameWorldTimeMsg_Packet(client->GetVersion());
		ptime->year = 3956;
		ptime->month = 5;
		ptime->day = 5;
		ptime->hour = 9;
		ptime->minute = 38;
		ptime->unknown = 27;
		ptime->unix_time = Timer::GetUnixTimeStamp();
		ptime->unknown2 = 1.0f;
		client->QueuePacket(ptime);

		// Send initial spawns
		// Finaly send the finished packet
		OP_DoneSendingInitialEntitiesMsg_Packet* p = new OP_DoneSendingInitialEntitiesMsg_Packet(client->GetVersion());
		client->QueuePacket(p);
	}

	uint32_t unknown;


private:

	void RegisterElements() {
		if (GetVersion() > 283) {
			RegisterUInt32(unknown);
		}
	}

};