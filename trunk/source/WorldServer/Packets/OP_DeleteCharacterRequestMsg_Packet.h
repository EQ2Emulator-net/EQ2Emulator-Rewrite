#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

#include "OP_DeleteCharacterReplyMsg_Packet.h"
#include "../Database/WorldDatabase.h"
#include "../WorldServer/WorldServer.h"

extern WorldDatabase database;

class OP_DeleteCharacterRequestMsg_Packet : public EQ2Packet {
public:
	OP_DeleteCharacterRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		char_id = 0;
		server_id = 0;
		unknown = 0;
		name = "";
	}

	void HandlePacket(Client* client) {
		OP_DeleteCharacterReplyMsg_Packet* reply = new OP_DeleteCharacterReplyMsg_Packet(client->GetVersion());

		if (database.DeleteCharacter(client->GetAccountID(), char_id, name))
			reply->response = 1;
		else
			reply->response = 0;

		reply->server_id = server_id;
		reply->char_id = char_id;
		reply->account_id = client->GetAccountID();
		reply->name = name;
		reply->max_characters = client->GetServer()->GetMaxCharactersPerAccount();
		client->QueuePacket(reply);
	}

	uint32_t char_id;
	uint32_t server_id;
	uint32_t unknown;
	std::string name;

private:
	void RegisterElements() {
		RegisterUInt32(char_id);
		RegisterUInt32(server_id);
		RegisterUInt32(unknown);
		Register16String(name);
	}
};

/*
<Struct Name = "LS_DeleteCharacterRequest" ClientVersion = "1" OpcodeName = "OP_DeleteCharacterRequestMsg">
<Data ElementName = "char_id" Type = "int32" Size = "1" / >
<Data ElementName = "server_id" Type = "int32" Size = "1" / >
<Data ElementName = "unknown" Type = "int32" Size = "1" / >
<Data ElementName = "name" Type = "EQ2_16Bit_String" / >
< / Struct>
*/