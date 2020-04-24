#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_DisplayTextCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_DisplayTextCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		onscreenMsgType = 0xFF;
		bOnscreen = false;
	}

	void RegisterElements() {
		RegisterUInt8(chatFilterID);
		Register16String(text);
		RegisterUInt8(onscreenMsgType);
		auto e = RegisterBool(bOnscreen);
		if (GetVersion() >= 284) {
			Register16String(onscreenSound)->SetIsVariableSet(e);
		}
	}

	uint8_t chatFilterID;
	uint8_t onscreenMsgType;
	bool bOnscreen;
	std::string text;
	std::string onscreenSound;
};