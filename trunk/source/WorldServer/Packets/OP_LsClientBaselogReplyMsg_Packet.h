#pragma once

#include "ClientLogReply_Packet.h"

class OP_LsClientBaselogReplyMsg_Packet : public ClientLogReply_Packet {
public:
	OP_LsClientBaselogReplyMsg_Packet(uint16_t version)
		: ClientLogReply_Packet(version) {
		name = "Base Log";
	}
};