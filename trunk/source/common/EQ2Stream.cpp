#include "EQ2Stream.h"
#include "log.h"
#include "Packets/ProtocolPackets/ProtocolPackets.h"
#include "CRC16.h"
#include "util.h"
#include "Server.h"

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
	Compressed = false;
	Encoded = false;

	crypto.setRC4Key(0);
}

void EQ2Stream::Process(unsigned char* data, unsigned int length) {
	Stream::Process(data, length);
	// TODO: Validate crc and decompress or decode

	ProtocolPacket* p = ProtocolPacket::GetProtocolPacket(data, length);
	if (p) {
		ProcessPacket(p);
		delete p;
	}
}

void EQ2Stream::ProcessPacket(ProtocolPacket* p) {
	LogDebug(LOG_NET, 0, "ProtocolPacket Received, opcode: %u", p->GetOpcode());

	switch (p->GetOpcode()) {
	case OP_SessionRequest: {
		OP_SessionRequest_Packet* request = (OP_SessionRequest_Packet*)p;
		
		Session = ntohl(request->Session);
		MaxLength = ntohl(request->MaxLength);
		NextInSeq = 0;
		Key = 0x33624702;
		LogDebug(LOG_NET, 0, "OP_SessionRequest unknowna: %u, Session: %u, MaxLength: %u", request->UnknownA, request->Session, request->MaxLength);

		SendSessionResponse();
		break;
	}
	case OP_SessionDisconnect: {
		OP_SessionDisconnect_Packet* disconnect = (OP_SessionDisconnect_Packet*)p;
		SendDisconnect(disconnect->Reason);
		break;
	}
	case OP_KeepAlive: {
		SendKeepAlive();
		break;
	}
	case OP_ClientSessionUpdate: {
		OP_ClientSessionUpdate_Packet* update = (OP_ClientSessionUpdate_Packet*)p;
		//AdjustRates(ntohl(Stats->average_delta));
		SendServerSessionUpdate(update->RequestID);
		/*if(!crypto->isEncrypted())
			SendKeyRequest();
		*/
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

void EQ2Stream::WritePacket(ProtocolPacket* p) {
	unsigned char* buffer = nullptr;
	uint32_t size = p->Write(buffer);
	if (p->GetOpcode() != OP_SessionRequest && p->GetOpcode() != OP_SessionResponse) {
		if (Compressed) {

		}

		if (Encoded) {

		}

		*(uint16_t*)(buffer + (size - 2)) = htons((uint16_t)CRC16(buffer, size - 2, Key));
	}

	// The dump is for debugging, remove when this all works
	DumpBytes(buffer, size);
	Stream::WritePacket(server->GetSocket(), buffer, size);
}

void EQ2Stream::SendSessionResponse() {
	OP_SessionResponse_Packet Response;
	Response.Session = htonl(Session);
	Response.MaxLength = htonl(MaxLength);
	Response.UnknownA = 2;
	Response.Format = 0;

	if (Compressed)
		Response.Format |= FLAG_COMPRESSED;
	if (Encoded)
		Response.Format |= FLAG_ENCODED;

	Response.Key = htonl(Key);


	// Every thing below is temporary, just to test sending a packet out
	WritePacket(&Response);
	
}

void EQ2Stream::SendDisconnect(uint16_t reason) {
	OP_SessionDisconnect_Packet disconnect;
	disconnect.Session = htonl(Session);
	disconnect.Reason = reason;

	// Every thing below is temporary, just to test sending a packet out
	WritePacket(&disconnect);
	server->StreamDisconnected(this); // this deletes the stream (client)
}

void EQ2Stream::SendKeepAlive() {
	OP_KeepAlive_Packet keepAlive;

	// Every thing below is temporary, just to test sending a packet out
	WritePacket(&keepAlive);
}

void EQ2Stream::SendServerSessionUpdate(uint16_t requestID) {
	OP_ServerSessionUpdate_Packet update;

	uint32_t sent = ntohl(SentPackets);
	uint32_t received = ntohl(ReceivedPackets);

	update.RequestID = requestID;
	update.CurrentTime = 0; // Update to timer when added
	update.SentPackets = sent;
	update.SentPackets2 = sent;
	update.ReceivedPackets = received;
	update.ReceivedPackets2 = received;

	// Every thing below is temporary, just to test sending a packet out
	WritePacket(&update);
}