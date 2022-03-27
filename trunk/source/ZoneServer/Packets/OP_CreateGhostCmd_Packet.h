#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"
#include "../Spawns/SpawnStructs.h"
#include "Substruct_SpawnHeader.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../ZoneServer/Client.h"
#include "../Spawns/Spawn.h"
#include "../../common/timer.h"

class OP_CreateGhostCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_CreateGhostCmd_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version),
		header(version), pos(version), vis(version), info(version), 
		footer(version), packedData(version <= 283) {
		packedData.LinkSubstruct(pos, "pos");
		packedData.LinkSubstruct(vis, "vis");
		packedData.LinkSubstruct(info, "info");
		RegisterElements();
	}
	~OP_CreateGhostCmd_Packet() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		if (bUnpackData) {
			RegisterSubstruct(packedData);
		}
		else {
			Register32String(packedDataBuf);
		}
		if (GetVersion() > 283) {
			RegisterSubstruct(footer);
		}
	}

	void PostRead() override {
		//This is for the parser
		if (packedData.lastPackedSize != 0) {
			pos.DecodedRead();
			vis.DecodedRead();
			info.DecodedRead();

			pos.PostRead();
			vis.PostRead();
			info.PostRead();
		}
	}

	virtual void InsertSpawnData(const std::shared_ptr<Client>& client, const std::shared_ptr<Spawn>& spawn, uint16_t index) {
		SetHeaderData(client, spawn, index, client->GetIDForSpawn(spawn));
		pos.InsertSpawnData(client, spawn);
		static_cast<SpawnInfoStruct&>(info) = *spawn->GetInfoStruct();
		vis.DetermineForClient(client, spawn);
		SetFooterData(spawn);
	}

	void SetHeaderData(const std::shared_ptr<Client>& client, const std::shared_ptr<Spawn>& spawn, uint16_t index, uint32_t id) {
		header.index = index;
		header.spawn_id = id;
		header.spawn_anim = 0xFFFFFFFF;
		header.crc = 1;
		header.time_stamp = Timer::GetServerTime();

		header.command_list_array.clear();

		if (auto primaryCommands = spawn->GetPrimaryCommandList()) {
			//Set the default command
			header.default_command = primaryCommands->commands[0].command;
			header.max_distance = primaryCommands->commands[0].distance;

			for (auto& cmd : primaryCommands->commands) {
				header.AddEntityCommand(cmd);
			}
		}

		if (auto secondaryCommands = spawn->GetSecondaryCommandList()) {
			for (auto& cmd : secondaryCommands->commands) {
				header.AddEntityCommand(cmd);
			}
		}


		std::shared_ptr<SpawnGroupList> group = spawn->GetSpawnGroupList();
		if (group) {
			std::vector<std::weak_ptr<Spawn> >* group_spawns = group->GetSpawnList();
			for (std::weak_ptr<Spawn> s : *group_spawns) {
				std::shared_ptr<Spawn> s2 = s.lock();
				if (s2) {
					uint32_t id = client->GetIDForSpawn(s2);
					if (id == 0)
						id = client->AddSpawnToIDMap(s2);
					
					header.AddSpawnGroupID(id);
				}
			}
		}
		
	}

	void SetFooterData(const std::shared_ptr<Spawn>& spawn) {
		static_cast<SpawnTitleStruct&>(footer.titleStruct) = *spawn->GetTitleStruct();

		//Logic from old code.. dunno for sure if it is correct
		if (spawn->IsAttackable()) {
			footer.spawn_type = 3;
		}
		else if (spawn->GetAdventureLevel() > 0) {
			footer.spawn_type = 1;
		}
		else {
			footer.spawn_type = 6;
		}
	}

	void SetEncodedBuffers(const std::shared_ptr<Client>& client, uint32_t spawnIndex) {
		client->InitSpawnBuffers(spawnIndex);
		pos.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnPos, spawnIndex));
		info.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnInfo, spawnIndex));
		vis.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnVis, spawnIndex));
	}

	//Most XOR'd packets actually use default values for a base instead of all 00's (just mostly 00)
	// This is why we have needed to set some values oddly, like hp XOR'd with 100 which is default
	void SetXORDefaults() {
		memset(&static_cast<SpawnInfoStruct&>(info), 0, sizeof(SpawnInfoStruct));
		memset(&static_cast<SpawnVisualizationStruct&>(vis), 0, sizeof(SpawnVisualizationStruct));
		memset(&static_cast<SpawnPositionStruct&>(pos), 0, sizeof(SpawnPositionStruct));

		info.hp_percent = 100;

		info.size_mod = 1.0f;
		info.show_archtype_icon = 0xff;

		//These are all probably IDs, cast maybe spell id. one more in the unknown3b unknown
		info.target_id = -1;
		info.follow_target = -1;
		info.id_unknown = -1;
		info.pet_owner_id = -1;
		info.cast_unknown = -1;
		info.cast_percentage = -1;
		info.unknown3c2 = -1;
		info.visual_flag = 2;
		info.interaction_flag = 2;

		pos.heading = 180.f;
		pos.desiredHeading = 180.f;
	}

	PacketPackedData packedData;

	Substruct_SpawnHeader header;
	Substruct_SpawnPosition pos;
	Substruct_SpawnVisualization vis;
	Substruct_SpawnInfo info;
	Substruct_SpawnFooter footer;
	static bool bUnpackData;
	//This is for the parser where we only want headers/footers in certain cases
	std::string packedDataBuf;
};