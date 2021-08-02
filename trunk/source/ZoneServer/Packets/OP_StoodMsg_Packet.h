#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"
#include "../Spawns/Spawn.h"
#include "../Controllers/PlayerController.h"
#include "OP_CampAbortedMsg_Packet.h"

class OP_StoodMsg_Packet : public EQ2Packet {
public:
	OP_StoodMsg_Packet(uint32_t version) : EQ2Packet(version) {
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		std::shared_ptr<Spawn> controlled = client->GetController()->GetControlled();

		if (!controlled) {
			return;
		}

		if (controlled->GetInfoStruct()->entityFlags & EntityFlagCamping) {
			//Abort camping
			controlled->DisableEntityFlags(EntityFlagCamping, false);
			controlled->SetActivityTimer(0);

			OP_CampAbortedMsg_Packet p(client->GetVersion());
			client->QueuePacket(p);
		}
	}
};
