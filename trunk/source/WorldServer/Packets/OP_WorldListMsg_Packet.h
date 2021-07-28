#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketSubstruct.h"
#include <vector>

class Substruct_WorldListEntry : public PacketSubstruct {
public:
	Substruct_WorldListEntry(uint32_t ver = 0) : PacketSubstruct(ver) {
		//We're only sending one world at the moment so just hardcode some stuff
		ID = 1;
		bServerUp = true;
		Locked = 0;
		Hidden = 0;
		Unknown = 0;
		NumPlayers = 0;
		Index = 0;
		NumOnlineFlag = 1;
		Flags = 0;
		AllowedRaces = 0x003FFFFF;
	}

	void RegisterElements() override {
		RegisterUInt32(ID);
		Register16String(Name);
		if (GetVersion() > 283) {
			Register16String(Name2);
		}
		RegisterBool(bServerUp);
		RegisterUInt8(Locked);
		if (GetVersion() > 283) {
			RegisterUInt8(Hidden);
			if (GetVersion() > 864) {
				RegisterUInt8(Unknown);
			}
		}
		RegisterUInt8(NumPlayers);
		if (GetVersion() > 283) {
			RegisterUInt16(Index);
		}
		RegisterUInt8(NumOnlineFlag);

		if (GetVersion() <= 283) {
			return;
		}

		RegisterUInt8(Unk2);

		if (GetVersion() >= 60100) {
			RegisterUInt16(Flags);
		}
		else {
			RescopeToReference(Flags, uint8_t);
			RegisterUInt8(Flags);
		}
		RegisterUInt32(AllowedRaces);
	}

	uint32_t ID;
	std::string Name;
	std::string Name2;
	bool bServerUp;
	uint8_t Locked;
	uint8_t Hidden;
	uint8_t Unknown;
	uint8_t NumPlayers;
	uint16_t Index;
	uint8_t NumOnlineFlag;
	//Flags values
	// 1 = Roleplaying
	// 8 = Euro English
	// 16 = Exchange enabled server
	// 32 = PVP
	// 256 = TLE
	// 512 = Event
	// 1024 = Prison Server
	// 2048 = Free Trade
	// 4096 = Heroic
	uint16_t Flags;
	uint8_t Unk2;
	uint32_t AllowedRaces;
};

class OP_WorldListMsg_Packet : public EQ2Packet {
public:
	OP_WorldListMsg_Packet(uint32_t version)
		: EQ2Packet(version), worlds(1) {
		RegisterElements();

		NumWorlds = 1;
		Unknown2 = 0;
		//worlds.emplace_back();
		//auto& tar = worlds[1];
		//tar.Name = "Tarinax";
		//tar.Name2 = tar.Name;
		//tar.Flags = 256 | 32;
		//tar.ID = 2;
	}

	uint8_t Unknown2; // 60100
	std::vector<Substruct_WorldListEntry> worlds;

private:
	uint8_t NumWorlds;

	void RegisterElements() {
		auto e = RegisterUInt8(NumWorlds);
		e->SetMyArray(RegisterArray(worlds, Substruct_WorldListEntry));
		if (GetVersion() >= 60100)
			RegisterUInt8(Unknown2);
	}

};