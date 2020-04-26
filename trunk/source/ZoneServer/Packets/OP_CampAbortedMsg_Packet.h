#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"

class OP_CampAbortedMsg_Packet : public EQ2Packet {
public:
	OP_CampAbortedMsg_Packet(uint32_t version) : EQ2Packet(version) {
	}
};
