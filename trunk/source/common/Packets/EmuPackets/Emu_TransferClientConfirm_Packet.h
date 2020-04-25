#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_TransferClientConfirm_Packet : public EmuPacket {
public:
	Emu_TransferClientConfirm_Packet() : EmuPacket(EmuOpcode_t::EMUOP_TRANSFER_CLIENT_CONFIRM) {
		RegisterElements();

		access_code = 0;
	}
	~Emu_TransferClientConfirm_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t access_code;
	uint32_t characterID;

private:
	void RegisterElements() {
		RegisterUInt32(access_code);
		RegisterUInt32(characterID);
	}
};