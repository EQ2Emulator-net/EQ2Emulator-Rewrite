#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"
#include "../Database/ZoneDatabase.h"

extern ZoneDatabase database;

class OP_ReadyToZoneMsg_Packet : public EQ2Packet {
public:
	OP_ReadyToZoneMsg_Packet(uint32_t version) : EQ2Packet(version) {
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		auto controller = client->GetController();
		if (auto player = client->GetController()->GetControlled()) {
			auto charSheet = controller->GetCharacterSheet();
			database.SaveSingleCharacter(*charSheet);
		}

		client->bZoningDisconnect = true;

		client->SendDisconnect(6);
	}

private:
};
