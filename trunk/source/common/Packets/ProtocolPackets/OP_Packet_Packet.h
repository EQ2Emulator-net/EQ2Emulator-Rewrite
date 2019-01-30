#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"
#include "../EQ2Packet.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

class OP_Packet_Packet : public ProtocolPacket {
public:
	OP_Packet_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_Packet;
		HasCRC = true;
	}

	uint32_t Write(unsigned char*& writeBuffer) {
		uint16_t op = htons(opcode);
		memcpy(buffer, &op, 2);
		offset = 2;
		memcpy(buffer + offset, &sequence, 2);
		writeBuffer = buffer;

		return Size;
	}

	void Write(EQ2Packet* p) {
		Size = p->Size + 4; //  int16 for opcode,  int16 for crc
		
		if (buffer)
			delete[] buffer;

		buffer = new unsigned char[Size];
		offset = 2;
		memcpy(buffer + offset, p->buffer, p->Size);
	}

private:
	void RegisterElements() {

	}
};
