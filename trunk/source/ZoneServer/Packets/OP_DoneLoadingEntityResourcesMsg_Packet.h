#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

// Probably remove these when the proper systems are in place
#include "../ZoneServer/Client.h"
#include "OP_EqSetPOVGhostCmd_Packet.h"
#include "OP_EqSetControlGhostCmd_Packet.h"
#include "OP_CreateGhostCmd_Packet.h"
#include "OP_UpdateCharacterSheetMsg_Packet.h"

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
			/*OP_CreateGhostCmd_Packet* ghost = new OP_CreateGhostCmd_Packet(client->GetVersion());
			ghost->SetTestData();
			ghost->SetEncodedBuffers(client, ghost->header.index);
			client->QueuePacket(ghost);*/

			OP_EqSetPOVGhostCmd_Packet* pov = new OP_EqSetPOVGhostCmd_Packet(client->GetVersion());
			pov->spawn_id = 1;
			client->QueuePacket(pov);

			OP_EqSetControlGhostCmd_Packet* control = new OP_EqSetControlGhostCmd_Packet(client->GetVersion());
			control->speed = 5;
			control->air_speed = 5;
			control->size = 0.8f;
			control->spawn_id = 0xFFFFFFFF;
			client->QueuePacket(control);

			control = new OP_EqSetControlGhostCmd_Packet(client->GetVersion());
			control->speed = 5;
			control->air_speed = 5;
			control->size = 0.8f;
			control->spawn_id = 1;
			control->unknown2 = 0xFF;
			client->QueuePacket(control);

			OP_UpdateCharacterSheetMsg_Packet* char_sheet = new OP_UpdateCharacterSheetMsg_Packet(client->GetVersion());
			char_sheet->SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateCharSheet));
			strcpy(char_sheet->character_name, "Foof");
			strcpy(char_sheet->character_name2, "Foof");
			strcpy(char_sheet->character_name3, "Foof");
			char_sheet->level = 1;
			char_sheet->effective_level = 1;
			char_sheet->flags = 871628879;
			char_sheet->flags2 = 3219560;
			char_sheet->base_hp = 100;
			char_sheet->base_hp2 = 100;
			char_sheet->current_hp = 100;
			char_sheet->max_hp = 100;
			char_sheet->base_power = 50;
			char_sheet->current_power = 50;
			char_sheet->max_power = 50;
			client->QueuePacket(char_sheet);
		}
	}

private:
	void RegisterElements() {

	}

};