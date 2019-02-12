#include <cstdint>

class EmuPacket;

namespace EmuOpcode {
	EmuPacket* GetPacketForOpcode(uint8_t op);
}

enum EmuOpcode_t : uint8_t {
	EMUOP_FORWARD_PACKET = 1
};