#include "stdafx.h"

#include "ZoneTalk.h"
#include "ZoneStream.h"
#include "../Packets/OP_PlayCharacterReplyMsg_Packet.h"

ZoneTalk::ZoneTalk() : TCPServer(true) {
	m_zones.SetName("m_zones");
}

Stream* ZoneTalk::GetNewStream(unsigned int ip, unsigned short port) {
	return new ZoneStream(ip, port);
}

bool ZoneTalk::Process() {
	ReadLocker lock(streamLock);
	for (auto& itr : Streams) {
		itr.second->Process();
	}

	return true;
}

void ZoneTalk::RegisterZoneServer(ZoneStream* stream, std::string ip, uint16_t port) {
	ZoneListeningInfo info;
	info.ip = ip;
	info.port = port;
	WriteLocker lock(m_zones);
	zones[stream->GetSocket()] = info;
}

void ZoneTalk::StreamDisconnected(Stream* stream) {
	WriteLocker lock(m_zones);
	zones.erase(stream->GetSocket());
}

bool ZoneTalk::GetAvailableZone(OP_PlayCharacterReplyMsg_Packet* p) {
	bool ret = false;
	ReadLocker lock(m_zones);

	//Do more checks here later... for now just grab the first avail
	if (!zones.empty()) {
		const ZoneListeningInfo& info = zones.begin()->second;
		
		p->server = info.ip;
		p->port = info.port;

		ret = true;
	}

	return ret;
}