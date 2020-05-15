#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

// Probably remove these when the proper systems are in place
#include "../ZoneServer/Client.h"
#include "OP_EqSetPOVGhostCmd_Packet.h"
#include "OP_EqSetControlGhostCmd_Packet.h"
#include "OP_CreateGhostCmd_Packet.h"
#include "OP_UpdateCharacterSheetMsg_Packet.h"
#include "../Controllers/PlayerController.h"
#include "../Spawns/Entity.h"

#include "../ZoneServer/ZoneServer.h"

class OP_DoneLoadingEntityResourcesMsg_Packet : public EQ2Packet {
public:
	OP_DoneLoadingEntityResourcesMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();
	}

	void HandlePacket(std::shared_ptr<Client> client) {

		std::shared_ptr<ZoneServer> zone = client->GetZone();
		if (zone) {

			zone->SendCharacterInfo(client);

			auto controller = client->GetController();

			auto spawn = controller->GetControlled();
			uint32_t id = client->GetIDForSpawn(spawn);

			OP_EqSetPOVGhostCmd_Packet* pov = new OP_EqSetPOVGhostCmd_Packet(client->GetVersion());
			pov->spawn_id = id;
			client->QueuePacket(pov);

			OP_EqSetControlGhostCmd_Packet* control = new OP_EqSetControlGhostCmd_Packet(client->GetVersion());
			control->speed = 5;
			control->airSpeed = 5;
			control->swimmingSpeedMultiplier = 0.8f;
			control->spawn_id = 0xFFFFFFFF;
			client->QueuePacket(control);

			control = new OP_EqSetControlGhostCmd_Packet(client->GetVersion());
			control->speed = 5;
			control->airSpeed = 5;
			control->swimmingSpeedMultiplier = 0.8f;
			control->spawn_id = id;
			control->unknown2 = 0xFF;
			client->QueuePacket(control);

			OP_UpdateCharacterSheetMsg_Packet* char_sheet = new OP_UpdateCharacterSheetMsg_Packet(client->GetVersion(), *controller->GetCharacterSheet());
			char_sheet->SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateCharSheet));

			strcpy(char_sheet->bind_zone, "test zone");
	

			client->QueuePacket(char_sheet);
		}
	}

private:
	void RegisterElements() {

	}

};