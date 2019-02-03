#pragma once

#include "../ProtocolPacket.h"

class OP_AppCombined_Packet : public ProtocolPacket {
public:
	OP_AppCombined_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_AppCombined;
		HasCRC = true;
	}

	OP_AppCombined_Packet(const unsigned char* buf, uint32_t len)
		: ProtocolPacket(buf, len) {
		RegisterElements();

		opcode = OP_AppCombined;
		HasCRC = true;
	}

private:
	void RegisterElements() {

	}
};