#pragma once

#include "Packets/PacketElements/PacketElements.h"
#include <string>

class CharacterBasicInfo {
public:
	CharacterBasicInfo() {
		advLevel = 0;
		tradeskillLevel = 0;
		advClass = 0;
		tradeskillClass = 0;
		u_statusFlags = 0;
		alignment = 0;
		race = 0;
		characterID = 0;
		accountID = 0;
	}

	~CharacterBasicInfo() = default;

	std::string characterName;
	std::string currentZone;
	std::string guild;
	uint32_t characterID;
	uint32_t accountID;
	uint8_t advLevel;
	uint8_t tradeskillLevel;
	uint8_t advClass;
	uint8_t tradeskillClass;
	uint8_t alignment;
	uint8_t race;

	struct PlayerStatusFlags {
		bool bAfk:1;
		bool bRoleplaying:1;
		bool bLfg:1;
		bool bLfw:1;
		bool bAnonymous:1;
	};

	union {
		PlayerStatusFlags statusFlags;
		uint8_t u_statusFlags;
	};
};

class EmuSubstruct_CharacterBasicInfo : public PacketSubstruct, public CharacterBasicInfo {
public:
	EmuSubstruct_CharacterBasicInfo() : PacketSubstruct(0) {

	}

	void RegisterElements() override {
		RegisterUInt32(characterID);
		RegisterUInt32(accountID);
		Register8String(characterName);
		Register8String(currentZone);
		Register8String(guild);
		RegisterUInt8(advLevel);
		RegisterUInt8(tradeskillLevel);
		RegisterUInt8(advClass);
		RegisterUInt8(tradeskillClass);
		RegisterUInt8(u_statusFlags);
		RegisterUInt8(alignment);
		RegisterUInt8(race);
	}
};