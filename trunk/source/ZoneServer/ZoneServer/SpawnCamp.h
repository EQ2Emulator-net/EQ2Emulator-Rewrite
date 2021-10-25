#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <set>

class ZoneServer;
class Spawn;

struct SpawnCampLocationEntry {
	uint32_t id = 0;
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float x_offset = 0.f;
	float y_offset =0.f;
	float z_offset =0.f;
	float heading = 0.f;
	float pitch = 0.f;
	float roll = 0.f;
	uint32_t grid = 0;
	uint32_t visual_state_override = 0;
	uint32_t action_state_override = 0;
};

struct SpawnCampSpawnEntry {
	uint32_t id;
	uint32_t spawnDBID;
	uint32_t respawn_time;
	uint32_t expire_time;
	uint32_t expire_offset;
	uint8_t minLevel;
	uint8_t maxLevel;
	uint8_t minEncounterLevel;
	uint8_t maxEncounterLevel;
	uint64_t conditional;
	uint32_t weight;
};

class SpawnCamp : public std::enable_shared_from_this<SpawnCamp> {
public:
	SpawnCamp(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid);

	virtual void Process();
	void Destroy();
	void Reset();

	virtual bool IsSingleCamp() { return !IsAreaCamp() && !IsGroupCamp() && !IsBossCamp() && !IsEventCamp(); }
	virtual bool IsAreaCamp() { return false; }
	virtual bool IsGroupCamp() { return false; }
	virtual bool IsBossCamp() { return false; }
	virtual bool IsEventCamp() { return false; }


	uint32_t GetID() { return m_id; }
	std::string GetName() { return m_name; }
	float GetX() { return m_x; }
	float GetY() { return m_y; }
	float GetZ() { return m_z; }
	uint32_t GetGridID() { return m_grid; }
	std::shared_ptr<ZoneServer> GetZone() { return m_zone.lock(); }

	void SetID(uint32_t val) { m_id = val; }
	void SetName(std::string val) { m_name = val; }
	void SetX(float val) { m_x = val; }
	void SetY(float val) { m_y = val; }
	void SetZ(float val) { m_z = val; }
	void SetGridID(uint32_t val) { m_grid = val; }

	void AddSpawn(std::shared_ptr<SpawnCampSpawnEntry> scse);
	void AddSpawnCampLocationEntry(std::shared_ptr<SpawnCampLocationEntry> scle) { m_locationEntries.push_back(scle); }
	virtual std::shared_ptr<Spawn> CreateSpawn(uint32_t id, std::shared_ptr<SpawnCampLocationEntry> scle);
	void AddSpawnToZone(std::shared_ptr<Spawn> spawn);

protected:

	uint32_t m_id;
	std::string m_name;

	// Location info
	float m_x;
	float m_y;
	float m_z;

	// needed to show the spawn in game correctly, otherwise no use
	uint32_t m_grid;

	uint32_t m_totalWeight;

	// zone this camp is in
	std::weak_ptr<ZoneServer> m_zone;

	// list of spawn DB ID's
	std::map<uint32_t, std::shared_ptr<SpawnCampSpawnEntry> > m_spawnEntries;

	std::vector<std::shared_ptr<SpawnCampLocationEntry> > m_locationEntries;

	// list of spawns this camp has in the world
	std::vector<std::weak_ptr<Spawn> > m_spawns;
};

class SpawnCampArea : public SpawnCamp {
public:

	SpawnCampArea(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid)
		: SpawnCamp(zone, x, y, z, grid) {

	}
	
	virtual bool IsAreaCamp() override { return true; }

	void SetRadius(float val) { m_radius = val; }
	float GetRadius() { return m_radius; }
	void SetNumRadiusEncounter(uint16_t val) { m_numRadiusEncounters = val; }
	uint16_t  GetNumRadiusEncounters() { return m_numRadiusEncounters; }

private:

	// Area around the camp to spawn stuff
	float m_radius;

	// Number of encounters this camp should spawn within the radius
	uint16_t m_numRadiusEncounters;
};

class SpawnCampGroup : public SpawnCamp {
public:

	SpawnCampGroup(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid)
		: SpawnCamp(zone, x, y, z, grid) {

	}

	virtual void Process() override;
	virtual bool IsGroupCamp() override { return true; }
	virtual std::shared_ptr<Spawn> CreateSpawn(uint32_t id, std::shared_ptr<SpawnCampLocationEntry> scle) override;

	void SetMinSpawns(uint16_t val) { m_minSpawns = val; }
	void SetMaxSpawns(uint16_t val) { m_maxSpawns = val; }

	uint16_t GetMinSpawns() { return m_minSpawns; }
	uint16_t GetMaxSpawns() { return m_maxSpawns; }

protected:

	uint16_t m_minSpawns;
	uint16_t m_maxSpawns;


};

class SpawnCampBoss : public SpawnCampGroup {
public:

	SpawnCampBoss(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid)
		: SpawnCampGroup(zone, x, y, z, grid) {

	}

	virtual bool IsBossCamp() override { return true; }

};

class SpawnCampEvent : public SpawnCampGroup {
public:

	SpawnCampEvent(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid)
		: SpawnCampGroup(zone, x, y, z, grid) {

	}

	virtual bool IsEventCamp() override { return true; }

};