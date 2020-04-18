#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_ChangeZoneMsg_Packet : public EQ2Packet {
public:
	OP_ChangeZoneMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	uint32_t account_id;
	std::string ip_address;
	uint32_t key;
	uint16_t port;

private:
	void RegisterElements() {
		RegisterUInt32(account_id);
		RegisterUInt32(key);
		Register16String(ip_address);
		RegisterUInt16(port);
	}

};

/*
<Struct Name="WS_RemoteCmdMsg" ClientVersion="1" OpcodeName="OP_RemoteCmdMsg" >
<Data ElementName="command_handler" Type="int16" />
<Data ElementName="arguments" Type="EQ2_16Bit_String" Size="1" />
</Struct>
*/