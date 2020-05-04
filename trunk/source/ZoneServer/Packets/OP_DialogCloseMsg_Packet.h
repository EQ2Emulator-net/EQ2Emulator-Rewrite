#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_DialogCloseMsg_Packet : public EQ2Packet {
public:
	OP_DialogCloseMsg_Packet(uint32_t ver) : EQ2Packet(ver) {
		conversationID = 0;
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt32(conversationID);
	}

	uint32_t conversationID;
};