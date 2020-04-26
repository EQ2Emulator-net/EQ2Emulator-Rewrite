#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_CampStartedMsg_Packet : public EQ2Packet {
public:
	OP_CampStartedMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
		numSeconds = 0;
		bCampDesktop = false;
		bCampCharSelect = false;
		unkBool = false;
		unkBool2 = false;
	}

	uint8_t numSeconds;
	bool bCampDesktop;
	bool bCampCharSelect;
	bool unkBool;
	std::string charName;
	//guess, another string
	std::string serverName;
	uint32_t unknown;
	bool unkBool2;

private:
	void RegisterElements() {
		RegisterUInt8(numSeconds);
		RegisterBool(bCampDesktop);

		if (GetVersion() <= 283) {
			return;
		}

		RegisterBool(bCampCharSelect);
		Register16String(charName);
		RegisterBool(unkBool);
		Register16String(serverName);

		if (GetVersion() >= 1153) {
			RegisterUInt32(unknown);
			RegisterBool(unkBool2);
		}
	}
};
