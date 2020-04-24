#include "stdafx.h"

#include "WorldStream.h"
#include "../../common/EmuPacket.h"
#include "../../common/util.h"
#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"

#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator g_zoneOperator;

WorldStream::WorldStream(unsigned int ip, unsigned int port) : EmuStream(ip, port) {
	authentication = EAuthentication::ENonAuth;
}

void WorldStream::Process() {
	auto incoming = PopIncoming();

	for (auto& itr : incoming) {
		if (NetDebugEnabled()) {
			LogDebug(LOG_PACKET, 0, "WorldStream::incoming dump");
			DumpBytes(itr->buffer, itr->Size);
		}
		itr->HandlePacket(shared_from_this());
	}

	if (authentication == EAuthentication::ENonAuth) {
		SetAuthentication(EAuthentication::EAuthenticating);

		Emu_RegisterZoneServer_Packet* p = new Emu_RegisterZoneServer_Packet();	
		p->ip = g_zoneOperator.GetHostString();
		p->port = g_zoneOperator.GetPort();

		QueuePacket(p);
	}

	auto outgoing = PopOutgoing();

	for (auto& itr : outgoing) {
		WritePacket(Sock, itr->buffer, itr->Size);
	}
}