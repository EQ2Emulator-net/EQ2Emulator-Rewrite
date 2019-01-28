#pragma once

#include <stdint.h>

class Stream {
public:
	Stream(unsigned int ip, unsigned short port);

	virtual void Process(unsigned char* data, unsigned int length);
	void SetLastPacketTime(unsigned int time) { LastPacketTime = time; }

protected:
	void WritePacket(int socket, unsigned char* buffer, int length);


	uint32_t LastPacketTime;
	uint32_t ReceivedPackets;
	uint32_t SentPackets;

private:
	uint32_t RemoteIP;
	uint16_t RemotePort;

};