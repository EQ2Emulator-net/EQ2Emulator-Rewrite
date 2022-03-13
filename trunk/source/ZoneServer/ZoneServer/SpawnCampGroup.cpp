#include "stdafx.h"

#include "SpawnCamp.h"
#include "../Spawns/SpawnGroupList.h"
#include "../Spawns/Spawn.h"

void SpawnCampGroup::Process() {
	if (m_spawnEntries.size() == 0)
		return;

	if (m_locationEntries.size() == 0)
		return;

	if (m_spawns.size() > 0)
		return;

	uint32_t numToSpawn = 0;
	if (m_minSpawns >= m_maxSpawns)
		numToSpawn = m_minSpawns;
	else
		numToSpawn = MakeRandomInt(m_minSpawns, m_maxSpawns);

	std::vector<std::shared_ptr<Spawn> > spawnsToSend;
	std::vector<std::shared_ptr<SpawnCampLocationEntry> > availableLocations = m_locationEntries;
	for (int i = 0; i < numToSpawn; i++) {

		uint32_t spawnID = 0;
		uint32_t roll = MakeRandomInt(1, m_totalWeight);
		LogWarn(LOG_NPC, 0, "roll = %u, total weight = %u", roll, m_totalWeight);

		std::map<uint32_t, std::shared_ptr<SpawnCampSpawnEntry> >::iterator itr;
		for (itr = m_spawnEntries.begin(); itr != m_spawnEntries.end(); itr++) {
			if (roll <= itr->second->weight) {
				spawnID = itr->first;
				break;
			}

			roll -= itr->second->weight;
		}

		std::shared_ptr<SpawnCampLocationEntry> scle;
		uint32_t index = MakeRandomInt(0, availableLocations.size() - 1);
		scle = availableLocations[index];
		availableLocations.erase(availableLocations.begin() + index);

		std::shared_ptr<Spawn> spawn = CreateSpawn(spawnID, scle);
		spawnsToSend.push_back(spawn);

		if (availableLocations.size() == 0)
			break;
	}

	std::vector<std::shared_ptr<Spawn> >::iterator itr;
	for (itr = spawnsToSend.begin(); itr != spawnsToSend.end(); itr++)
		AddSpawnToZone(*itr);
}

std::shared_ptr<Spawn> SpawnCampGroup::CreateSpawn(uint32_t id, std::shared_ptr<SpawnCampLocationEntry> scle) {
	std::shared_ptr<Spawn> spawn = SpawnCamp::CreateSpawn(id, scle);

	if (spawn) {
		std::shared_ptr<SpawnGroupList> group_list;
		if (m_spawns.size() == 1) {
			group_list = std::make_shared<SpawnGroupList>();
		}
		else {
			std::shared_ptr<Spawn> spawn2 = m_spawns[0].lock();
			if (spawn2) {
				group_list = spawn2->GetSpawnGroupList();
			}
		}

		spawn->SetSpawnGroupID(10000000 + GetID());
		group_list->AddSpawn(spawn);
		spawn->SetSpawnGroupList(group_list);
	}

	return spawn;
}