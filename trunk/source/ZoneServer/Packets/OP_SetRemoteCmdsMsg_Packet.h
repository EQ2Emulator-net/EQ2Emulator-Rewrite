#pragma once
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_SetRemoteCmdsMsg_Packet : public EQ2Packet {
public:
	OP_SetRemoteCmdsMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		num_commands = 0;
	}

	struct Command : public PacketSubstruct {

		std::string name;

		Command(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		}

		void RegisterElements() {
			Register8String(name);
		}
	};

	uint16_t num_commands;
	std::vector<Command> commands_array;

private:
	void RegisterElements() {
		PacketUInt16* asize = RegisterUInt16(num_commands);
		asize->SetMyArray(RegisterArray(commands_array, Command));
	}

};

/*
<Struct Name="WS_SetRemoteCmdsMsg" ClientVersion="1" OpcodeName="OP_SetRemoteCmdsMsg" >
<Data ElementName="num_commands" Type="int16" />
<Data ElementName="commands_array" Type="Array" ArraySizeVariable="num_commands">
	<Data ElementName="name" Type="EQ2_8Bit_String" Size="1" />
</Data>
*/