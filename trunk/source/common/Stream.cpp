#include "stdafx.h"

#include "Stream.h"
#include "Server.h"
#include "util.h"
#include "log.h"
#include <sstream>

Stream::Stream(unsigned int ip, unsigned short port) {
	RemoteIP = ip;
	RemotePort = port;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = RemoteIP;
	address.sin_port = RemotePort;

	LastPacketTime = 0;
	ReceivedPackets = 0;
	SentPackets = 0;
	Sock = INVALID_SOCKET;
}

void Stream::Process(const unsigned char* buffer, unsigned int length) {
	ReceivedPackets++;

	DumpBytes(buffer, length);
}

void Stream::WritePacket(SOCKET socket, const unsigned char* buffer, int length) {
	SentPackets++;

	sendto(socket, reinterpret_cast<const char*>(buffer), length, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
}

std::string Stream::ToString() {
	uint32_t port = ntohs(GetPort());
	uint32_t ip = ntohl(GetIP());

	std::ostringstream ss;
	ss << ip << '.' << port;
	return ss.str();
}