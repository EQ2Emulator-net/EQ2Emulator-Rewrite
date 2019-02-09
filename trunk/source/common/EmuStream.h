#pragma once

#include "Stream.h"
#include <queue>

class EmuPacket;

class EmuStream : public Stream {
public:
	~EmuStream();

	void Process(const unsigned char* data, unsigned int length) override;
	void Process() override;
	
	void QueuePacket(const unsigned char* data, uint32_t length);

protected:
	EmuStream(unsigned int ip, unsigned int port) : Stream(ip, port) {}

	std::queue<EmuPacket*> incoming;
	std::queue<EmuPacket*> outgoing;
};