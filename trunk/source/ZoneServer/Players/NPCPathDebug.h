#pragma once

#include <memory>
#include <vector>

#include "../AI/MovementLocationInfo.h"

class Client;
class Spawn;
class PathPointSpawn;

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
	void RemovePathPoint(std::shared_ptr<PathPointSpawn> spawn);
	void GenerateLua();

	void SetPathPointSpeed(std::shared_ptr<PathPointSpawn> spawn, float speed);
	void SetPathPointDelay(std::shared_ptr<PathPointSpawn> spawn, float delay);
	void SetPathPointCallback(std::shared_ptr<PathPointSpawn> spawn, std::string callback);

private:

	std::shared_ptr<PathPointSpawn> CreateLocationSpawn(std::shared_ptr<MovementLocationInfo> location, uint32_t pointIndex);
	void SendClientMessage(std::string message, std::string filter = "Command Text");
	void UpdatePathPointSpawn(std::shared_ptr<PathPointSpawn> spawn, std::shared_ptr<MovementLocationInfo> location);
	std::string to_string_with_precision(float a_value, const int n = 2);

	std::weak_ptr<Client> m_client;
	std::weak_ptr<Spawn> m_targetSpawn;
	std::vector<std::shared_ptr<MovementLocationInfo> > m_locations;
	std::vector<std::shared_ptr<PathPointSpawn> > m_locationSpawns;
};