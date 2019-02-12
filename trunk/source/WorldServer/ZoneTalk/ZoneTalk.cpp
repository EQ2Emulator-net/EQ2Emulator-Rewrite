#include "stdafx.h"

#include "ZoneTalk.h"
#include "ZoneStream.h"
#include "../../common/EmuPacket.h"
#include "../../common/util.h"

ZoneTalk::ZoneTalk() : TCPServer(true) {

}

Stream* ZoneTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return new ZoneStream(ip, port);
}

bool ZoneTalk::Process() {
	ReadLocker lock(streamLock);
	for (auto& itr : Streams) {
		SOCKET sock = itr.first;
		auto s = static_cast<ZoneStream*>(itr.second);
		auto incoming = s->PopIncoming();

		for (auto& itr : incoming) {
			DumpBytes(itr->buffer, itr->Size);
		}

		auto outgoing = s->PopOutgoing();

		for (auto& itr : outgoing) {
			s->WritePacket(sock, itr->buffer, itr->Size);
		}
	}

	return true;
}