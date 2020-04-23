#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include <string>

class OP_OnscreenMsgMsg_Packet : public EQ2Packet {
public:
	OP_OnscreenMsgMsg_Packet(uint32_t ver) : EQ2Packet(ver) {
		RegisterElements();
		chatFilterID = 0;
		//5 seconds seems like a good default?
		durationSecs = 5.f;
	}

	void RegisterElements() {
		if (GetVersion() > 283) {
			RegisterUInt8(chatFilterID);
		}
		Register16String(text);
		Register16String(sound);
		RegisterFloat(durationSecs);
		RegisterEQ2Color(color);
	}

	//educated guess on this being the chat filter
	uint8_t chatFilterID;
	std::string text;
	std::string sound;
	float durationSecs;
	EQ2Color color;
};