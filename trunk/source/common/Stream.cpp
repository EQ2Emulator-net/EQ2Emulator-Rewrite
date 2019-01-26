#include "Stream.h"

#include "util.h"
#include "log.h"



Stream::Stream(unsigned int ip, unsigned short port) {
	RemoteIP = ip;
	RemotePort = port;
}

void Stream::Process(unsigned char* buffer, unsigned int length) {
	ReceivedPackets++;

	DumpBytes(buffer, length);
}