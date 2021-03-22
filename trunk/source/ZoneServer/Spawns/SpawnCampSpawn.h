#pragma once

#include "Spawn.h"
#include "../ZoneServer/SpawnCamp.h"

class SpawnCampSpawn : public Spawn {
public:
	SpawnCampSpawn(std::shared_ptr<SpawnCamp> camp) {
		m_spawnCamp = camp;
	}

	bool IsSpawnCampSpawn() override { return true; }
	std::shared_ptr<SpawnCamp> GetSpawnCamp() { return m_spawnCamp.lock(); }

private:
	std::weak_ptr<SpawnCamp> m_spawnCamp;
};