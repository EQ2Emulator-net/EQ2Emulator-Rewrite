#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_OpenDialogCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_OpenDialogCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();

		conversationID = 0;
		unknown2 = 0xFF;
		bCloseable = true;
		unknown = 0;
		spawnID = 0xFFFFFFFF;
		key1 = 0;
		key2 = 0;
	}

	class Substruct_Response : public PacketSubstruct {
	public:
		Substruct_Response(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		}

		void RegisterElements() override {
			Register16String(response);
		}

		std::string response;
	};

	void RegisterElements() {
		RegisterUInt32(conversationID);
		Register16String(title);
		Register16String(message);
		auto e = RegisterUInt16(numResponses);
		e->SetMyArray(RegisterArray(responseArray, Substruct_Response));
		if (GetVersion() >= 1096) {
			RegisterUInt8(unknown);
		}
		RegisterBool(bCloseable);
		RegisterUInt32(spawnID);
		Register16String(voiceFile);
		RegisterUInt8(unknown2);
		RegisterUInt32(key1);
		RegisterUInt32(key2);
	}

	uint32_t conversationID;
	std::string title;
	std::string message;
	uint16_t numResponses;
	bool bCloseable;
	uint8_t unknown;
	std::vector<Substruct_Response> responseArray;
	uint32_t spawnID;
	std::string voiceFile;
	uint32_t key1;
	uint32_t key2;
	uint8_t unknown2;
};