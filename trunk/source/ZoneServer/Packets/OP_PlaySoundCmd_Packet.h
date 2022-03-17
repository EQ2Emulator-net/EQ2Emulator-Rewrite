#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_PlaySoundCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_PlaySoundCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		unk = 0.0f;
	}

	void RegisterElements() {
		Register16String(sound);
		if (GetVersion() >= 845) {
			RegisterFloat(unk);
		}
	}

	std::string sound;
	float unk;
};