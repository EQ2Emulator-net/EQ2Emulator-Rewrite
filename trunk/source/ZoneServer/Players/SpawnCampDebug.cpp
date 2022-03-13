#include "stdafx.h"

#include "SpawnCampDebug.h"
#include "../ZoneServer/Client.h"
#include "../Spawns/SpawnCampSpawn.h"
#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/SpawnCamp.h"

bool SpawnCampDebug::Process() {
	bool ret = true;

	for (std::shared_ptr<Spawn> spawn : m_spawnCampSpawns) {
		spawn->Process();
	}

	if (m_radiusSpawn)
		m_radiusSpawn->Process();

	return ret;
}

bool SpawnCampDebug::GenerateSpawns() {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client) {
		LogError(LOG_ZONE, 0, "SpawnCampDebug::GenerateSpawns() unable to get a valid client...");
		return false;
	}

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone) {
		LogError(LOG_ZONE, 0, "SpawnCampDebug::GenerateSpawns() unable to get a valid zone...");
		return false;
	}

	std::map<uint32_t, std::shared_ptr<SpawnCamp> >* camps = zone->GetSpawnCamps();
	for (std::pair<uint32_t, std::shared_ptr<SpawnCamp> > camp : *camps) {
		std::shared_ptr<SpawnCampSpawn> spawn = CreateSpawnCampSpawn(camp.second);
		m_spawnCampSpawns.push_back(spawn);
	}

	return true;
}

std::shared_ptr<SpawnCampSpawn> SpawnCampDebug::CreateSpawnCampSpawn(std::shared_ptr<SpawnCamp> camp) {
	std::shared_ptr<SpawnCampSpawn> ret = std::make_shared<SpawnCampSpawn>(camp);

	ret->SetModelType(2467, false);
	ret->SetLocation(camp->GetX(), camp->GetY(), camp->GetZ(), false);
	ret->SetGridID(camp->GetGridID(), false);

	std::string name = "Spawn Camp";
	ret->SetName(name, false);
	ret->SetGuild(camp->GetName() + " (" + ::to_string(camp->GetID()) + ")");
	ret->SetSize(0.5f, false);
	ret->SetSizeRatio(1.0f, false);
	ret->SetCollisionRadius(1.0f, false);
	ret->SetState(16384, false);
	ret->SetShowLevel(false);

	return ret;
}

void SpawnCampDebug::Show() {
	// Already showing the camps so return out
	if (bShow)
		return;

	if (GenerateSpawns()) {
		for (std::shared_ptr<SpawnCampSpawn> spawn : m_spawnCampSpawns) {
			std::shared_ptr<Client> client = m_client.lock();
			if (client) {
				std::shared_ptr<ZoneServer> zone = client->GetZone();
				if (zone) {
					zone->SendSpawnToClient(spawn, client);
				}
			}
		}
	}

	bShow = true;
}

void SpawnCampDebug::AddSpawnCamp(std::shared_ptr<SpawnCamp> camp) {
	if (!bShow)
		return;

	std::shared_ptr<SpawnCampSpawn> spawn = CreateSpawnCampSpawn(camp);
	if (spawn) {
		m_spawnCampSpawns.push_back(spawn);
		std::shared_ptr<Client> client = m_client.lock();
		if (client) {
			std::shared_ptr<ZoneServer> zone = client->GetZone();
			if (zone) {
				zone->SendSpawnToClient(spawn, client);
			}
		}
	}
}

void SpawnCampDebug::ShowSpawnCampRadius(std::shared_ptr<SpawnCamp> camp) {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client) {
		LogError(LOG_ZONE, 0, "SpawnCampDebug::ShowSpawnCampRadius() unable to get a valid client...");
		return;
	}

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone) {
		LogError(LOG_ZONE, 0, "SpawnCampDebug::ShowSpawnCampRadius() unable to get a valid zone...");
		return;
	}

	if (m_radiusSpawn)
		zone->SendDestroyGhost(client, m_radiusSpawn);

	m_radiusSpawnCamp = camp;
	m_radiusSpawn = std::make_shared<Spawn>();

	m_radiusSpawn->SetModelType(5015, false);
	m_radiusSpawn->SetLocation(camp->GetX(), camp->GetY(), camp->GetZ(), false);
	m_radiusSpawn->SetGridID(camp->GetGridID(), false);

	std::string name = "Spawn Camp Radius";
	m_radiusSpawn->SetName(name, false);
	//m_radiusSpawn->SetSize(camp->GetRadius() / 20.0f, false); // divide by 20 as the duel flag ring has a radius of 20 at size 1.0
	m_radiusSpawn->SetSizeRatio(1.0f, false);
	m_radiusSpawn->SetCollisionRadius(1.0f, false);
	m_radiusSpawn->SetState(16384, false);
	m_radiusSpawn->SetShowLevel(false);

	zone->SendSpawnToClient(m_radiusSpawn, client);
}

void SpawnCampDebug::UpdateSpawnCampRadius(std::shared_ptr<SpawnCamp> camp) {
	if (!EmuWeakCmp(m_radiusSpawnCamp, camp))
		return;

	//m_radiusSpawn->SetSize(camp->GetRadius() / 20.0f);
}