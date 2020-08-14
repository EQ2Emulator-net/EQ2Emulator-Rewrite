#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_FlythroughEndMsg_Packet : public EQ2Packet {
public:
	OP_FlythroughEndMsg_Packet(uint32_t ver) : EQ2Packet(ver) {
	}

	~OP_FlythroughEndMsg_Packet() = default;

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_PACKET, 0, "Received a FlythroughEndMsg packet!");
	}
};