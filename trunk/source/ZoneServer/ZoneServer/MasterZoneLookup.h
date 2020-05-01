#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "../../common/Mutex.h"

struct MasterZoneDetails {
	uint32_t id;
	uint8_t expansionID;
	std::string name;
	std::string file;
	std::string description;
	float safeX;
	float safeY;
	float safeZ;
	float safeHeading;
	float underworld;
	float expModifier;
	uint8_t minRecommended;
	uint8_t maxRecomended;
	std::string zoneType;
	bool isAlwaysLoaded;
	bool cityZone;
	bool weatherAllowed;
	uint32_t minStatus;
	uint32_t minLevel;
	uint32_t maxLevel;
	uint8_t startingZone;
	std::string instanceType;
	uint32_t defaultReenterTime;
	uint32_t defaultResetTime;
	uint32_t defaultLockoutTime;
	uint16_t forceGroupToZone;
	std::string luaScript;
	uint32_t shutdownTimer;
	std::string zoneMOTD;
	uint32_t rulesetID;
};

class MasterZoneLookup {
public:
	MasterZoneLookup() = default;
	~MasterZoneLookup() = default;

	uint32_t AddZoneDetails(MasterZoneDetails* details_entry);
	MasterZoneDetails* GetZoneInfoByID(uint32_t id);
	MasterZoneDetails* GetZoneInfoByName(std::string name);

private:

	Mutex masterList_lock;
	std::map<uint32_t, MasterZoneDetails*> masterList;
};