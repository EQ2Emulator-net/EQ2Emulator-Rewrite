#include "stdafx.h"

#include "MasterZoneLookup.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/log.h"
#include "../../common/string.h"

bool ZoneDatabase::LoadMasterZoneList(MasterZoneLookup& lookup) {
	DatabaseResult result;
	if (!Select(&result, "SELECT * FROM zones")) {
		LogDebug(LOG_DATABASE, 0, "Could not load master zone list!");
		return false;
	}

	std::map<uint32_t, std::shared_ptr<MasterZoneDetails> > zoneDetails;

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t id = result.GetUInt32(0);
		auto details = std::make_shared<MasterZoneDetails>();
		zoneDetails[id] = details;

		details->id = id;
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

		count++;
	}

	if (!Select(&result, "SELECT z.id, zm.map_id, zm.lowest_z, zm.highest_z, zmf.*\n"
		"FROM zone_maps zm\n"
		"LEFT JOIN zone_map_files zmf ON zmf.map_id = zm.map_id\n"
		"INNER JOIN zones z ON zm.map_id = z.map_id\n"
		"ORDER BY z.id, zmf.id")) {
		LogDebug(LOG_DATABASE, 0, "Could not load map data!");
	}
	
	while (result.Next()) {
		uint32_t i = 0;

		uint32_t zone_id = result.GetUInt32(i++);

		auto itr = zoneDetails.find(zone_id);
		if (itr == zoneDetails.end()) {
			continue;
		}

		std::shared_ptr<MasterZoneDetails>& zd = itr->second;

		if (!zd->mapData) {
			zd->mapData = std::make_unique<ZoneMapData>();
			ZoneMapData& md = *zd->mapData;
			md.mapID = result.GetUInt32(i++);
			zd->mapData->lowestZ = result.GetFloat(i++);
			zd->mapData->highestZ = result.GetFloat(i++);
		}
		
		//Skip over the first few fields in case we already loaded them
		i = 6;

		//This result doesn't contain any file info
		if (result.IsNull(i)) {
			continue;
		}

		zd->mapData->files.emplace_back();
		ZoneMapFile& md = zd->mapData->files.back();

		md.fileName = result.GetString(i++);
		md.exploredMapName = result.GetString(i++);
		md.unexploredMapName = result.GetString(i++);
		md.boundsX1 = result.GetFloat(i++);
		md.boundsZ1 = result.GetFloat(i++);
		md.boundsX2 = result.GetFloat(i++);
		md.boundsZ2 = result.GetFloat(i++);
		md.boundsX3 = result.GetFloat(i++);
		md.boundsZ3 = result.GetFloat(i++);
		md.boundsX4 = result.GetFloat(i++);
		md.boundsZ4 = result.GetFloat(i++);
		md.exploredKey = result.GetUInt64(i++);
		md.unexploredKey = result.GetUInt64(i++);
	}

	lookup.SetZoneDetails(zoneDetails);

	LogDebug(LOG_DATABASE, 0, "Loaded %u zones to master zone list", count);

	return true;
}

void MasterZoneLookup::SetZoneDetails(std::map<uint32_t, std::shared_ptr<MasterZoneDetails> >& details) {
	WriteLocker lock(masterList_lock);
	masterList = std::move(details);
}

std::shared_ptr<const MasterZoneDetails> MasterZoneLookup::GetZoneInfoByID(uint32_t id) {
	std::shared_ptr<MasterZoneDetails> ret;

	ReadLocker lock(masterList_lock);
	auto itr = masterList.find(id);
	if (itr != masterList.end()) {
		ret = itr->second;
	}

	return ret;
}

std::shared_ptr<const MasterZoneDetails> MasterZoneLookup::GetZoneInfoByName(std::string name) {
	std::shared_ptr<MasterZoneDetails> ret;

	ReadLocker lock(masterList_lock);
	for (auto& itr : masterList) {
		if (strcasecmp(itr.second->name.c_str(), name.c_str()) == 0) {
			ret = itr.second;
			break;
		}
	}

	return ret;
}

std::shared_ptr<const MasterZoneDetails> MasterZoneLookup::GetZoneInfoByFileName(const std::string& name) {
	std::shared_ptr<MasterZoneDetails> ret;

	ReadLocker lock(masterList_lock);
	for (auto& itr : masterList) {
		if (itr.second->file == name) {
			ret = itr.second;
			break;
		}
	}

	return ret;
}