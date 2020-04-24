#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include <string>

class OP_OnscreenMsgMsg_Packet : public EQ2Packet {
public:
	OP_OnscreenMsgMsg_Packet(uint32_t ver) : EQ2Packet(ver) {
		RegisterElements();
		msgType = 9;
		//5 seconds seems like a good default?
		durationSecs = 5.f;
	}

	void RegisterElements() {
		if (GetVersion() > 283) {
			RegisterUInt8(msgType);
		}
		Register16String(text);
		Register16String(sound);
		RegisterFloat(durationSecs);
		RegisterEQ2Color(color);
	}

	//2 is the ding animation
	//16 is an achievement unlocked message
	uint8_t msgType;

	std::string text;
	std::string sound;
	float durationSecs;
	EQ2Color color;
};