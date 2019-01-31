#include "Stream.h"
#include "Server.h"
#include "util.h"
#include "log.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

Stream::Stream(unsigned int ip, unsigned short port) {
	RemoteIP = ip;
	RemotePort = port;

	LastPacketTime = 0;
	ReceivedPackets = 0;
	SentPackets = 0;
}

void Stream::Process(const unsigned char* buffer, unsigned int length) {
	ReceivedPackets++;

	DumpBytes(buffer, length);
}

void Stream::WritePacket(int socket, unsigned char* buffer, int length) {
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = RemoteIP;
	address.sin_port = RemotePort;


	SentPackets++;
	sendto(socket, (char*)buffer, length, 0, (sockaddr*)&address, sizeof(address));
}