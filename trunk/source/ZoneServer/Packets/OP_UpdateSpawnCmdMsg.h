#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"
#include "../Spawns/SpawnStructs.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../ZoneServer/Client.h"

class Substruct_SpawnPosUpdate : public PacketSubstruct {
public:
	Substruct_SpawnPosUpdate(uint32_t version) : PacketSubstruct(version), 
		packedPos(version <= 283, false), data(version), spawnIndex(0), isPlayer(false), coeTimestamp(0) {
		packedPos.LinkSubstruct(data, "packedPos");
	}

	~Substruct_SpawnPosUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		if (GetVersion() >= 1188) {
			PacketElement* bplayer = RegisterBool(isPlayer);
			bplayer->SetIsVariableSet(e);
			RegisterUInt32(coeTimestamp)->SetIsVariableSet(bplayer);
		}
		RegisterSubstruct(packedPos)->SetIsVariableSet(e);
	}

	Substruct_SpawnPosition data;
	PacketPackedData packedPos;
	uint16_t spawnIndex;
	bool isPlayer;
	uint32_t coeTimestamp;
};

class Substruct_SpawnInfoUpdate : public PacketSubstruct {
public:
	Substruct_SpawnInfoUpdate(uint32_t version) : PacketSubstruct(version), data(version), spawnIndex(0), packedInfo(version <= 283, false) {
		packedInfo.LinkSubstruct(data, "packedInfo");
	}

	~Substruct_SpawnInfoUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		RegisterSubstruct(packedInfo)->SetIsVariableSet(e);
	}

	Substruct_SpawnInfo data;
	PacketPackedData packedInfo;
	uint16_t spawnIndex;
	
};

class Substruct_SpawnVisUpdate : public PacketSubstruct {
public:
	Substruct_SpawnVisUpdate(uint32_t version) : PacketSubstruct(version), data(version), spawnIndex(0), packedVis(version <= 283, false) {
		packedVis.LinkSubstruct(data, "packedVis");
	}

	~Substruct_SpawnVisUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		RegisterSubstruct(packedVis)->SetIsVariableSet(e);
	}

	Substruct_SpawnVisualization data;
	PacketPackedData packedVis;
	uint16_t spawnIndex;
};

class OP_UpdateGhostCmdMsg_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_UpdateGhostCmdMsg_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version), 
		info(version), pos(version), vis(version), infoSize(0), visSize(0), posSize(0)
	{
		RegisterElements();
	}

	~OP_UpdateGhostCmdMsg_Packet() = default;

	void RegisterElements() {
		RegisterUInt32(timestamp);
		PacketElement* e = RegisterOversizedByte(infoSize);
		RegisterSubstruct(info)->SetIsVariableSet(e);
		e = RegisterOversizedByte(posSize);
		RegisterSubstruct(pos)->SetIsVariableSet(e);
		e = RegisterOversizedByte(visSize);
		RegisterSubstruct(vis)->SetIsVariableSet(e);
	}

	uint32_t Write(unsigned char*& writeBuffer) override {
		infoSize = static_cast<uint16_t>(info.GetSize());
		posSize = static_cast<uint16_t>(pos.GetSize());
		visSize = static_cast<uint16_t>(vis.GetSize());
		return OP_ClientCmdMsg_Packet::Write(writeBuffer);
	}

	void InsertSpawnInfoData(const SpawnInfoStruct& spawn_info, uint16_t index) {
		info.spawnIndex = index;
		static_cast<SpawnInfoStruct&>(info.data) = spawn_info;
	}

	void InsertSpawnPosData(const SpawnPositionStruct& spawn_pos, uint16_t index, bool isPlayer, uint32_t timestamp) {
		pos.spawnIndex = index;
		pos.isPlayer = isPlayer;
		pos.coeTimestamp = timestamp;
		static_cast<SpawnPositionStruct&>(pos.data) = spawn_pos;
	}

	void InsertSpawnVisData(const SpawnVisualizationStruct& spawn_vis, uint16_t index) {
		vis.spawnIndex = index;
		static_cast<SpawnVisualizationStruct&>(vis.data) = spawn_vis;
	}

	void SetEncodedBuffers(const std::shared_ptr<Client>& client, uint16_t spawnIndex) {
		pos.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnPos, spawnIndex));
		info.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnInfo, spawnIndex));
		vis.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnVis, spawnIndex));
	}

	uint32_t timestamp;
	Substruct_SpawnInfoUpdate info;
	Substruct_SpawnPosUpdate pos;
	Substruct_SpawnVisUpdate vis;

private:
	uint16_t infoSize;
	uint16_t posSize;
	uint16_t visSize;
};