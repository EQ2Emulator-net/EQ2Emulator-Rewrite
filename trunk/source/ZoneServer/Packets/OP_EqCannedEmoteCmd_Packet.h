#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../ZoneServer/Client.h"

#include "OP_EqCannedEmoteCmd_Packet.h"


class OP_EqCannedEmoteCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqCannedEmoteCmd_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version) {
		RegisterElements();

		spawn_id = 0;
		emote_msg = "";
		anim_type = 0;
		unknown = 0;

	}

	uint32_t spawn_id;
	std::string emote_msg;
	uint32_t anim_type;
	int32_t unknown;

	void RegisterElements() {
		RegisterUInt32(spawn_id);
		Register16String(emote_msg);

		if (GetVersion() >= 57080) {
			RegisterUInt32(anim_type);
		}
		else {
			uint16_t& anim_type = reinterpret_cast<uint16_t&>(this->anim_type);
			RegisterUInt16(anim_type);
		}

		RegisterInt32(unknown);
	}
};