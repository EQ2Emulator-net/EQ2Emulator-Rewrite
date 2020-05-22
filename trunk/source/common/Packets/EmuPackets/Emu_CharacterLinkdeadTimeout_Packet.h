#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_CharacterLinkdeadTimeout_Packet : public EmuPacket {
public:
	Emu_CharacterLinkdeadTimeout_Packet() : EmuPacket(EmuOpcode_t::EMUOP_CHARACTER_LINKDEAD_TIMEOUT) {
		RegisterElements();
		characterID = 0;
	}
	~Emu_CharacterLinkdeadTimeout_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t characterID;

private:
	void RegisterElements() {
		RegisterUInt32(characterID);
	}
};