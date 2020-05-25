#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

//Character Creation Replies
#define UNKNOWNERROR_REPLY				0
#define CREATESUCCESS_REPLY				1
#define NOSERVERSAVAIL_REPLY			2
#define CREATEPENDING_REPLY				3
#define MAXCHARSALLOWED_REPLY			4
#define INVALIDRACE_REPLY				5
#define INVALIDCITY_REPLY				6
#define INVALIDCLASS_REPLY				7
#define INVALIDGENDER_REPLY				8
#define INVALIDFIRST_LVL_REPLY			9
#define BADNAMELENGTH_REPLY				10
#define NAMEINVALID_REPLY				11
#define NAMEFILTER_REPLY				12 // name_filter reply (bad word or blocked words)
#define NAMETAKEN_REPLY					13
#define OVERLOADEDSERVER_REPLY			14
#define EXCHANGENOTALLOWED_REPLY		15
#define INVALIDFEATURES1_REPLY			16
#define INVALIDRACE_APPEARANCE_REPLY	17
#define INVALID_APPEARANCE_REPLY		18

class OP_CreateCharacterReplyMsg_Packet : public EQ2Packet {
public:
	OP_CreateCharacterReplyMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		account_id = 0;
		unknown = 0xFFFFFFFF;
		response = 0;
		name = "";
	}

	uint32_t account_id;
	uint32_t unknown; // 1189
	uint8_t response;
	std::string name;

private:
	void RegisterElements() {
		RegisterUInt32(account_id);
		if (GetVersion() >= 1189)
			RegisterUInt32(unknown);
		RegisterUInt8(response);
		Register16String(name);
	}

};

/*
<Struct Name="LS_CreateCharacterReply" ClientVersion="1" OpcodeName="OP_CreateCharacterReplyMsg">
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
</Struct>
<Struct Name="LS_CreateCharacterReply" ClientVersion="1189" OpcodeName="OP_CreateCharacterReplyMsg">
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="unknown" Type="int32" Size="1" />
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
</Struct>
*/