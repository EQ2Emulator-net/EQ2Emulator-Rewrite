#pragma once

#include <memory>
#include <vector>

class Spawn;

class SpawnGroupList {
public:

	void AddSpawn(std::shared_ptr<Spawn> spawn) {
		m_spawnList.push_back(spawn);
	}

	std::vector<std::weak_ptr<Spawn> >* GetSpawnList() { return &m_spawnList; }

private:
	std::vector<std::weak_ptr<Spawn> > m_spawnList;
};