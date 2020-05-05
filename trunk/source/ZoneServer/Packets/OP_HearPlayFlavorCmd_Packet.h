#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_HearPlayFlavorCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_HearPlayFlavorCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		language = 0;
		memset(unknown5, 0, sizeof(unknown5));
		key1 = 0;
		key2 = 0;
	}

	void RegisterElements() {
		RegisterUInt32(spawnID);
		RegisterUInt32(targetID);
		Register16String(mp3);
		Register16String(speakerName);
		Register16String(targetName);
		Register16String(text);
		Register16String(emoteText);
		Register16String(emote);
		RegisterUInt32(key1);
		RegisterUInt32(key2);
		RegisterUInt8(language);
		//bShowBubble maybe?
		RegisterUInt8(unknown4);
		RegisterBool(bUnderstood);
		//These are probably the same 10 bytes that are unknown in other chat packets(a 1 byte followed by 4 byte, then another 1 another 4)
		RescopeArrayElement(unknown5);
		RegisterUInt8(unknown5)->SetCount(10);
	}

	uint32_t spawnID;
	uint32_t targetID;
	std::string mp3;
	std::string speakerName;
	std::string targetName;
	std::string text;
	std::string emoteText;
	//This is just a visual state name
	std::string emote;
	uint32_t key1;
	uint32_t key2;
	uint8_t language;
	uint8_t unknown4;
	bool bUnderstood;
	uint8_t unknown5[10];
};