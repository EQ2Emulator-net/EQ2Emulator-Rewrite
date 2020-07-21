#pragma once

#include <memory>
#include <vector>

#include "../AI/MovementLocationInfo.h"

class Client;
class Spawn;

class NPCPathDebug {
public:
	NPCPathDebug(std::shared_ptr<Client> client) {
		m_client = client;
	}

	bool Process();
	void Start(std::shared_ptr<Spawn> target);
	bool GenerateSpawns();
	void End();
	void SendLocationSpawnsToClient();
	void RemoveLocationsFromClient();
	void ClearLocations();
	void AddPathPoint();
	void RemovePathPoint(std::shared_ptr<Spawn> spawn);

private:

	std::shared_ptr<Spawn> CreateLocationSpawn(std::shared_ptr<MovementLocationInfo> location, uint32_t pointIndex);
	void SendClientMessage(std::string message, std::string filter = "Command Text");
	uint32_t GetIndexForSpawn(std::shared_ptr<Spawn> spawn);

	std::weak_ptr<Client> m_client;
	std::weak_ptr<Spawn> m_targetSpawn;
	std::vector<std::shared_ptr<MovementLocationInfo> >* m_locations;
	std::vector<std::shared_ptr<Spawn> > m_locationSpawns;
};