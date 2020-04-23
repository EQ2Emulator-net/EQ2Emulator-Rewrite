#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_HearChatCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_HearChatCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		bFromNPC = false;
		unknown = 0;
		bUnderstood = true;
		bShowBubble = true;
		//common language
		language = 0;

		unknowna = 0;
		unknownb = 0;
		unknownc = 0;
		unknownd = 0;
		unknown4 = 0;
		unknown5 = 0;
		unknown6 = 0;
		unknown7 = 0;
	}

	void RegisterElements() {
		RegisterBool(bFromNPC);
		if (GetVersion() > 283) {
			RegisterUInt8(unknown);
		}
		RegisterUInt32(fromSpawnID);
		RegisterUInt32(toSpawnID);
		Register16String(fromName);
		Register16String(toName);
		if (GetVersion() > 283) {
			RegisterUInt8(unknowna);
			RegisterUInt32(unknownb);
			RegisterUInt8(unknownc);
			//The old code structs list this as time ?
			RegisterUInt32(unknownd);
		}
		RegisterUInt8(chatFilterID);
		RegisterUInt8(language);
		Register16String(message);
		Register16String(channelName);
		RegisterBool(bShowBubble);
		if (GetVersion() > 283) {
			//Maybe this is where bShowBubble is for classic? not sure which they would have
			RegisterBool(bUnderstood);
			RegisterUInt8(unknown4);
			if (GetVersion() >= 57080) {
				RegisterUInt8(unknown5);
				if (GetVersion() >= 67650) {
					RegisterUInt8(unknown6);
					RegisterUInt8(unknown7);
				}
			}
		}
		
	}

	bool bFromNPC;
	uint8_t unknown;
	uint8_t chatFilterID;
	uint8_t language;
	uint32_t fromSpawnID;
	uint32_t toSpawnID;
	std::string fromName;
	std::string toName;
	std::string message;
	std::string channelName;

	bool bShowBubble;
	bool bUnderstood;
	uint8_t unknowna;
	uint8_t unknownc;
	uint32_t unknownb;
	uint32_t unknownd;

	uint8_t unknown4;
	uint8_t unknown5;
	uint8_t unknown6;
	uint8_t unknown7;
};