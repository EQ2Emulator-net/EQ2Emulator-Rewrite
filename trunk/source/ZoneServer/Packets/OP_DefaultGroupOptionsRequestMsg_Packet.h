#pragma once
#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

#include "../ZoneServer/Client.h"
#include "OP_DefaultGroupOptionsMsg_Packet.h"

class OP_DefaultGroupOptionsRequestMsg_Packet : public EQ2Packet {
public:
	OP_DefaultGroupOptionsRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

	}

	void HandlePacket(std::shared_ptr<Client> client) {
		// Move to client and load options from the db
		OP_DefaultGroupOptionsMsg_Packet* p = new OP_DefaultGroupOptionsMsg_Packet(client->GetVersion());
		p->loot_method = 1;
		p->loot_items_rarity = 1;
		p->auto_split_coin = 1;
		p->default_yell_method = 1;
		client->QueuePacket(p);
	}

private:
	void RegisterElements() {
	}

};

/*
<Struct Name="WS_DefaultGroupOptionsRequestMsg" ClientVersion="1" OpcodeName="OP_DefaultGroupOptionsRequestMsg">
</Struct>
*/