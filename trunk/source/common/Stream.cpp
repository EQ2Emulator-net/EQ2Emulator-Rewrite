#include "stdafx.h"

#include "Stream.h"
#include "Server.h"
#include "util.h"
#include "log.h"
#include <sstream>
#include "NetUtil.h"

Stream::Stream(unsigned int ip, unsigned short port) {
	RemoteIP = ip;
	RemotePort = port;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = RemoteIP;
	address.sin_port = RemotePort;

	heartbeat = 0;
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

	if (sendto(socket, reinterpret_cast<const char*>(buffer), length, 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
		LogError(LOG_NET, 0, "sendto error: %s", NetUtil::SocketError().c_str());
	}
}

std::string Stream::ToString() {
	uint32_t port = ntohs(GetPort());
	std::string ip = GetIPString();

	std::ostringstream ss;
	ss << ip << ':' << port;
	return ss.str();
}

uint64_t Stream::GetConnectionKey() {
	uint64_t ret = GetIP();
	ret <<= 16;
	return ret | GetPort();
}

void Stream::UpdateHeartbeat(uint32_t time) {
	heartbeat = time;
}

uint32_t Stream::CheckHeartbeatDelta(uint32_t time) {
	return time - heartbeat;
}

std::string Stream::GetIPString() {
	char buf[64];
	inet_ntop(address.sin_family, &address.sin_addr, buf, sizeof(buf));
	return std::string(buf); 
}