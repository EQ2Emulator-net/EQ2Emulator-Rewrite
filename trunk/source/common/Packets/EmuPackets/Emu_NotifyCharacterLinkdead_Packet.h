#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_NotifyCharacterLinkdead_Packet : public EmuPacket {
public:
	Emu_NotifyCharacterLinkdead_Packet() : EmuPacket(EmuOpcode_t::EMUOP_NOTIFY_CHARACTER_LINKDEAD) {
		RegisterElements();
		characterID = 0;
	}
	~Emu_NotifyCharacterLinkdead_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t characterID;

private:
	void RegisterElements() {
		RegisterUInt32(characterID);
	}
};