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

	OP_Packet_Packet(const unsigned char* buf, uint32_t len)
		: ProtocolPacket(buf, len) {
		RegisterElements();

		opcode = OP_Packet;
		HasCRC = true;
	}

	OP_Packet_Packet(uint16_t op, const unsigned char* buf, uint32_t len)
		: ProtocolPacket(buf, len) {
		opcode = op;
		HasCRC = true;
	}

	OP_Packet_Packet(OP_Packet_Packet&& rhs) : ProtocolPacket(std::move(rhs)) {

	}

	uint32_t Write(unsigned char*& writeBuffer) {
		uint16_t op = htons(opcode);
		memcpy(buffer, &op, 2);
		offset = 2;
		uint16_t seq = htons(sequence);
		memcpy(buffer + offset, &seq, 2);
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

	OP_Packet_Packet* MoveCopy() {
		return new OP_Packet_Packet(std::move(*this));
	}

private:
	void RegisterElements() {

	}
};
