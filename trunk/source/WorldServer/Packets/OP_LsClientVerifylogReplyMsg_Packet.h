#pragma once

#include "ClientLogReply_Packet.h"

class OP_LsClientVerifylogReplyMsg_Packet : public ClientLogReply_Packet {
public:
	OP_LsClientVerifylogReplyMsg_Packet(uint16_t version)
		: ClientLogReply_Packet(version) {
		name = "Verify Log";
	}
};