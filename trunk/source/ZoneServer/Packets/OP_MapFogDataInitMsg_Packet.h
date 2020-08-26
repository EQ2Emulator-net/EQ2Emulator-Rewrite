#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"
#include "OP_MapFogDataUpdateMsg_Packet.h"
#include "../ZoneServer/MasterZoneLookup.h"

class Substruct_MapData : public PacketSubstruct {
public:
	Substruct_MapData(uint32_t ver = 0) : PacketSubstruct(ver) {
		memset(unknown6, 0, sizeof(unknown6));
		unknown7 = 1600;
		unknown8 = 1200;
		unknown = 0;
		memset(unknown9, 0, sizeof(unknown9));
	}

	class Substruct_POI : public PacketSubstruct {
	public:
		Substruct_POI(uint32_t ver = 0) : PacketSubstruct(ver, true) { }

		void RegisterElements() override {
			RegisterUInt8(unkByte);
			Register16String(poiName);
			RegisterFloat(poiX);
			RegisterFloat(poiY);
			RegisterFloat(poiZ);
			RescopeArrayElement(unkData);
			RegisterInt32(unkData)->SetCount(5);
		}

		uint8_t unkByte;
		std::string poiName;
		float poiX;
		float poiY;
		float poiZ;
		int32_t unkData[5];
	};

	void RegisterElements() override {
		Register16String(zone_name);
		Register16String(explored_map_name);
		Register16String(unexplored_map_name);
		RescopeArrayElement(unknown6);
		RegisterInt32(unknown6)->SetCount(2);
		RegisterInt32(unknown7);
		RegisterInt32(unknown8);
		RegisterFloat(boundsX1);
		RegisterFloat(boundsZ1);
		RegisterFloat(boundsX2);
		RegisterFloat(boundsZ2);
		RegisterFloat(boundsX3);
		RegisterFloat(boundsZ3);
		RegisterFloat(boundsX4);
		RegisterFloat(boundsZ4);
		RescopeArrayElement(unknown9);
		RegisterInt32(unknown9)->SetCount(2);
		RegisterOversizedByte(unknown);
		RegisterUInt64(explored_key);
		RegisterUInt64(unexplored_key);
		auto e = RegisterOversizedByte(poiArraySize);
		e->SetMyArray(RegisterArray(poiArray, Substruct_POI));
	}

	std::string zone_name;
	std::string explored_map_name;
	std::string unexplored_map_name;
	int32_t unknown6[2];
	int32_t unknown7;
	int32_t unknown8;
	float boundsX1;
	float boundsZ1;
	float boundsX2;
	float boundsZ2;
	float boundsX3;
	float boundsZ3;
	float boundsX4;
	float boundsZ4;
	int32_t unknown9[2];
	uint16_t unknown;
	uint64_t explored_key;
	uint64_t unexplored_key;
	uint16_t poiArraySize;
	std::vector<Substruct_POI> poiArray;
};

class OP_MapFogDataInitMsg_Packet : public EQ2Packet {
public:
	OP_MapFogDataInitMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
		unknown3 = 0;
		unknown4 = 0;
		unknown1 = 0;
	}

	uint8_t unknown1;
	uint32_t mapID;
	float lowest_z;
	float highest_z;
	uint8_t unknown3;
	uint8_t unknown4;
	uint8_t mapCount;
	std::vector<Substruct_MapData> mapArray;
	uint8_t fogCount;
	std::vector<Substruct_MapFogData> fogArray;
	uint8_t map2Count;
	std::vector<Substruct_MapData> map2Array;

	void RegisterElements() {
		RegisterUInt8(unknown1);
		RegisterUInt32(mapID);
		RegisterFloat(lowest_z);
		RegisterFloat(highest_z);
		RegisterUInt8(unknown3);
		RegisterUInt8(unknown4);
		auto e = RegisterUInt8(mapCount);
		e->SetMyArray(RegisterArray(mapArray, Substruct_MapData));
		e = RegisterUInt8(fogCount);
		e->SetMyArray(RegisterArray(fogArray, Substruct_MapFogData));
		e = RegisterUInt8(map2Count);
		e->SetMyArray(RegisterArray(map2Array, Substruct_MapData));
	}

	void InsertMapData(const ZoneMapData& zd) {
		mapID = zd.mapID;
		lowest_z = zd.lowestZ;
		highest_z = zd.highestZ;

		map2Array.reserve(zd.files.size());
		map2Array.clear();

		for (auto& map : zd.files) {
			map2Array.emplace_back(GetVersion());
			Substruct_MapData& md = map2Array.back();

			md.zone_name = map.fileName;
			md.explored_map_name = map.exploredMapName;
			md.unexplored_map_name = map.unexploredMapName;
			md.boundsX1 = map.boundsX1;
			md.boundsX2 = map.boundsX2;
			md.boundsX3 = map.boundsX3;
			md.boundsX4 = map.boundsX4;
			md.boundsZ1 = map.boundsZ1;
			md.boundsZ2 = map.boundsZ2;
			md.boundsZ3 = map.boundsZ3;
			md.boundsZ4 = map.boundsZ4;
			md.explored_key = map.exploredKey;
			md.unexplored_key = map.unexploredKey;
		}
	}
};