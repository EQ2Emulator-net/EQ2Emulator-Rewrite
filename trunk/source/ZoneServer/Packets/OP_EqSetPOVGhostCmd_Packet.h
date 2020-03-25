#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"

class OP_EqSetPOVGhostCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqSetPOVGhostCmd_Packet(uint32_t version)
		: OP_ClientCmdMsg_Packet(version) {
		RegisterElements();

		spawn_id = 0xFFFFFFFF;
	}

	uint32_t spawn_id;

private:
	void RegisterElements() {
		RegisterUInt32(spawn_id);
	}

};
