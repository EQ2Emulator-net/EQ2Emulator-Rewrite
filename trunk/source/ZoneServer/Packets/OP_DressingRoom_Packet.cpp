#include "stdafx.h"

#include "OP_DressingRoom_Packet.h"

EQ2Packet* OP_DressingRoom_Packet::GetSubPacket() {
	if (type == 4 || (type == 3 && appearance_type == 0xFFFFFFFF)) {
		return new DressingRoom_NetAppearance_Packet(GetVersion());
	}

	return new DressingRoom_Standard_Packet(GetVersion());
}