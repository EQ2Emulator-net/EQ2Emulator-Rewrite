#include "stdafx.h"

#include "ZoneStream.h"
#include "../../common/util.h"
#include "../../common/EmuPacket.h"

void ZoneStream::Process() {
	auto incoming = PopIncoming();

	for (auto& itr : incoming) {
		DumpBytes(itr->buffer, itr->Size);
	}

	auto outgoing = PopOutgoing();

	for (auto& itr : outgoing) {
		WritePacket(Sock, itr->buffer, itr->Size);
	}
}