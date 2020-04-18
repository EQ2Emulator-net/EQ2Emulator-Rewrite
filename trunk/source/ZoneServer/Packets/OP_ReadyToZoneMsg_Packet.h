#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"

class OP_ReadyToZoneMsg_Packet : public EQ2Packet {
public:
	OP_ReadyToZoneMsg_Packet(uint32_t version) : EQ2Packet(version) {
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		client->SendDisconnect(6);
	}

private:
};
