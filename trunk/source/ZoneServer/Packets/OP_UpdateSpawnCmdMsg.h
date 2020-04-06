#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"
#include "../Spawns/SpawnStructs.h"
#include "Substruct_SpawnHeader.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"
#include "../ZoneServer/Client.h"

class Substruct_SpawnPosUpdate : public PacketSubstruct {
public:
	Substruct_SpawnPosUpdate(uint32_t version) : PacketSubstruct(version), data(version), spawnIndex(0) {

	}

	~Substruct_SpawnPosUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		RegisterSubstruct(data)->SetIsVariableSet(e);
	}

	Substruct_SpawnPosition data;
	uint16_t spawnIndex;
};

class Substruct_SpawnInfoUpdate : public PacketSubstruct {
public:
	Substruct_SpawnInfoUpdate(uint32_t version) : PacketSubstruct(version), data(version), spawnIndex(0), isPlayer(false), coeTimestamp(0) {

	}

	~Substruct_SpawnInfoUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		RegisterSubstruct(data)->SetIsVariableSet(e);

		if (GetVersion() >= 1188) {
			PacketElement* bplayer = RegisterBool(isPlayer);
			bplayer->SetIsVariableSet(e);
			RegisterUInt32(coeTimestamp)->SetIsVariableSet(bplayer);
		}
	}

	Substruct_SpawnInfo data;
	uint16_t spawnIndex;
	bool isPlayer;
	uint32_t coeTimestamp;
};

class Substruct_SpawnVisUpdate : public PacketSubstruct {
public:
	Substruct_SpawnVisUpdate(uint32_t version) : PacketSubstruct(version), data(version), spawnIndex(0) {

	}

	~Substruct_SpawnVisUpdate() = default;

	void RegisterElements() {
		PacketElement* e = RegisterOversizedByte(spawnIndex);
		e->SetIsVariableSet(e);
		RegisterSubstruct(data)->SetIsVariableSet(e);
	}

	Substruct_SpawnVisualization data;
	uint16_t spawnIndex;
};

class OP_UpdateGhostCmdMsg_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_UpdateGhostCmdMsg_Packet(uint32_t version) : OP_ClientCmdMsg_Packet(version), 
		info(version), pos(version), vis(version),
		packedInfo(version > 283, true), packedPos(version > 283, true), packedVis(version > 283, true)
	{
		packedInfo.LinkSubstruct(info, "info");
		packedPos.LinkSubstruct(pos, "pos");
		packedVis.LinkSubstruct(vis, "vis");
		RegisterElements();
	}

	~OP_UpdateGhostCmdMsg_Packet() = default;

	void RegisterElements() {
		RegisterUInt32(timestamp);
		RegisterSubstruct(packedInfo);
		RegisterSubstruct(packedPos);
		RegisterSubstruct(packedVis);
	}

	void SetEncodedBuffers(const std::shared_ptr<Client>& client, uint16_t spawnIndex) {
		pos.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnPos, spawnIndex));
		info.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnInfo, spawnIndex));
		vis.data.SetEncodedBuffer(client->encoded_packets.GetBuffer(EEncoded_UpdateSpawnVis, spawnIndex));
	}

	uint32_t timestamp;
	Substruct_SpawnInfoUpdate info;
	PacketPackedData packedInfo;
	Substruct_SpawnPosUpdate pos;
	PacketPackedData packedPos;
	Substruct_SpawnVisUpdate vis;
	PacketPackedData packedVis;
};