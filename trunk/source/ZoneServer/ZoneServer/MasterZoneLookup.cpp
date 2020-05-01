#include "stdafx.h"

#include "MasterZoneLookup.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/log.h"
#include "../../common/string.h"

bool ZoneDatabase::LoadMasterZoneList(class MasterZoneLookup& lookup) {
	DatabaseResult result;
	if (!Select(&result, "SELECT * FROM zones")) {
		LogDebug(LOG_DATABASE, 0, "Could not load master zone list!");
		return false;
	}


	uint32_t count = 0;
	while (result.Next()) {
		MasterZoneDetails* details = new MasterZoneDetails;

		details->id = result.GetUInt32(0);
		details->expansionID = result.GetUInt8(1);
		details->name = result.GetString(2);
		details->file = result.GetString(3);
		details->description = result.GetString(4);
		details->safeX = result.GetFloat(5);
		details->safeY = result.GetFloat(6);
		details->safeZ = result.GetFloat(7);
		details->safeHeading = result.GetFloat(8);
		details->underworld = result.GetFloat(9);
		details->expModifier = result.GetFloat(10);
		details->minRecommended = result.GetUInt8(11);
		details->maxRecomended = result.GetUInt8(12);
		details->zoneType = result.GetString(13);
		details->isAlwaysLoaded = result.GetBool(14);
		details->cityZone = result.GetBool(15);
		details->weatherAllowed = result.GetBool(16);
		details->minStatus = result.GetUInt32(17);
		details->minLevel = result.GetUInt32(18);
		details->maxLevel = result.GetUInt32(19);
		details->startingZone = result.GetUInt8(20);
		details->instanceType = result.GetString(21);
		details->defaultReenterTime = result.GetUInt32(22);
		details->defaultResetTime = result.GetUInt32(23);
		details->defaultLockoutTime = result.GetUInt32(24);
		details->forceGroupToZone = result.GetUInt16(25);
		details->luaScript = result.GetString(26);
		details->shutdownTimer = result.GetUInt32(27);
		details->zoneMOTD = result.GetString(28);
		details->rulesetID = result.GetUInt32(29);

		lookup.AddZoneDetails(details);
		count++;
	}


	LogDebug(LOG_DATABASE, 0, "Loaded %u zones to master zone list", count);

	return true;
}

uint32_t MasterZoneLookup::AddZoneDetails(MasterZoneDetails* details_entry) {
	{
		WriteLocker lock(masterList_lock);

		masterList[details_entry->id] = details_entry;
	}

	return details_entry->id;
}

MasterZoneDetails* MasterZoneLookup::GetZoneInfoByID(uint32_t id) {
	MasterZoneDetails* ret = nullptr;

	ReadLocker lock(masterList_lock);
	for (auto& itr : masterList) {
		if (itr.second->id == id) {
			ret = itr.second;
			return ret;
		}
	}

	return ret;
}

MasterZoneDetails* MasterZoneLookup::GetZoneInfoByName(std::string name) {
	MasterZoneDetails* ret = nullptr;

	ReadLocker lock(masterList_lock);
	for (auto& itr : masterList) {
		if (strcasecmp(itr.second->name.c_str() ,name.c_str()) == 0) {
			ret = itr.second;
			return ret;
		}
	}

	return ret;
}