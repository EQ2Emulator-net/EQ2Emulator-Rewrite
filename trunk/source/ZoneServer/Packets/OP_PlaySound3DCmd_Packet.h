#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_PlaySound3DCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_PlaySound3DCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		unk1 = 1.0f;
		unk2 = 20.0f;
		unk3 = 0.0f;
		unk4 = 1.0f;
	}

	void RegisterElements() {
		Register16String(sound);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
		RegisterFloat(unk1);
		if (GetVersion() >= 67650) {
			RegisterFloat(unk4);
		}
		RegisterFloat(unk2);
		RegisterFloat(unk3);
	}

	std::string sound;
	float x, y, z;
	//These are probably related to volume, attentuation, distance etc
	float unk1;
	float unk2;
	float unk3;
	float unk4;
};