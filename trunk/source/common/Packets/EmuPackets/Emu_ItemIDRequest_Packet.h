#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ItemIDRequest_Packet : public EmuPacket {
public:
	Emu_ItemIDRequest_Packet() : EmuPacket(EmuOpcode_t::EMUOP_ITEM_ID_REQUEST) {
		RegisterElements();

#ifdef EQ2_ZONE
		SetRequestTag();
#endif
	}

	~Emu_ItemIDRequest_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t requestTag;

private:
	void RegisterElements() {
		RegisterUInt32(requestTag);
	}

#ifdef EQ2_ZONE
	void SetRequestTag() {
		static std::atomic<uint32_t> g_tag(0);
		requestTag = g_tag.fetch_add(1);
	}
#endif
};