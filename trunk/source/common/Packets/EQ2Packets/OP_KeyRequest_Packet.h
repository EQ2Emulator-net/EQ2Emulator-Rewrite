#pragma once

#include "../EQ2Packet.h"
#include "../PacketElements/PacketElements.h"
#include "OpcodeManager.h"

class OP_KeyRequest_Packet : public EQ2Packet {
public:
	OP_KeyRequest_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		opcode = 2;
		memset(Blah, 0xFF, sizeof(Blah));
		_Size = sizeof(Blah);
		Exponent_len = 1;
		Exponent = 1;
	}

	uint32_t _Size;
	uint32_t Blah[15];
	uint32_t Exponent_len;
	uint8_t Exponent;

private:
	void RegisterElements() {
		RegisterUInt32(_Size);
		RescopeArrayElement(Blah);
		RegisterUInt32(Blah)->SetCount(15);
		RegisterUInt32(Exponent_len);
		RegisterUInt8(Exponent);
	}

	void FindOpcode() override {
		//This packet is sent before we have a version so we won't be able to find an opcode
		opcode = 2;
		bOpcodeError = false;
	}
};