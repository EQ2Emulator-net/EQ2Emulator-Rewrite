#include "EQ2Stream.h"
#include "log.h"
#include "Packets/ProtocolPacket.h"
#include "Packets/ProtocolPackets/OP_SessionRequest_Packet.h"
#include "Packets//ProtocolPackets/OP_SessionResponse_Packet.h"
#include "CRC16.h"
#include "util.h"

// Remove when done testing
#include"Server.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

EQ2Stream::EQ2Stream(unsigned int ip, unsigned short port) : Stream(ip, port) {
	Key = 0;
	Session = 0;
	MaxLength = 0;
	NextInSeq = 0;
	NextOutSeq = 0;
	MaxAckReceived = -1;
	NextAckToSend = -1;
	LastAckSent = -1;
	LastSeqSent = -1;
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
		p.Read(data, offset, length);
		
		Session = ntohl(p.Session);
		MaxLength = ntohl(p.MaxLength);
		NextInSeq = 0;
		Key = 0x33624702;
		LogDebug(LOG_NET, 0, "OP_SessionRequest unknowna: %u, Session: %u, MaxLength: %u", p.UnknownA, p.Session, p.MaxLength);

		SendSessionResponse();
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
		uint16_t comp_crc = (uint16_t)CRC16(buffer, length - 2, key);
		uint16_t packet_crc = ntohs(*(uint16_t*)(buffer + length - 2));

		valid = (!packet_crc || comp_crc == packet_crc);
	}

	return valid;
}

void EQ2Stream::SendSessionResponse() {
	OP_SessionResponse_Packet Response;
	Response.Session = htonl(Session);
	Response.MaxLength = htonl(MaxLength);
	Response.UnknownA = 2;
	Response.Format = 0;

	/*if (compressed)
		Response->Format |= FLAG_COMPRESSED;
	if (encoded)
		Response->Format |= FLAG_ENCODED;*/

	Response.Key = htonl(Key);


	// Every thing below is temporary, just to test sending a packet out
	uint16_t op = htons(Response.GetOpcode());
	unsigned char* temp_buffer = nullptr;
	uint32_t size = Response.Write(temp_buffer);
	unsigned char* buffer = new unsigned char[size + 2];
	memcpy(buffer, &op, 2);
	memcpy(buffer + 2, temp_buffer, size);
	WritePacket(Server::Sock, buffer, size + 2);

	DumpBytes(buffer, size + 2);
}