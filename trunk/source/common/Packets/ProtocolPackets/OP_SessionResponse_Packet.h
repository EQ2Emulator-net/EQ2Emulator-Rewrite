#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_SessionResponse_Packet : public ProtocolPacket {
public:
	OP_SessionResponse_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_SessionResponse;
	}

	uint32_t Session;
	uint32_t Key;
	uint8_t UnknownA;
	uint8_t Format;
	uint8_t UnknownB;
	uint32_t MaxLength;
	uint32_t UnknownD;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt32(Session));
		elements.push_back(new PacketUInt32(Key));
		elements.push_back(new PacketUInt8(UnknownA));
		elements.push_back(new PacketUInt8(Format));
		elements.push_back(new PacketUInt8(UnknownB));
		elements.push_back(new PacketUInt32(MaxLength));
		elements.push_back(new PacketUInt32(UnknownD));
	}
};