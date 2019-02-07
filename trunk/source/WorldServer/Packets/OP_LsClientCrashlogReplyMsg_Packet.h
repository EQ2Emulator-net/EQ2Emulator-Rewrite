#pragma once

#include "ClientLogReply_Packet.h"

class OP_LsClientCrashlogReplyMsg_Packet : public ClientLogReply_Packet {
public:
	OP_LsClientCrashlogReplyMsg_Packet(uint16_t version)
		: ClientLogReply_Packet(version) {
		name = "Crash Log";
	}
};