#pragma once

#include <stdint.h>
#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include "sys/socket.h"
#endif

class Server;

class Stream {
public:
	Stream(unsigned int ip, unsigned short port);
	virtual ~Stream() = default;

	virtual void Process(const unsigned char* data, unsigned int length);
	virtual void Process(){};
	virtual bool RequestNewClient() { return false; }
	virtual void ConnectionTimeout() {};
	void SetServer(Server* serv) { server = serv; }
	uint32_t GetIP() { return RemoteIP; }
	std::string GetIPString();
	uint16_t GetPort() { return RemotePort; }
	void SetSocket(SOCKET s) { Sock = s; }
	void WritePacket(SOCKET socket, const unsigned char* buffer, int length);
	void UpdateHeartbeat(uint32_t time);
	uint32_t CheckHeartbeatDelta(uint32_t time);

	std::string ToString();
	SOCKET GetSocket() { return Sock; }

	uint64_t GetConnectionKey();

protected:
	uint32_t heartbeat;
	uint32_t ReceivedPackets;
	uint32_t SentPackets;
	Server* server;
	SOCKET Sock;

private:
	uint32_t RemoteIP;
	uint16_t RemotePort;
	sockaddr_in address;
};