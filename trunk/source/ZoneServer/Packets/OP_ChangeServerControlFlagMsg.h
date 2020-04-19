#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_ChangeServerControlFlagMsg_Packet : public EQ2Packet {
public:
	OP_ChangeServerControlFlagMsg_Packet(uint32_t version) : EQ2Packet(version),
	parameter1(0), parameter2(0), parameter3(0), parameter4(0), parameter5(0), value(false) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt8(parameter1);
		RegisterUInt8(parameter2);
		RegisterUInt8(parameter3);
		RegisterUInt8(parameter4);
		RegisterUInt8(parameter5);
		RescopeArrayElement(unknown);
		RegisterUInt8(unknown)->SetCount(3);
		RegisterBool(value);
	}

	//Pretty clearly just a bitflag, keep it as separate bytes?
	uint8_t parameter1;
	uint8_t parameter2;
	uint8_t parameter3;
	uint8_t parameter4;
	uint8_t parameter5;
	uint8_t unknown[3];
	bool value;	
};