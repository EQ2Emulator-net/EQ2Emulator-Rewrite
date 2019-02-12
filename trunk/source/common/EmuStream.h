#pragma once

#include "Stream.h"
#include <deque>
#include "Mutex.h"
#include <memory>

class EmuPacket;

class EmuStream : public Stream {
public:
	~EmuStream();

	void Process(const unsigned char* data, unsigned int length) override;
	
	void QueuePacket(EmuPacket* p);

	std::deque<std::unique_ptr<EmuPacket> > PopIncoming();
	std::deque<std::unique_ptr<EmuPacket> > PopOutgoing();

protected:
	EmuStream(unsigned int ip, unsigned int port);

	std::deque<std::unique_ptr<EmuPacket> > incoming;
	std::deque<std::unique_ptr<EmuPacket> > outgoing;

	SpinLock m_incoming;
	SpinLock m_outgoing;
};