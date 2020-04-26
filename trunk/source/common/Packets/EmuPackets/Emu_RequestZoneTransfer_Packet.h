#pragma once

#include "../../EmuPacket.h"
#include "../../EmuOpcodes.h"
#include "../PacketElements/PacketElements.h"

class Emu_RequestZoneTransfer_Packet : public EmuPacket {
public:
	Emu_RequestZoneTransfer_Packet() : EmuPacket(EmuOpcode_t::EMUOP_REQUEST_ZONE_TRANSFER) {
		RegisterElements();

		zoneID = 0;
		instanceID = 0;
		sessionID = 0;
	}
	~Emu_RequestZoneTransfer_Packet() = default;

	void HandlePacket(std::shared_ptr<ZoneStream> w);

	uint32_t zoneID;
	uint32_t instanceID;
	uint32_t sessionID;
	uint32_t characterID;
	uint32_t accountID;

private:
	void RegisterElements() {
		RegisterUInt32(zoneID);
		RegisterUInt32(instanceID);
		RegisterUInt32(sessionID);
		RegisterUInt32(characterID);
		RegisterUInt32(accountID);
	}
};