#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../Packets/OP_ClientCmdMsg_Packet.h"
#include "../ZoneServer/Client.h"

class OP_EqCannedEmoteCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqCannedEmoteCmd_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version) {
		RegisterElements();

		performerID = 0xFFFFFFFF;
		animType = 0;
		targetID = 0xFFFFFFFF;
	}

	uint32_t performerID;
	std::string emoteMsg;
	uint32_t animType;
	uint32_t targetID;

	void RegisterElements() {
		RegisterUInt32(performerID);
		Register16String(emoteMsg);

		if (GetVersion() >= 57080) {
			RegisterUInt32(animType);
		}
		else {
			uint16_t& animType = reinterpret_cast<uint16_t&>(this->animType);
			RegisterUInt16(animType);
		}

		RegisterUInt32(targetID);
	}
};