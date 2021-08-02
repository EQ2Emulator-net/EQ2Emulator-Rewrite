#include "stdafx.h"

#include "SpawnCamp.h"
#include "ZoneServer.h"
#include "../../common/log.h"
#include "../Spawns/Spawn.h"

SpawnCamp::SpawnCamp(std::shared_ptr<ZoneServer> zone, float x, float y, float z, uint32_t grid) {
	m_zone = zone;
	m_x = x;
	m_y = y;
	m_z = z;
	m_grid = grid;
	bInitialized = false;
	m_radius = 0.0f;
	m_numRadiusEncounters = 0;
}

void SpawnCamp::Initialize() {
	std::shared_ptr<ZoneServer> zone = m_zone.lock();
	if (!zone) {
		LogError(LOG_ZONE, 0, "SpawnCamp::Initialize() failed to get a valid zone...");
		return;
	}

	// Process the radius spawns
	if (m_spawnIDs.size() > 0) {
		for (uint16_t count = 0; count < m_numRadiusEncounters; ) {
			// maybe this will work?
			// r1, r2 was extracted for debugging
			float r1 = MakeRandom(0, 1);
			float r2 = MakeRandom(0, 1);
			float a = static_cast<float>(r1 * 2 * 3.1415926535897932384626433832795);
			float r = m_radius * sqrtf(r2);
			float x = m_x + (r * cos(a));
			float z = m_z + (r * sin(a));
			//LogDebug(LOG_ZONE, 0, "Random r1 = %f, r2 = %f)", r1, r2);
			

			//float x = MakeRandom(m_x - m_radius, m_x + m_radius);
			//float z = MakeRandom(m_z - m_radius, m_z + m_radius);
			uint32_t id = m_spawnIDs.at(MakeRandomInt(0, static_cast<int32_t>(m_spawnIDs.size() - 1)));

			std::shared_ptr<Spawn> spawn = zone->GetNewSpawnFromMasterList(id);
			if (spawn) {

				spawn->SetX(x, false);
				spawn->SetY(m_y, false);
				spawn->SetZ(z, false);
				spawn->SetGridID(m_grid, false);

				// maybe turn this type if into a function on spawn?
				SpawnEntryType type;
				if (spawn->IsSign())
					type = SpawnEntryType::ESIGN;
				else if (spawn->IsWidget())
					type = SpawnEntryType::EWIDGET;
				else if (spawn->IsGroundSpawn())
					type = SpawnEntryType::EGROUNDSPAWN;
				else if (spawn->IsObject())
					type = SpawnEntryType::EOBJECT;
				else
					type = SpawnEntryType::ENPC;

				// Add the spawn to the world
				zone->AddSpawn(spawn, type);
				m_spawns.push_back(spawn);

				count++;
			}
		}
	}

	// Process the location spawns

	bInitialized = true;
}

void SpawnCamp::Process() {
	if (!bInitialized) {
		Initialize();
		return;
	}

	// Do what spawn camps need to do like respawns, expire timers, and so on
}

void SpawnCamp::Destroy() {
	std::shared_ptr<ZoneServer> zone = m_zone.lock();
	if (!zone) {
		LogError(LOG_ZONE, 0, "SpawnCamp::Destroy() failed to get a valid zone...");
	}

	for (std::weak_ptr<Spawn> s : m_spawns) {
		std::shared_ptr<Spawn> spawn = s.lock();
		if (spawn)
			zone->RemoveSpawn(spawn);
	}

	m_spawns.clear();
}

void SpawnCamp::Reset() {
	Destroy();
	bInitialized = false;
}