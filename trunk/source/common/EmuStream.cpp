#include "stdafx.h"

#include "EmuStream.h"
#include "util.h"
#include "EmuPacket.h"
#include "EmuOpcodes.h"

EmuStream::EmuStream(unsigned int ip, unsigned int port)
:Stream(ip, port) {
}

EmuStream::~EmuStream() {
}

void EmuStream::Process(const unsigned char* data, unsigned int length) {
	EmuPacket* p = EmuOpcode::GetPacketForOpcode(static_cast<EmuOpcode_t>(data[0]));
	if (!p) {
		return;
	}

	if (!p->Read(data, length)) {
		return;
	}

	SpinLocker lock(m_incoming);
	incoming.emplace_back(p);
}

void EmuStream::QueuePacket(EmuPacket* p) {
	unsigned char* buf = nullptr;
	p->Write(buf);
	
	SpinLocker lock(m_outgoing);
	outgoing.emplace_back(p);
}

std::deque<std::unique_ptr<EmuPacket> > EmuStream::PopOutgoing() {
	decltype(outgoing) ret;
	SpinLocker lock(m_outgoing);
	std::swap(ret, outgoing);
	return ret;
}

std::deque<std::unique_ptr<EmuPacket> > EmuStream::PopIncoming() {
	decltype(incoming) ret;
	SpinLocker lock(m_incoming);
	std::swap(ret, incoming);
	return ret;
}