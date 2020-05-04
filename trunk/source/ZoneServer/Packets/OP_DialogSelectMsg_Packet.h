#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_DialogSelectMsg_Packet : public EQ2Packet {
public:
	OP_DialogSelectMsg_Packet(uint32_t ver) : EQ2Packet(ver) {
		conversationID = 0;
		selection = 0;
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt32(conversationID);
		RegisterUInt32(selection);
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		client->chat.HandleDialogSelection(conversationID, selection);
	}

	uint32_t conversationID;
	uint32_t selection;
};