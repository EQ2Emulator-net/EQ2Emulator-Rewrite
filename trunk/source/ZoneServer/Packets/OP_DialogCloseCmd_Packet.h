#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"

class OP_DialogCloseCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_DialogCloseCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		conversationID = 0;
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt32(conversationID);
	}

	uint32_t conversationID;
};