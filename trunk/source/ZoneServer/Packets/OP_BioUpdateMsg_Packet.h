#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"
#include "../Database/ZoneDatabase.h"
#include "../Controllers/PlayerController.h"

#include "OP_BioUpdateMsg_Packet.h"

extern ZoneDatabase database;

class OP_BioUpdateMsg_Packet : public EQ2Packet {
public:
	OP_BioUpdateMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		CharacterSheet* charSheet = client->GetController()->GetCharacterSheet();
		charSheet->biography = std::move(biography);
	}

	std::string biography;

private:
	void RegisterElements() {
		Register16String(biography);
	}
};