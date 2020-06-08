#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ItemIDReply_Packet : public EmuPacket {
public:
	Emu_ItemIDReply_Packet() : EmuPacket(EmuOpcode_t::EMUOP_ITEM_ID_REPLY) {
		RegisterElements();
	}
	~Emu_ItemIDReply_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t requestTag;
	uint32_t itemID;

private:
	void RegisterElements() {
		RegisterUInt32(requestTag);
		RegisterUInt32(itemID);
	}
};