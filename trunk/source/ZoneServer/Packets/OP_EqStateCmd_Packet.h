#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"

class OP_EqStateCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqStateCmd_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version),
		spawnID(0xFFFFFFFF), stateID(0), unknown1(0), unknown2(0)
	{
		RegisterElements();
	}

	uint32_t spawnID;
	uint32_t stateID;
	uint8_t unknown1;
	uint8_t unknown2;

private:
	void RegisterElements() {
		const uint32_t ver = GetVersion();
		RegisterUInt32(spawnID);
		if (ver >= 57048) {
			RegisterUInt32(stateID);
		}
		else {
			RescopeToReference(stateID, uint16_t);
			RegisterUInt16(stateID);
		}
		if (ver > 546) {
			RegisterUInt8(unknown1);
			if (ver > 864) {
				RegisterUInt8(unknown2);
			}
		}
	}
};