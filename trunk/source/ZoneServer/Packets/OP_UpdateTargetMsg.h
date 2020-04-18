#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../Spawns/Spawn.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/PlayerController.h"

class OP_UpdateTargetMsg_Packet : public EQ2Packet {
public:
	OP_UpdateTargetMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}
	~OP_UpdateTargetMsg_Packet() = default;

	void RegisterElements() {
		RegisterUInt16(spawn_index);
		RegisterUInt8(unknown);
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		std::shared_ptr<Spawn> spawn = client->GetSpawnByIndex(spawn_index);

		if (!spawn) {
			LogError(LOG_PLAYER, 0, "Player tried to change their target to a spawn they do not currently have!");
			return;
		}

		std::shared_ptr<PlayerController> controller = client->GetController();
		controller->SetTarget(spawn, false);
	}

	uint16_t spawn_index;
	uint8_t unknown;
};