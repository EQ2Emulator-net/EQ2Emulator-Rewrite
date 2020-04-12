#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_RequestZoneReply_Packet : public EmuPacket {
public:
	Emu_RequestZoneReply_Packet() : EmuPacket(EmuOpcode_t::EMUOP_REQUEST_ZONE_REPLY) {
		RegisterElements();

		reply = 0;
	}
	~Emu_RequestZoneReply_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> z);

	uint32_t reply;

private:
	void RegisterElements() {
		RegisterUInt32(reply);
	}
};