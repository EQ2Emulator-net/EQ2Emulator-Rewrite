#pragma once


#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"
#include "../ZoneTalk/ZoneTalk.h"
#include "../Database/WorldDatabase.h"
#include "../WorldServer/CharacterList.h"

#include "OP_PlayCharacterReplyMsg_Packet.h"

extern ZoneTalk zoneTalk;
extern WorldDatabase database;
extern CharacterList g_characterList;

class OP_PlayCharacterRequestMsg_Packet : public EQ2Packet {
public:
	OP_PlayCharacterRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		char_id = 0;
		server_id = 0;
		memset(unknown, 0, sizeof(unknown));
	}

	void HandlePacket(std::shared_ptr<Client> client) {
		if (g_characterList.AccountIsOnline(client->GetAccountID())) {
			OP_PlayCharacterReplyMsg_Packet* p = new OP_PlayCharacterReplyMsg_Packet(client->GetVersion());
			p->response = PlayCharacterResponse::EAccountInUse;
			client->QueuePacket(p);
			return;
		}

		uint32_t zone_id = database.GetZoneIDForCharacter(char_id);
		auto zs = zoneTalk.GetAvailableZone(zone_id);
		client->SetPendingZone(char_id, zone_id, 0);

		if (zs) {
			uint32_t access_code = zoneTalk.TransferClientToZone(zs, char_id, zone_id, client->GetAccountID(), 0, false);
			client->pending_access_code = access_code;
		}
		else if (!zoneTalk.HasPendingZone(zone_id)) {
			// no zone servers connected
			OP_PlayCharacterReplyMsg_Packet* p = new OP_PlayCharacterReplyMsg_Packet(client->GetVersion());
			p->response = PlayCharacterResponse::EZoneDown;
			client->QueuePacket(p);
		}
		else {
			zoneTalk.AddPendingZoneClient(zone_id, client);
		}
	}

	uint32_t char_id;
	uint32_t server_id;
	uint8_t unknown[3]; // Size="3"

private:
	void RegisterElements() {
		RegisterUInt32(char_id);
		RegisterUInt32(server_id);
		if (GetVersion() > 283) {
			uint8_t& Unknown = unknown[0];
			RegisterUInt8(Unknown)->SetCount(3);
		}
	}
};

/*
<Struct Name="LS_PlayRequest" ClientVersion="1" OpcodeName="OP_PlayCharacterRequestMsg">
<Data ElementName="char_id" Type="int32" Size="1" />
<Data ElementName="server_id" Type="int32" Size="1" />
<Data ElementName="unknown" Type="int8" Size="3" />
</Struct>
*/