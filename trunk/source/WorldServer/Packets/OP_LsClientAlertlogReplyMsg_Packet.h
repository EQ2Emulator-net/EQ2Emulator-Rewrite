#pragma once

#include "ClientLogReply_Packet.h"

class OP_LsClientAlertlogReplyMsg_Packet : public ClientLogReply_Packet {
public:
	OP_LsClientAlertlogReplyMsg_Packet(uint32_t version)
		: ClientLogReply_Packet(version) {
		name = "Alert Log";
	}
};