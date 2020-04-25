#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ClientSessionEnded_Packet : public EmuPacket {
public:
	Emu_ClientSessionEnded_Packet() : EmuPacket(EmuOpcode_t::EMUOP_CLIENT_SESSION_ENDED) {
		RegisterElements();
		sessionID = 0;
	}

	~Emu_ClientSessionEnded_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t sessionID;

private:
	void RegisterElements() {
		RegisterUInt32(sessionID);
	}
};