#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"

class OP_EqSetControlGhostCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqSetControlGhostCmd_Packet(uint32_t version)
		: OP_ClientCmdMsg_Packet(version) {
		RegisterElements();
		spawn_id = 0xFFFFFFFF;
		speed = 0;
		air_speed = 0;
		size = 0;
		unknown2 = 0;
		memset(unknown, 0, sizeof(unknown));
	}

	uint32_t spawn_id;
	float speed;
	float air_speed; // 1096
	float size;
	uint8_t unknown2;
	uint8_t unknown[13]; // 1119 = 12, 1193 = 13

private:
	void RegisterElements() {
		RegisterUInt32(spawn_id);
		RegisterFloat(speed);
		if (GetVersion() >= 1096)
			RegisterFloat(air_speed);
		RegisterFloat(size);
		RegisterUInt8(unknown2);
		if (GetVersion() >= 1119) {
			uint8_t& Unknown = unknown[0];
			if (GetVersion() >= 1193)
				RegisterUInt8(Unknown)->SetCount(13);
			else
				RegisterUInt8(Unknown)->SetCount(12);
		}
	}

};