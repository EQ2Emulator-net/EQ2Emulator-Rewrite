#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"

class OP_SessionDisconnect_Packet : public ProtocolPacket {
public:
	OP_SessionDisconnect_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_SessionDisconnect;
		HasCRC = true;
	}

	uint32_t Session;
	uint16_t Reason;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt32(Session));
		elements.push_back(new PacketUInt16(Reason));
	}
};