#include "stdafx.h"
#include "../ZoneTalk/ZoneStream.h"
#include "../ZoneTalk/ZoneTalk.h"

#include "../../common/Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"

void Emu_RegisterZoneServer_Packet::HandlePacket(ZoneStream* z) {
	ZoneTalk* serv = z->GetServer();
	serv->RegisterZoneServer(z, ip, port);
}