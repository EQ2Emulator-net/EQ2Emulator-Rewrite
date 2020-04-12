#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_RequestZone_Packet : public EmuPacket {
public:
	Emu_RequestZone_Packet() : EmuPacket(EmuOpcode_t::EMUOP_REQUEST_ZONE) {
		RegisterElements();

		zone_id = 0;
		instance_id = 0;
	}
	~Emu_RequestZone_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t zone_id;
	uint32_t instance_id;

private:
	void RegisterElements() {
		RegisterUInt32(zone_id);
		RegisterUInt32(instance_id);
	}
};