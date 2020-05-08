#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"

class OP_EntityVerbsVerbMsg_Packet : public EQ2Packet {
public:
	OP_EntityVerbsVerbMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	uint32_t spawnID;
	std::string command;

	void RegisterElements() {
		RegisterUInt32(spawnID);
		Register16String(command);
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		auto spawn = client->GetSpawnByID(spawnID);
		if (spawn) {
			spawn->HandleEntityCommand(command, client);
		}
	}
};