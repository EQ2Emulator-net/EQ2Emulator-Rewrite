#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_EqDestroyGhostCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqDestroyGhostCmd_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version) {
		RegisterElements();
	}

	uint16_t spawn_index;
	uint8_t bDelete;

private:
	void RegisterElements() {
		RegisterOversizedByte(spawn_index);
		RegisterUInt8(bDelete);
	}
};

/*
<Struct Name = "WS_DestroyGhostCmd" ClientVersion = "1" OpcodeName = "OP_ClientCmdMsg" OpcodeType = "OP_EqDestroyGhostCmd">
<Data ElementName = "spawn_index" Type = "int16" OversizedValue = "255" / >
<Data ElementName = "delete" Type = "int8" / >
< / Struct>
*/