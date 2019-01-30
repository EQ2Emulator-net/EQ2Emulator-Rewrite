#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_Ack_Packet : public ProtocolPacket {
public:
	OP_Ack_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_Ack;
		HasCRC = true;
	}

	uint16_t Sequence;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt16(Sequence));
	}
};