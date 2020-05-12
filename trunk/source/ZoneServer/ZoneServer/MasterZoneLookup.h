#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "../../common/Mutex.h"

struct ZoneMapFile {
	std::string fileName;
	std::string exploredMapName;
	std::string unexploredMapName;
	float boundsX1;
	float boundsZ1;
	float boundsX2;
	float boundsZ2;
	float boundsX3;
	float boundsZ3;
	float boundsX4;
	float boundsZ4;
	uint64_t exploredKey;
	uint64_t unexploredKey;
};

struct ZoneMapData {
	uint32_t mapID;
	float lowestZ;
	float highestZ;
	std::vector<ZoneMapFile> files;
};

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
	std::unique_ptr<ZoneMapData> mapData;
};

class MasterZoneLookup {
public:
	MasterZoneLookup() = default;
	~MasterZoneLookup() = default;

	void SetZoneDetails(std::map<uint32_t, std::shared_ptr<MasterZoneDetails> >& details);
	std::shared_ptr<const MasterZoneDetails> GetZoneInfoByID(uint32_t id);
	std::shared_ptr<const MasterZoneDetails> GetZoneInfoByName(std::string name);
	std::shared_ptr<const MasterZoneDetails> GetZoneInfoByFileName(const std::string& name);

private:

	Mutex masterList_lock;
	std::map<uint32_t, std::shared_ptr<MasterZoneDetails> > masterList;
};