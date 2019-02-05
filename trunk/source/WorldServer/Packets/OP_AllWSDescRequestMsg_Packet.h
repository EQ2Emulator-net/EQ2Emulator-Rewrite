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
		// set server info
		client->QueuePacket(world_list);
	}

private:

};

RegisterWorldStruct("OP_AllWSDescRequestMsg", OP_AllWSDescRequestMsg_Packet, 1);