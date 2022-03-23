#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include <string>

class OP_DialogOpenCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_DialogOpenCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();

		conversationID = 0;
		language = 0;
		bCloseable = true;
		bSignatureDialog = false;
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
		e->SetMyArray(RegisterElementArray(responseArray, std::string, Packet16String));
		if (GetVersion() >= 1096) {
			RegisterBool(bSignatureDialog);
		}
		RegisterBool(bCloseable);
		RegisterUInt32(spawnID);
		Register16String(voiceFile);
		if (GetVersion() >= 864) {
			RegisterUInt8(language);
		}
		RegisterUInt32(key1);
		RegisterUInt32(key2);
	}

	uint32_t conversationID;
	std::string title;
	std::string message;
	uint16_t numResponses;
	bool bCloseable;
	//This bool makes the dialog have a blue background with yellow text
	bool bSignatureDialog;
	std::vector<std::string> responseArray;
	uint32_t spawnID;
	std::string voiceFile;
	uint32_t key1;
	uint32_t key2;
	uint8_t language;
};