#include "stdafx.h"

#include "ZoneStream.h"
#include "../../common/util.h"
#include "../../common/EmuPacket.h"
#include "ZoneTalk.h"

void ZoneStream::Process() {
	auto incoming = PopIncoming();

	for (auto& itr : incoming) {
		LogDebug(LOG_PACKET, 0, "ZoneStream::incoming dump");
		DumpBytes(itr->buffer, itr->Size);
		itr->HandlePacket(this);
	}

	auto outgoing = PopOutgoing();

	for (auto& itr : outgoing) {
		WritePacket(Sock, itr->buffer, itr->Size);
	}
}

ZoneTalk* ZoneStream::GetServer() {
	return static_cast<ZoneTalk*>(server);
}