#pragma once

#include <stdint.h>

class Server;

class Stream {
public:
	Stream(unsigned int ip, unsigned short port);

	virtual void Process(unsigned char* data, unsigned int length);
	void SetLastPacketTime(unsigned int time) { LastPacketTime = time; }
	void SetServer(Server* serv) { server = serv; }
	uint32_t GetIP() { return RemoteIP; }
	uint16_t GetPort() { return RemotePort; }

protected:
	void WritePacket(int socket, unsigned char* buffer, int length);


	uint32_t LastPacketTime;
	uint32_t ReceivedPackets;
	uint32_t SentPackets;
	Server* server;

private:
	uint32_t RemoteIP;
	uint16_t RemotePort;
	
};