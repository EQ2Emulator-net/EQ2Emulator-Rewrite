#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

class OP_DeleteCharacterReplyMsg_Packet : public EQ2Packet {
public:
	OP_DeleteCharacterReplyMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		response = 0;
		server_id = 0;
		char_id = 0;
		account_id = 0;
		name = "";
		max_characters = 0;
	}

	uint8_t response;
	uint32_t server_id;
	uint32_t char_id;
	uint32_t account_id;
	std::string name;
	uint32_t max_characters;

private:
	void RegisterElements() {
		RegisterUInt8(response);
		RegisterUInt32(server_id);
		RegisterUInt32(char_id);
		RegisterUInt32(account_id);
		Register16String(name);
		RegisterUInt32(max_characters);
	}
};

/*
<Struct Name="LS_DeleteCharacterResponse" ClientVersion="1" OpcodeName="OP_DeleteCharacterReplyMsg">
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="server_id" Type="int32" Size="1" />
<Data ElementName="char_id" Type="int32" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
<Data ElementName="max_characters" Type="int32" Size="1" />
</Struct>
*/