#include "stdafx.h"

#include "Client.h"
#include "../Packets/OP_LoginByNumRequestMsg_Packet.h"

void Client::ReadVersionPacket(unsigned char* data, uint32_t size, uint32_t offset, uint16_t opcode) {
	//Choose the struct version depending on the size of the data
	OP_LoginByNumRequestMsg_Packet p(size - offset > 34 ? 1212 : 1);
	p.Read(data, offset, size);
	
	SetVersion(p.version);
}