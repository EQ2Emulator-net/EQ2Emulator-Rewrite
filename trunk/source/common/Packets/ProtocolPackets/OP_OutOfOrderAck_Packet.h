#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

//The "OutOfOrderAck" really just means a single packet is being acked rather than all packets up to the seq like the normal ack packet
class OP_OutOfOrderAck_Packet : public ProtocolPacket {
public:
	OP_OutOfOrderAck_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_OutOfOrderAck;
		HasCRC = true;
	}

	OP_OutOfOrderAck_Packet(uint16_t seq)
		: ProtocolPacket(),
		Sequence(seq)
	{
		RegisterElements();

		opcode = OP_OutOfOrderAck;
		HasCRC = true;
	}

	uint16_t Sequence;

private:
	void RegisterElements() {
		auto e = new PacketUInt16(Sequence);
		e->SetIsNetOrder(true);
		elements.push_back(e);
	}
};