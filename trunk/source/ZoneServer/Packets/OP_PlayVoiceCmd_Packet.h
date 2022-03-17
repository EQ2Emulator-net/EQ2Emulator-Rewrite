#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_PlayVoiceCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_PlayVoiceCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
		language = 0;
		bMuffled = false;
		key1 = 0;
		key2 = 0;
		spawnID = -1;
	}

	void RegisterElements() {
		RegisterUInt32(spawnID);
		Register16String(voiceFile);
		RegisterUInt8(language);
		Register16String(garbled);
		RegisterUInt32(key1);
		RegisterUInt32(key2);
		RegisterBool(bMuffled);
	}

	uint32_t spawnID;
	std::string voiceFile;
	std::string garbled;
	uint32_t key1;
	uint32_t key2;
	uint8_t language;
	bool bMuffled;
};