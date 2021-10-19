#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"


class OP_TeleportWithinZoneNoReloadMsg_Packet : public EQ2Packet {
public:
	OP_TeleportWithinZoneNoReloadMsg_Packet(uint32_t version) : EQ2Packet(version),
	x(0.f), y(0.f), z(0.f), location(0xFFFFFFFF), heading(0.f), bForceRender(true) {
		RegisterElements();
	}

	float x;
	float y;
	float z;
	float heading;
	uint32_t location;
	bool bForceRender;

	void PreWrite() override {
		//This packet seems to want heading on a -180 to 180 scale, flipped if I just send it normal
		heading -= 180.f;
	}

	void PostWrite() override {
		heading += 180.f;
	}

private:
	void RegisterElements() {
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
		RegisterFloat(heading);
		RegisterBool(bForceRender);
		if (GetVersion() >= 869) {
			RegisterUInt32(location);
		}
	}
};