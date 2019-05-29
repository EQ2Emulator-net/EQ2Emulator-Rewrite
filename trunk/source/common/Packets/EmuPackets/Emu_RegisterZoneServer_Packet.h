#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_RegisterZoneServer_Packet : public EmuPacket {
public:
	Emu_RegisterZoneServer_Packet() : EmuPacket(EMUOP_REGISTER_ZONESERVER) {
		RegisterElements();
	}
	~Emu_RegisterZoneServer_Packet() = default;

	void HandlePacket(ZoneStream* z);

	std::string ip;
	uint16_t port;

private:
	void RegisterElements() {
		Register8String(ip);
		RegisterUInt16(port);
	}
};