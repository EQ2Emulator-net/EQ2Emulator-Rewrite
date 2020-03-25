#pragma once

#include <cstdint>

class EmuPacket;

namespace EmuOpcode {
	EmuPacket* GetPacketForOpcode(uint8_t op);
}

enum EmuOpcode_t : uint8_t {
	EMUOP_FORWARD_PACKET = 1,
	EMUOP_REGISTER_ZONESERVER = 2,
	EMUOP_REGISTER_ZONESERVERREPLY = 3,
	EMUOP_REQUEST_ZONE = 4,
	EMUOP_REQUEST_ZONE_REPLY = 5,
	EMUOP_TRANSFER_CLIENT = 6
};