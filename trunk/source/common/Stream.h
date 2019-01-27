#pragma once

#include <stdint.h>

class Stream {
public:
	Stream(unsigned int ip, unsigned short port);

	virtual void Process(unsigned char* data, unsigned int length);
	void SetLastPacketTime(unsigned int time) { LastPacketTime = time; }

protected:
	void WritePacket(int socket, unsigned char* buffer, int length);


	int32_t LastPacketTime;
	int32_t ReceivedPackets;
	int32_t SentPackets;

private:
	int32_t RemoteIP;
	int16_t RemotePort;

};