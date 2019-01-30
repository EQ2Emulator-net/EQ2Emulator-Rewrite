#include "EQ2Packet.h"
#include "../util.h"
#include "../Packets/EQ2Packets/OpcodeManager.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Packet::EQ2Packet() {
	app_opcode_size = 2;
}

uint8_t EQ2Packet::PreparePacket(uint16_t MaxLen) {
	uint16_t OpcodeVersion = GetOpcodeVersion(Version);

	// stops a crash for incorrect version
	/*if (EQOpcodeManager.count(OpcodeVersion) == 0) {
		LogError(LOG_PACKET, 0, "Version %u is not listed in the opcodes table.", Version);
		return -1;
	}*/

	PacketPrepared = true;

	uint16_t login_opcode = 2;// EQOpcodeManager[OpcodeVersion]->EmuToEQ(login_op);
	uint8_t offset = 0;
	//one of the int16s is for the seq, other is for the EQ2 opcode and compressed flag (OP_Packet is the header, not the opcode)
	uint32_t new_size = Size + sizeof(uint16_t) + sizeof(uint8_t);
	bool oversized = false;
	if (login_opcode != 2) {
		new_size += sizeof(uint8_t); //for opcode
		if (login_opcode >= 255) {
			new_size += sizeof(uint16_t);
			oversized = true;
		}
		else
			login_opcode = ntohs(login_opcode);
	}
	unsigned char* new_buffer = new unsigned char[new_size];
	memset(new_buffer, 0, new_size);
	unsigned char* ptr = new_buffer + sizeof(uint16_t); // sequence is first
	if (login_opcode != 2) {
		if (oversized) {
			ptr += sizeof(uint8_t); //compressed flag
			uint8_t addon = 0xff;
			memcpy(ptr, &addon, sizeof(uint8_t));
			ptr += sizeof(uint8_t);
		}
		memcpy(ptr, &login_opcode, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
	}
	else {
		memcpy(ptr, &login_opcode, sizeof(uint8_t));
		ptr += sizeof(uint8_t);
	}
	memcpy(ptr, buffer, Size);
	if (buffer)
		delete[] buffer;
	buffer = new_buffer;
	offset = new_size - Size - 1;
	Size = new_size;
	return offset;
}

uint32_t EQ2Packet::serialize(unsigned char *dest) const {
	if (app_opcode_size == 1)
		*(unsigned char *)dest = (uint8_t)opcode;
	else
		*(uint16_t *)dest = opcode;

	memcpy(dest + app_opcode_size, buffer, Size);

	return Size + app_opcode_size;
}

extern OpcodeManager* g_OpcodeManager;

void EQ2Packet::FindOpcode() {
	g_OpcodeManager->SetOpcodeForPacket(this);
}