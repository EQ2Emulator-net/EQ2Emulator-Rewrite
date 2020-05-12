#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../ZoneServer/Client.h"

class Substruct_Byte : public PacketSubstruct {
public:
	Substruct_Byte(uint32_t ver = 0) : PacketSubstruct(ver, true) {}

	void RegisterElements() override {
		RegisterUInt8(data);
	}

	uint8_t data;
};

class Substruct_MapFogData : public PacketSubstruct {
public:
	Substruct_MapFogData(uint32_t ver = 0) : PacketSubstruct(ver, true) { }

	void RegisterElements() override {
		RegisterInt32(unknown1);
		Register16String(location_name);
		RegisterFloat(boundsX1);
		RegisterFloat(boundsZ1);
		RegisterFloat(boundsX2);
		RegisterFloat(boundsZ2);
		RegisterOversizedByte(unknown2);
		RegisterOversizedByte(unknown3);
		auto e = RegisterOversizedByte(compressedDataSize);
		e->SetMyArray(RegisterArray(compressedData, Substruct_Byte));
	}

	int32_t unknown1;
	std::string location_name;
	float boundsX1;
	float boundsX2;
	float boundsZ1;
	float boundsZ2;
	uint16_t unknown2;
	uint16_t unknown3;
	uint16_t compressedDataSize;
	std::vector<Substruct_Byte> compressedData;
};

class OP_MapFogDataUpdateMsg_Packet : public EQ2Packet {
public:
	OP_MapFogDataUpdateMsg_Packet(uint32_t version) : EQ2Packet(version) {
		RegisterElements();
	}

	uint8_t fogCount;
	std::vector<Substruct_MapFogData> fogArray;

	void RegisterElements() {
		auto e = RegisterUInt8(fogCount);
		e->SetMyArray(RegisterArray(fogArray, Substruct_MapFogData));
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		LogDebug(LOG_PLAYER, 0, "Received a map fog data update...");
	}
};