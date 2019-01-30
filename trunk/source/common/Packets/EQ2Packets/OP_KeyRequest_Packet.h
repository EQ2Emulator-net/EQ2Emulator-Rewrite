#pragma once

#include "../EQ2Packet.h"
#include "../PacketElements/PacketElements.h"

class OP_KeyRequest_Packet : public EQ2Packet {
public:
	OP_KeyRequest_Packet()
		: EQ2Packet() {
		RegisterElements();

		opcode = 2;
		_Size = 60;
		Blah1 = 0xFFFFFFFF;
		Blah2 = 0xFFFFFFFF;
		Blah3 = 0xFFFFFFFF;
		Blah4 = 0xFFFFFFFF;
		Blah5 = 0xFFFFFFFF;
		Blah6 = 0xFFFFFFFF;
		Blah7 = 0xFFFFFFFF;
		Blah8 = 0xFFFFFFFF;
		Blah9 = 0xFFFFFFFF;
		Blah10 = 0xFFFFFFFF;
		Blah11 = 0xFFFFFFFF;
		Blah12 = 0xFFFFFFFF;
		Blah13 = 0xFFFFFFFF;
		Blah14 = 0xFFFFFFFF;
		Blah15 = 0xFFFFFFFF;
		Exponent_len = 1;
		Exponent = 1;

		//Size = 69;
	}

	uint32_t _Size;
	uint32_t Blah1;
	uint32_t Blah2;
	uint32_t Blah3;
	uint32_t Blah4;
	uint32_t Blah5;
	uint32_t Blah6;
	uint32_t Blah7;
	uint32_t Blah8;
	uint32_t Blah9;
	uint32_t Blah10;
	uint32_t Blah11;
	uint32_t Blah12;
	uint32_t Blah13;
	uint32_t Blah14;
	uint32_t Blah15;
	uint32_t Exponent_len;
	uint8_t Exponent;

private:
	void RegisterElements() {
		elements.push_back(new PacketUInt32(_Size));
		elements.push_back(new PacketUInt32(Blah1));
		elements.push_back(new PacketUInt32(Blah2));
		elements.push_back(new PacketUInt32(Blah3));
		elements.push_back(new PacketUInt32(Blah4));
		elements.push_back(new PacketUInt32(Blah5));
		elements.push_back(new PacketUInt32(Blah6));
		elements.push_back(new PacketUInt32(Blah7));
		elements.push_back(new PacketUInt32(Blah8));
		elements.push_back(new PacketUInt32(Blah9));
		elements.push_back(new PacketUInt32(Blah10));
		elements.push_back(new PacketUInt32(Blah11));
		elements.push_back(new PacketUInt32(Blah12));
		elements.push_back(new PacketUInt32(Blah13));
		elements.push_back(new PacketUInt32(Blah14));
		elements.push_back(new PacketUInt32(Blah15));
		elements.push_back(new PacketUInt32(Exponent_len));
		elements.push_back(new PacketUInt8(Exponent));
	}
};