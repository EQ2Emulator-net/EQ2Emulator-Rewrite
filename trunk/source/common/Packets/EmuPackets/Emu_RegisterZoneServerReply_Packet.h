#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_RegisterZoneServerReply_Packet : public EmuPacket {
public:
	Emu_RegisterZoneServerReply_Packet() : EmuPacket(EmuOpcode_t::EMUOP_REGISTER_ZONESERVERREPLY) {
		RegisterElements();
	}
	~Emu_RegisterZoneServerReply_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint8_t reply;
	std::string serverName;

private:
	void RegisterElements() {
		RegisterUInt8(reply);
		Register8String(serverName);
	}
};
