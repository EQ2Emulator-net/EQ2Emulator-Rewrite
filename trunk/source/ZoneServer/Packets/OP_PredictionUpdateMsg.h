#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_PredictionUpdateMsg_Packet : public EQ2Packet {
public:
	OP_PredictionUpdateMsg_Packet(uint32_t version) : EQ2Packet(version) {
	}
	~OP_PredictionUpdateMsg_Packet() = default;

	void HandlePacket(std::shared_ptr<Client> client) override {
		client->QueuePacket(new OP_PredictionUpdateMsg_Packet(GetVersion()));
	}
};