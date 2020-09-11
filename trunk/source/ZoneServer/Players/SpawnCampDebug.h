#pragma once

#include <vector>

class Client;
class SpawnCampSpawn;
class SpawnCamp;
class Spawn;

class SpawnCampDebug {
public:
	SpawnCampDebug(std::shared_ptr<Client> client) {
		m_client = client;
		bShow = false;
	}

	bool Process();
	void Show();

	void AddSpawnCamp(std::shared_ptr<SpawnCamp> camp);

	void ShowSpawnCampRadius(std::shared_ptr<SpawnCamp> camp);
	void UpdateSpawnCampRadius(std::shared_ptr<SpawnCamp> camp);

private:
	bool GenerateSpawns();
	std::shared_ptr<SpawnCampSpawn> CreateSpawnCampSpawn(std::shared_ptr<SpawnCamp> camp);

	bool bShow;
	std::weak_ptr<Client> m_client;
	std::vector<std::shared_ptr<SpawnCampSpawn> > m_spawnCampSpawns;
	std::shared_ptr<Spawn> m_radiusSpawn;
	std::weak_ptr<SpawnCamp> m_radiusSpawnCamp;
};