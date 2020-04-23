#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_DisplayTextCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_DisplayTextCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		unknown = 0xFF;
		bOnscreen = false;
	}

	void RegisterElements() {
		RegisterUInt8(chatFilterID);
		Register16String(text);
		RegisterUInt8(unknown);
		auto e = RegisterBool(bOnscreen);
		if (GetVersion() >= 284) {
			Register16String(unkString)->SetIsVariableSet(e);
		}
	}

	uint8_t chatFilterID;
	//Maybe like a sound?  normally 0xFF
	uint8_t unknown;
	bool bOnscreen;
	std::string text;
	std::string unkString;
};