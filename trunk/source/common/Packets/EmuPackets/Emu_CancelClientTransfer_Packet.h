#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_CancelClientTransfer_Packet : public EmuPacket {
public:
	Emu_CancelClientTransfer_Packet() : EmuPacket(EmuOpcode_t::EMUOP_CANCEL_CLIENT_TRANSFER) {
		RegisterElements();

		access_code = 0;
	}
	~Emu_CancelClientTransfer_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t access_code;

private:
	void RegisterElements() {
		RegisterUInt32(access_code);
	}
};