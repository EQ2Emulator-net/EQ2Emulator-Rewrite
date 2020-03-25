#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

#define PLAY_ERROR_PROBLEM					0
#define PLAY_CHARACTER_SUCCESS              1
#define PLAY_ERROR_ZONE_DOWN				4
#define PLAY_ERROR_CHAR_NOT_LOADED			5
#define PLAY_ERROR_CHAR_NOT_FOUND			6
#define PLAY_ERROR_ACCOUNT_IN_USE			7
#define PLAY_ERROR_SERVER_TIMEOUT			8
#define PLAY_ERROR_SERVER_SHUTDOWN			9
#define PLAY_ERROR_LOADING_ERROR			10
#define PLAY_ERROR_EXCHANGE_SERVER			11
#define PLAY_ERROR_REGION_SERVER			12
#define PLAY_ERROR_CLASS_INVALID			13
#define PLAY_ERROR_TOO_MANY_CHARACTERS		14
#define PLAY_ERROR_EOF_EXP_NOT_FOUND		15
#define PLAY_ERROR_UNKNOWN_RESPONSE			16
#define PLAY_ERROR_UNKNOWN					17
#define PLAY_ERROR_ACCOUNT_BANNED			18
#define PLAY_ERROR_PROHIBITED				19

enum class PlayCharacterResponse : uint8_t {
	EProblem = 0,
	ESuccess = 1,
	EZoneDown = 4,
	ECharacterNotLoaded = 5,
	ECharacterNotFound = 6,
	EAccountInUse = 7,
	EServerTimeout = 8,
	EServerShutdown = 9,
	ELoadingError = 10,
	EExchangeServer = 11,
	ERegionServer = 12,
	EClassInvalid = 13,
	ETooManyCharacters = 14,
	EEOFExpansionNotFound = 15,
	EUnknownResponse = 16,
	EUnknown = 17,
	EAccountBanned = 18,
	EProhibited = 19
};

class OP_PlayCharacterReplyMsg_Packet : public EQ2Packet {
public:
	OP_PlayCharacterReplyMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		response = PlayCharacterResponse::EProblem;
		memset(unknown1, 0, sizeof(unknown1));
		port = 0;
		account_id = 0;
		access_code = 0;
	}

	PlayCharacterResponse response;
	uint16_t unknown1[3]; // 1096 size = 1 // 60099 size = 3
	std::string server;
	uint16_t port;
	uint32_t account_id;
	uint32_t access_code;

private:
	void RegisterElements() {
		uint8_t& response = reinterpret_cast<uint8_t&>(this->response);
		RegisterUInt8(response);
		uint16_t& Unknown1 = unknown1[0];
		if (GetVersion() >= 1096 && GetVersion() < 60099)
			RegisterUInt16(Unknown1);
		else if (GetVersion() >= 60099)
			RegisterUInt16(Unknown1)->SetCount(3);
		Register8String(server);
		RegisterUInt16(port);
		RegisterUInt32(account_id);
		RegisterUInt32(access_code);
	}
};

/*
<Struct Name="LS_PlayResponse" ClientVersion="1" OpcodeName="OP_PlayCharacterReplyMsg">
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="server" Type="EQ2_8BitString" />
<Data ElementName="port" Type="int16" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="access_code" Type="int32" Size="1" />
</Struct>
<Struct Name="LS_PlayResponse" ClientVersion="1096" OpcodeName="OP_PlayCharacterReplyMsg">
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="unknown1" Type="int16" Size="1" />
<Data ElementName="server" Type="EQ2_8BitString" />
<Data ElementName="port" Type="int16" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="access_code" Type="int32" Size="1" />
</Struct>
<Struct Name="LS_PlayResponse" ClientVersion="60099" OpcodeName="OP_PlayCharacterReplyMsg">
<Data ElementName="response" Type="int8" Size="1" />
<Data ElementName="unknown1" Type="int16" Size="3" />
<Data ElementName="server" Type="EQ2_8BitString" />
<Data ElementName="port" Type="int16" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="access_code" Type="int32" Size="1" />
</Struct>
*/