#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

class ZoneServer;
class Spawn;

struct SpawnCampLocationEntry {
	uint32_t id;
	float x;
	float y;
	float z;
	float x_offset;
	float y_offset;
	float z_offset;
	float heading;
	float pitch;
	float roll;
	uint32_t grid;
	uint32_t visual_state_override;
	uint32_t action_state_override;
};

struct SpawnCampSpawnEntry {
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

class SpawnCamp {
public:
	SpawnCamp(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid);

	void Initialize();
	void Process();
	void Destroy();
	void Reset();


	void SetID(uint32_t val) { m_id = val; }
	uint32_t GetID() { return m_id; }
	void SetName(std::string val) { m_name = val; }
	std::string GetName() { return m_name; }
	void SetX(float val) { m_x = val; }
	float GetX() { return m_x; }
	void SetY(float val) { m_y = val; }
	float GetY() { return m_y; }
	void SetZ(float val) { m_z = val; }
	float GetZ() { return m_z; }
	void SetGridID(uint32_t val) { m_grid = val; }
	uint32_t GetGridID() { return m_grid; }
	void SetRadius(float val) { m_radius = val; }
	float GetRadius() { return m_radius; }
	void SetNumRadiusEncounter(uint16_t val) { m_numRadiusEncounters = val; }
	uint16_t  GetNumRadiusEncounters() { return m_numRadiusEncounters; }
	std::shared_ptr<ZoneServer> GetZone() { return m_zone.lock(); }

	void AddSpawnID(uint32_t id) { m_spawnIDs.push_back(id); }

protected:

	uint32_t m_id;
	std::string m_name;

	// Area around the camp to spawn stuff
	float m_radius;

	// Number of encounters this camp should spawn within the radius
	uint16_t m_numRadiusEncounters;

	// Flag for the initial spawning
	bool bInitialized;

	// Location info
	float m_x;
	float m_y;
	float m_z;

	// needed to show the spawn in game correctly, otherwise no use
	uint32_t m_grid;

	// zone this camp is in
	std::weak_ptr<ZoneServer> m_zone;

	// list of spawn DB ID's
	std::vector<uint32_t> m_spawnIDs;

	// list of specific locations
	std::vector<std::shared_ptr<SpawnCampLocationEntry> > m_spawnLocations;

	// list of spawns to spawn at specific locations
	// map <location id, vector<spawns>>
	std::map<uint32_t, std::vector<std::shared_ptr<SpawnCampSpawnEntry> > > m_locationSpawnIDs;

	// list of spawns this camp has in the world
	std::vector<std::weak_ptr<Spawn> > m_spawns;
};