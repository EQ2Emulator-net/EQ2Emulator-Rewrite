#pragma once


#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"
#include "../ZoneTalk/ZoneTalk.h"

#include "OP_PlayCharacterReplyMsg_Packet.h"

extern ZoneTalk zoneTalk;

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
		zoneTalk.GetAvailableZone(client, char_id);
	}

	uint32_t char_id;
	uint32_t server_id;
	uint8_t unknown[3]; // Size="3"

private:
	void RegisterElements() {
		RegisterUInt32(char_id);
		RegisterUInt32(server_id);
		uint8_t& Unknown = unknown[0];
		RegisterUInt8(Unknown)->SetCount(3);
	}
};

/*
<Struct Name="LS_PlayRequest" ClientVersion="1" OpcodeName="OP_PlayCharacterRequestMsg">
<Data ElementName="char_id" Type="int32" Size="1" />
<Data ElementName="server_id" Type="int32" Size="1" />
<Data ElementName="unknown" Type="int8" Size="3" />
</Struct>
*/