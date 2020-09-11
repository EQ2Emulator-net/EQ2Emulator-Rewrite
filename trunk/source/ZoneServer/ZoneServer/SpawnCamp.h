#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

class ZoneServer;
class Spawn;

class SpawnCamp {
public:
	SpawnCamp(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid);

	void Initialize();
	void Process();
	void Destroy();
	void Reset();


	float GetX() { return m_x; }
	float GetY() { return m_y; }
	float GetZ() { return m_z; }
	uint32_t GetGridID() { return m_grid; }

	float GetRadius() { return m_radius; }
	void SetRadius(float val) { m_radius = val; }
	uint16_t  GetNumRadiusEncounters() { return m_numRadiusEncounters; }
	void SetNumRadiusEncounter(uint16_t val) { m_numRadiusEncounters = val; }

	void AddSpawnID(uint32_t id) { m_spawnIDs.push_back(id); }

protected:

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

	// list of spawns this camp has in the world
	std::vector<std::weak_ptr<Spawn> > m_spawns;
};