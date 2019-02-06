#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"

class OP_LoginRequestMsg_Packet : public EQ2Packet {
public:
	OP_LoginRequestMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();

		opcode = 0;

		Unknown2a[0] = 0;
		Unknown2a[1] = 0;

		memset(Unknown5, 0, sizeof(Unknown5));
	}

	/*	
<Struct Name="LS_LoginRequest" ClientVersion="1" OpcodeName="OP_LoginRequestMsg">
<Data ElementName="accesscode" Type="EQ2_16BitString" />
<Data ElementName="unknown1" Type="EQ2_16BitString" />
<Data ElementName="username" Type="EQ2_16BitString" />
<Data ElementName="password" Type="EQ2_16Bit_String" />
<Data ElementName="unknown2" Type="int8" Size="8" />
<Data ElementName="version" Type="int32" />
<Data ElementName="unknown3" Type="int16" />
<Data ElementName="unknown4" Type="int32" />
</Struct>

<Struct Name="LS_LoginRequest" ClientVersion="1212" OpcodeName="OP_LoginRequestMsg">
<Data ElementName="accesscode" Type="EQ2_16BitString" />
<Data ElementName="unknown1" Type="EQ2_16BitString" />
<Data ElementName="username" Type="EQ2_16BitString" />
<Data ElementName="password" Type="EQ2_16Bit_String" />
<Data ElementName="unknown2" Type="int8" Size="8" />
<Data ElementName="unknown3" Type="int8" Size="2" />
<Data ElementName="version" Type="int16" />
<Data ElementName="unknown4" Type="int8" />
<Data ElementName="unknown5" Type="int32" Size="3" />
<Data ElementName="unknown6" Type="int16" />
<Data ElementName="unknown7" Type="EQ2_16Bit_String" />
</Struct>	
*/
	std::string AccessCode;
	std::string Unknown1;
	std::string Username;
	std::string Password;
	std::string Unknown6;
	uint8_t Unknown2[8];
	uint8_t Unknown2a[2];
	uint16_t Version;
	uint16_t Unknown3;
	uint32_t Unknown4;
	uint8_t Unknown5[7];

	void HandlePacket(Client* client) {
		client->SetVersion(Version);

		if (Password == "")
			Password = AccessCode;

		client->LogIn(Username, Password);
	}

private:
	void RegisterElements() {
		Register16String(AccessCode);
		Register16String(Unknown1);
		Register16String(Username);
		Register16String(Password);
		uint8_t& Unknown2 = this->Unknown2[0];
		RegisterUInt8(Unknown2)->SetCount(8);

		if (GetVersion() >= 1212) {
			uint8_t& Unknown2a = this->Unknown2a[0];
			RegisterUInt8(Unknown2a)->SetCount(2);
		}

		RegisterUInt16(Version);
		RegisterUInt16(Unknown3);
		RegisterUInt32(Unknown4);

		if (GetVersion() >= 1212) {
			uint8_t& Unknown5 = this->Unknown5[0];
			RegisterUInt8(Unknown5)->SetCount(7);
			Register16String(Unknown6);
		}
	}
};
