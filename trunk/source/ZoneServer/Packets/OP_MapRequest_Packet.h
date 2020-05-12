#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"
#include "../ZoneServer/MasterZoneLookup.h"
#include "OP_MapFogDataInitMsg_Packet.h"

extern MasterZoneLookup g_masterZoneLookup;

class OP_MapRequest_Packet : public EQ2Packet {
public:
	OP_MapRequest_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	std::string zoneFile;
	uint8_t unknown;

	void RegisterElements() {
		Register16String(zoneFile);
		RegisterUInt8(unknown);
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		auto zone = client->GetZone();
		if (!zone) {
			return;
		}

		auto zi = g_masterZoneLookup.GetZoneInfoByFileName(zoneFile);
		if (!zi || !zi->mapData) {
			return;
		}

		OP_MapFogDataInitMsg_Packet p(client->GetVersion());
		p.InsertMapData(*zi->mapData);
		p.unknown1 = 1;
		p.unknown3 = 1;
		client->QueuePacket(p);
	}
};