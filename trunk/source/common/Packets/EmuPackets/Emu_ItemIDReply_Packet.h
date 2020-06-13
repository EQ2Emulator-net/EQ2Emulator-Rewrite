#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ItemIDReply_Packet : public EmuPacket {
public:
	Emu_ItemIDReply_Packet() : EmuPacket(EmuOpcode_t::EMUOP_ITEM_ID_REPLY), rangeLow(0), rangeHigh(0) {
		RegisterElements();
	}
	~Emu_ItemIDReply_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t rangeLow;
	uint32_t rangeHigh;

private:
	void RegisterElements() {
		RegisterUInt32(rangeLow);
		RegisterUInt32(rangeHigh);
	}
};