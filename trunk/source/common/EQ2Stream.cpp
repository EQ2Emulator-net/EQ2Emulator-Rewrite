#include "EQ2Stream.h"
#include "log.h"
#include "Packets/ProtocolPacket.h"
#include "Packets/ProtocolPackets/OP_SessionRequest_Packet.h"
#include "CRC16.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Stream::EQ2Stream(unsigned int ip, unsigned short port) : Stream(ip, port) {

}

void EQ2Stream::Process(unsigned char* data, unsigned int length) {
	Stream::Process(data, length);
	// TODO: Validate crc and decompress or decode

	ProcessPacket(data, length);
}

void EQ2Stream::ProcessPacket(unsigned char* data, unsigned int length) {
	uint32_t offset = 2;

	uint16_t opcode = ntohs(*(uint16_t*)data);

	LogDebug(LOG_NET, 0, "ProtocolPacket Received, opcode: %u", opcode);
	switch (opcode) {
	case OP_SessionRequest: {
		OP_SessionRequest_Packet p;
		p.Read(data, offset);

		LogDebug(LOG_NET, 0, "OP_SessionRequest unknowna: %u, Session: %u, MaxLength: %u", p.UnknownA, p.Session, p.MaxLength);
		break;
	}
	default:
		break;
	}
}

bool EQ2Stream::HandlePacket() {
	LogError(LOG_NET, 0, "HandlePacket should be overriden");
	return false;
}

bool EQ2Stream::ValidateCRC(unsigned char* buffer, uint16_t length, uint32_t key) {
	bool valid = false;
	if (buffer[0] == 0x00 && (buffer[1] == OP_SessionRequest || buffer[1] == OP_SessionResponse || buffer[1] == OP_OutOfSession))
		valid = true;
	else if (buffer[2] == 0x00 && buffer[3] == 0x19)
		valid = true;
	else {
		uint16_t comp_crc = CRC16(buffer, length - 2, key);
		uint16_t packet_crc = ntohs(*(uint16_t*)(buffer + length - 2));

		valid = (!packet_crc || comp_crc == packet_crc);
	}

	return valid;
}