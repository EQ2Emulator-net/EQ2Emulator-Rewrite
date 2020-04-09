#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../Commands/CommandProcess.h"

extern CommandProcess g_commandProcess;

class OP_RemoteCmdMsg_Packet : public EQ2Packet {
public:
	OP_RemoteCmdMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		command_handler = 0;
		arguments = "";
	}

	uint16_t command_handler;
	std::string arguments;

	void HandlePacket(std::shared_ptr<Client> client) {
		LogDebug(LOG_CLIENT, 0, "Command (%u) received", command_handler);
		g_commandProcess.ProcessCommand(client, command_handler, arguments);
	}

private:
	void RegisterElements() {
		RegisterUInt16(command_handler);
		Register16String(arguments);
	}

};

/*
<Struct Name="WS_RemoteCmdMsg" ClientVersion="1" OpcodeName="OP_RemoteCmdMsg" >
<Data ElementName="command_handler" Type="int16" />
<Data ElementName="arguments" Type="EQ2_16Bit_String" Size="1" />
</Struct>
*/