#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ItemIDRequest_Packet : public EmuPacket {
public:
	Emu_ItemIDRequest_Packet() : EmuPacket(EmuOpcode_t::EMUOP_ITEM_ID_REQUEST) {
	}

	~Emu_ItemIDRequest_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);
};