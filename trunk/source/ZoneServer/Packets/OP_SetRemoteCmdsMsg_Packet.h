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

	uint16_t num_commands;
	std::vector<std::string> commands_array;

private:
	void RegisterElements() {
		PacketUInt16* asize = RegisterUInt16(num_commands);
		asize->SetMyArray(RegisterElementArray(commands_array, std::string, Packet8String));
	}

};

/*
<Struct Name="WS_SetRemoteCmdsMsg" ClientVersion="1" OpcodeName="OP_SetRemoteCmdsMsg" >
<Data ElementName="num_commands" Type="int16" />
<Data ElementName="commands_array" Type="Array" ArraySizeVariable="num_commands">
	<Data ElementName="name" Type="EQ2_8Bit_String" Size="1" />
</Data>
*/