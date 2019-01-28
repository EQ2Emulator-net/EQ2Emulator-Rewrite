#pragma once

#include "../ProtocolPacket.h"

class OP_KeepAlive_Packet : public ProtocolPacket {
public:
	OP_KeepAlive_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_KeepAlive;
		HasCRC = true;
	}

private:
	void RegisterElements() {

	}
};