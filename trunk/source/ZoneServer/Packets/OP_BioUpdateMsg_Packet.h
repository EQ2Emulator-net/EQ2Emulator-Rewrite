#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"
#include "../Database/ZoneDatabase.h"

#include "OP_BioUpdateMsg_Packet.h"

extern ZoneDatabase database;

class OP_BioUpdateMsg_Packet : public EQ2Packet {
public:
	OP_BioUpdateMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();

		biography = "";	
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		if (!database.CharacterUpdateBiography(client->GetCharacterID(), biography.c_str())) {
			LogError(LOG_DATABASE, 0, "Failed to update Character ID %u biography", client->GetCharacterID());
		}
	}

	std::string biography;

private:
	void RegisterElements() {
		Register16String(biography);
	}
};