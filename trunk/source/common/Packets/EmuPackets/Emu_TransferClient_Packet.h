#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_TransferClient_Packet : public EmuPacket {
public:
	Emu_TransferClient_Packet() : EmuPacket(EmuOpcode_t::EMUOP_TRANSFER_CLIENT) {
		RegisterElements();

		account_id = 0;
		access_code = 0;
		character_id = 0;
		zone_id = 0;
		instance_id = 0;
	}
	~Emu_TransferClient_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t account_id;
	uint32_t access_code;
	uint32_t character_id;
	uint32_t zone_id;
	uint32_t instance_id;

private:
	void RegisterElements() {
		RegisterUInt32(account_id);
		RegisterUInt32(access_code);
		RegisterUInt32(character_id);
		RegisterUInt32(zone_id);
		RegisterUInt32(instance_id);
	}
};