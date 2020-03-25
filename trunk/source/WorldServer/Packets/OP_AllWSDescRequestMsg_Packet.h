#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/WorldServer.h"
#include "../WorldServer/Client.h"
#include "OP_WorldListMsg_Packet.h"

class OP_AllWSDescRequestMsg_Packet : public EQ2Packet {
public:
	OP_AllWSDescRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		
	}

	void HandlePacket(std::shared_ptr<Client> client) {
		// Need this for when we load the server info
		WorldServer* s = client->GetServer();

		OP_WorldListMsg_Packet* world_list = new OP_WorldListMsg_Packet(client->GetVersion());
		world_list->Name = s->GetName();
		world_list->Name2 = s->GetName();

		//uint32_t AllowedRaces = s->GetAllowedRaces();
		LogWarn(LOG_PACKET, 0, "Sending world reply");
		client->QueuePacket(world_list);
	}

private:

};
