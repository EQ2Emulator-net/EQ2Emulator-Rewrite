#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ClientLoginConfirm_Packet : public EmuPacket {
public:
	Emu_ClientLoginConfirm_Packet() : EmuPacket(EmuOpcode_t::EMUOP_CLIENT_LOGIN_CONFIRM) {
		RegisterElements();

		access_code = 0;
	}
	~Emu_ClientLoginConfirm_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t access_code;
	uint32_t sessionID;
	std::string zoneName;

private:
	void RegisterElements() {
		RegisterUInt32(access_code);
		RegisterUInt32(sessionID);
		Register8String(zoneName);
	}
};