#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_ZoneTransferReply_Packet : public EmuPacket {
public:
	Emu_ZoneTransferReply_Packet() : EmuPacket(EmuOpcode_t::EMUOP_ZONE_TRANSFER_REPLY) {
		RegisterElements();

		access_code = 0;
		port = 0;
		sessionID = 0;
		bError = false;
	}
	~Emu_ZoneTransferReply_Packet() = default;

	void HandlePacket(std::shared_ptr<WorldStream> w);

	uint32_t sessionID;
	uint32_t access_code;
	uint16_t port;
	std::string host;
	bool bError;

private:
	void RegisterElements() {
		RegisterUInt32(sessionID);
		RegisterUInt32(access_code);
		RegisterUInt16(port);
		Register8String(host);
		RegisterBool(bError);
	}
};