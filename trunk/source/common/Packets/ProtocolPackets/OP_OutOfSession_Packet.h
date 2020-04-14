#pragma once

#include "../ProtocolPacket.h"

class OP_OutOfSession_Packet : public ProtocolPacket {
public:
	OP_OutOfSession_Packet()
		: ProtocolPacket() {

		opcode = OP_OutOfSession;
		HasCRC = false;
	}
};