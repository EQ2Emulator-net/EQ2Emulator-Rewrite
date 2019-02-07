#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/WorldServer.h"
#include "../WorldServer/Client.h"
#include "OP_WorldListMsg_Packet.h"

class OP_AllWSDescRequestMsg_Packet : public EQ2Packet {
public:
	OP_AllWSDescRequestMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		
	}

	void HandlePacket(Client* client) {
		// Need this for when we load the server info
		WorldServer* s = client->GetServer();

		OP_WorldListMsg_Packet* world_list = new OP_WorldListMsg_Packet(client->GetVersion());
		world_list->Name = s->GetName();
		world_list->Name2 = s->GetName();

		uint32_t AllowedRaces = s->GetAllowedRaces();
		if (GetVersion() < 1096 && AllowedRaces > 0x0007FFFF)
			AllowedRaces = 0x0007FFFF; // without Freeblood and Aerakyn race
		else if (GetVersion() < 60006 && AllowedRaces > 0x000FFFFF)
			AllowedRaces = 0x000FFFFF; // with Freeblood
		else if (AllowedRaces > 0x001FFFFF)
			AllowedRaces = 0x001FFFFF;	// with Aerakyn

		// set server info
		LogWarn(LOG_PACKET, 0, "Sending world reply");
		client->QueuePacket(world_list);
	}

private:

};
