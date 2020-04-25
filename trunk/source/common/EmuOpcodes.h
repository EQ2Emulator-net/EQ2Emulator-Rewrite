#pragma once

#include <cstdint>

class EmuPacket;

enum class EmuOpcode_t : uint8_t {
	EMUOP_FORWARD_PACKET = 1,
	EMUOP_REGISTER_ZONESERVER = 2,
	EMUOP_REGISTER_ZONESERVERREPLY = 3,
	EMUOP_REQUEST_ZONE = 4,
	EMUOP_REQUEST_ZONE_REPLY = 5,
	EMUOP_TRANSFER_CLIENT = 6,
	EMUOP_TRANSFER_CLIENT_CONFIRM = 7,
	EMUOP_CLIENT_LOGIN_CONFIRM = 8,
	EMUOP_CANCEL_CLIENT_TRANSFER = 9,
	EMUOP_CLIENT_SESSION_ENDED = 10
};

namespace EmuOpcode {
	EmuPacket* GetPacketForOpcode(EmuOpcode_t op);
}

