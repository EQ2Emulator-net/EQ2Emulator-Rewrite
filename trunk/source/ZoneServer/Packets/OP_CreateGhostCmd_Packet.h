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
		header(version), pos(version), vis(version), info(version), footer(version), packedData(version <= 283) {
		RegisterElements();
	}
	~OP_CreateGhostCmd_Packet() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		packedData.LinkSubstruct(pos, "pos");
		packedData.LinkSubstruct(vis, "vis");
		packedData.LinkSubstruct(info, "info");
		RegisterSubstruct(packedData);
		RegisterSubstruct(footer);
	}

	virtual void InsertSpawnData(const std::shared_ptr<Spawn>& spawn, uint16_t index) {
		SetHeaderData(spawn, index);
		static_cast<SpawnPositionStruct&>(pos) = *spawn->GetPosStruct();
		static_cast<SpawnInfoStruct&>(info) = *spawn->GetInfoStruct();
		static_cast<SpawnVisualizationStruct&>(vis) = *spawn->GetVisStruct();
		SetFooterData(spawn);
	}

	void SetHeaderData(const std::shared_ptr<Spawn>& spawn, uint16_t index) {
		header.index = index;
		header.spawn_id = spawn->GetID();
		header.spawn_anim = 0xFFFFFFFF;
		header.crc = spawn->GetID();
		header.time_stamp = Timer::GetCurrentTime2();
	}

	void SetFooterData(const std::shared_ptr<Spawn>& spawn) {
		static_cast<SpawnTitleStruct&>(footer.titleStruct) = *spawn->GetTitleStruct();

		//ADD PROPER WAY TO GET SPAWN TYPE
		footer.spawn_type = 1;
	}

	void SetTestData() {
		header.index = 1;
		header.spawn_id = 1;
		header.spawn_anim = 0xFFFFFFFF;
		header.crc = 1;
		header.time_stamp = Timer::GetCurrentTime2();
		pos.grid_id = 1841871875;
		pos.X = 242.55f;
		pos.Y = 12.21f;
		pos.Z = -978.47f;
		pos.state = 16512;
		pos.size = 0;
		pos.collision_radius = 28;
		pos.size_ratio = 1.f;
		pos.size_multiplier_ratio = 1.0f;
		vis.vis_flags = 55;
		info.model_type = 116;
		info.soga_model_type = 114;
		info.hp_remaining = 101;
		//footer.name = "Foof";
		//footer.spawn_type = 1;
		//footer.is_player = true;

		info.interaction_flag = 12;
		info.emote_voice = 1023;
	}

	void SetEncodedBuffers(const std::shared_ptr<Client>& client, uint32_t spawnIndex) {
		pos.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnPos, spawnIndex));
		info.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnInfo, spawnIndex));
		vis.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnVis, spawnIndex));
	}

	PacketPackedData packedData;

	Substruct_SpawnHeader header;
	Substruct_SpawnPosition pos;
	Substruct_SpawnVisualization vis;
	Substruct_SpawnInfo info;
	Substruct_SpawnFooter footer;
};