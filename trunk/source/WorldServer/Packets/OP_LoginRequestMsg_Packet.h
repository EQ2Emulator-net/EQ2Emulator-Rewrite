#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"


//enum class LoginReply : uint8_t {
//	EAccepted = 0,
//	EBadPassword = 1,
//	ECurrentlyPlaying = 2,
//	EBadVersion = 6,
//	EUnknown = 7
//};

class OP_LoginRequestMsg_Packet : public EQ2Packet {
public:
	OP_LoginRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		opcode = 0;

		Version = 0;
		Unknown9 = 0;
		memset(Unknown10, 0, sizeof(Unknown10));
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
	int32_t Unknown2;
	int32_t Unknown3;
	uint64_t Version;
	std::string Unknown4[2];
	int32_t Unknown5;
	bool bFullscreen;
	uint16_t SOEBuild;
	std::string Unknown7;
	std::string Unknown8[2];
	uint64_t Unknown9;
	std::string ComputerName;
	int32_t Unknown10[5];
	std::string GPUName;
	std::string CPUName;

	void HandlePacket(std::shared_ptr<Client> client) {
		client->SetVersion((uint32_t)Version);

		if (Password == "")
			Password = AccessCode;

		client->LogIn(Username, Password);
	}

	bool Read(const unsigned char* in_buf, uint32_t off, uint32_t bufsize) override {
		bool ret = Packet::Read(in_buf, off, bufsize);
		if (GetVersion() >= 1212) {
			//We were just using 2 bytes of this dateTime before we knew what it was as a version
			//It rolled over to 0x2F00000000, factor the value over that rollover into our new version
			uint32_t rolloverByte = (Version >> 32) & 0xFF;
			uint32_t additiveValue = (rolloverByte - 0x2E) << 16;

			//Move the 2 bytes we want all the way over
			Version >>= 16;
			//Strip off any higher bits
			Version &= 0xFFFF;
			//Add any extra rollover value
			Version |= additiveValue;
		}
		return ret;
	}

private:
	void RegisterElements() {
		Register16String(AccessCode);
		Register16String(Unknown1);
		Register16String(Username);
		Register16String(Password);
		RegisterInt32(Unknown2);
		RegisterInt32(Unknown3);

		if (GetVersion() >= 1212) {
			RegisterUInt64(Version);
		}
		else {
			uint16_t& Version = reinterpret_cast<uint16_t&>(this->Version);
			RegisterUInt16(Version);
		}

		std::string& Unknown4 = this->Unknown4[0];
		Register16String(Unknown4)->SetCount(2);
		RegisterInt32(Unknown5);
		RegisterBool(bFullscreen);
		RegisterUInt16(SOEBuild);
		Register16String(Unknown7);

		if (GetVersion() >= 1212) {
			std::string& Unknown8 = this->Unknown8[0];
			Register16String(Unknown8)->SetCount(2);
			RegisterUInt64(Unknown9);
			Register16String(ComputerName);
			int32_t& Unknown10 = this->Unknown10[0];
			RegisterInt32(Unknown10)->SetCount(5);
			Register16String(GPUName);
			Register16String(CPUName);
		}
	}
};
