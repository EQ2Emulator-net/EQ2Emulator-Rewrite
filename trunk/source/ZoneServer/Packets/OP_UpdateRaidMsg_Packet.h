#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../../common/Packets/EQ2Packet.h"
#include "Substruct_GroupMember.h"

class OP_UpdateRaidMsg_Packet : public EQ2Packet {
public:
	OP_UpdateRaidMsg_Packet(uint32_t ver) : EQ2Packet(ver), raidSheet(ver), packedData(ver <= 283) {
		packedData.LinkSubstruct(raidSheet, "raidSheet");
		RegisterElements();
	}

	void RegisterElements() {
		RegisterSubstruct(packedData);
	}

	PacketPackedData packedData;
	Substruct_RaidSheet raidSheet;
};
