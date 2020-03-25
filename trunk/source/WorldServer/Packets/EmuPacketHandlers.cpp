#include "stdafx.h"
#include "../ZoneTalk/ZoneStream.h"
#include "../ZoneTalk/ZoneTalk.h"

#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServerReply_Packet.h"

#include "../../common/Packets/EmuPackets/Emu_RequestZoneReply_Packet.h"

extern ZoneTalk zoneTalk;

void Emu_RegisterZoneServer_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	// TODO: Actually do checks instead of just accepting the ZoneServer
	Emu_RegisterZoneServerReply_Packet* p = new Emu_RegisterZoneServerReply_Packet();
	p->reply = 1;
	z->QueuePacket(p);

	// Set the ZoneServer info
	z->SetIP(ip);
	z->SetPort(port);
	LogDebug(LOG_NET, 0, "New zone server registed.  ip = %s:%u", ip.c_str(), port);
}

void Emu_RequestZoneReply_Packet::HandlePacket(std::shared_ptr<ZoneStream> z) {
	if (reply > 0) {
		zoneTalk.AddZone(z, reply, 0);
	}
	else {
		// error
	}
}