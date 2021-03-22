#include "stdafx.h"

#include "PacketLog.h"
#include "../WorldServer/Packets/OP_LoginRequestMsg_Packet.h"

uint32_t PacketLog::ReadLoginRequest(const unsigned char* data, uint32_t size) {
	uint32_t offset = 1;
	if (data[0] == 0) {
		//Set the offset 1 byte past the opcode
		if (data[1] == 9) offset = 5;
		//Opcode 0 so to check for the "extra" 0 hope that the first element is set in the packet normally
		else if (data[1] == 0) offset = 2;
	}

	uint32_t pver = OP_LoginRequestMsg_Packet::DetermineStructVersion(data, offset, size);
	OP_LoginRequestMsg_Packet p(pver);
	p.Read(data, offset, size);
	return static_cast<uint32_t>(p.Version);
}