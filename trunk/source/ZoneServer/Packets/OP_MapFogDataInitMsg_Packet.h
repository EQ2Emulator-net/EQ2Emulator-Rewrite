#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"
#include "OP_MapFogDataUpdateMsg_Packet.h"

class Substruct_MapData : public PacketSubstruct {
public:
	Substruct_MapData(uint32_t ver = 0) : PacketSubstruct(ver, true) { }

	class Substruct_SubArray : public PacketSubstruct {
	public:
		Substruct_SubArray(uint32_t ver = 0) : PacketSubstruct(ver, true) {}

		void RegisterElements() override {
			RegisterUInt8(unkByte);
			Register16String(unkString);
			RescopeArrayElement(unkData);
			RegisterInt32(unkData)->SetCount(8);
		}

		uint8_t unkByte;
		std::string unkString;
		int32_t unkData[8];
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
		auto e = RegisterOversizedByte(unkArraySize);
		e->SetMyArray(RegisterArray(unkArray, Substruct_SubArray));
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
	uint16_t unkArraySize;
	std::vector<Substruct_SubArray> unkArray;
};

class OP_MapForDataInitMsg_Packet : public EQ2Packet {
public:
	OP_MapForDataInitMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
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

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_PLAYER, 0, "Received a map fog data update...");
	}
};