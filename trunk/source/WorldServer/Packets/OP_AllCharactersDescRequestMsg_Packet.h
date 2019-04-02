#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"
#include "OP_AllCharactersDescReplyMsg_Packet.h"
#include "../Database/WorldDatabase.h"
#include "../WorldServer/WorldServer.h"

extern WorldDatabase database;

class OP_AllCharactersDescRequestMsg_Packet : public EQ2Packet {
public:
	OP_AllCharactersDescRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {

	}

	void HandlePacket(Client* client) {
		OP_AllCharactersDescReplyMsg_Packet* char_list = new OP_AllCharactersDescReplyMsg_Packet(client->GetVersion());
		database.LoadCharacters(client->GetAccountID(), char_list, client->GetServer()->GetMaxLevel());
		char_list->AccountID = client->GetAccountID();
		char_list->MaxAllowedCharacters = client->GetServer()->GetMaxCharactersPerAccount();
		client->QueuePacket(char_list);
	}

private:

};
