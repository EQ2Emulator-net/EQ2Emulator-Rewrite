#include "stdafx.h"

#include "Cell.h"
#include "../Spawns/Spawn.h"
#include "../ZoneServer/Client.h"
#include "../ZoneServer/ZoneServer.h"
#include "../../common/log.h"

Cell::Cell(std::pair<int32_t, int32_t> cellCoordinates) {
	m_cellCoordinates = cellCoordinates;
	bIsActive = false;
}

Cell::~Cell() {

}

void Cell::Process() {
	std::vector<std::weak_ptr<Spawn> >::iterator itr;
	for (itr = m_spawnList.begin(); itr != m_spawnList.end(); ) {
		std::shared_ptr<Spawn> spawn = (*itr).lock();
		if (spawn) {
			spawn->Process();
			itr++;
		}
		else
			itr = m_spawnList.erase(itr);
	}
}

void Cell::AddSpawn(std::shared_ptr<Spawn> spawn) {
	m_spawnList.push_back(spawn);
	LogDebug(LOG_ZONE, 7, "Adding spawn (%s) to cell (%i, %i)", spawn->GetName().c_str(), m_cellCoordinates.first, m_cellCoordinates.second);
}

void Cell::RemoveSpawn(std::shared_ptr<Spawn> spawn) {
	std::vector<std::weak_ptr<Spawn> >::iterator itr;
	for (itr = m_spawnList.begin(); itr != m_spawnList.end(); ) {
		if (EmuWeakCmp((*itr), spawn)) {
			itr = m_spawnList.erase(itr);
			LogDebug(LOG_ZONE, 7, "Removing spawn (%s) from cell (%i, %i)", spawn->GetName().c_str(), m_cellCoordinates.first, m_cellCoordinates.second);
		}
		else
			itr++;
	}
}

void Cell::ActivateCell(std::shared_ptr<Client> client) {
	bIsActive = true;

	LogDebug(LOG_ZONE, 0, "Activating cell (%i, %i) for client (%u)", m_cellCoordinates.first, m_cellCoordinates.second, client->GetAccountID());
	for (std::weak_ptr<Spawn> s : m_spawnList) {
		std::shared_ptr<Spawn> spawn = s.lock();
		if (spawn) {
			std::shared_ptr<ZoneServer> zone = client->GetZone();
			if (zone)
				zone->SendSpawnToClient(spawn, client);
		}
	}
}

void Cell::SendRemoveSpawnsForClient(std::shared_ptr<Client> client) {
	for (std::weak_ptr<Spawn> s : m_spawnList) {
		std::shared_ptr<Spawn> spawn = s.lock();
		if (spawn) {
			std::shared_ptr<ZoneServer> zone = client->GetZone();
			if (zone)
				zone->RemoveSpawnFromClient(spawn, client);
		}
	}
}

void Cell::DeactivateCell() {
	bIsActive = false;
	LogDebug(LOG_ZONE, 0, "Deactivating cell (%i, %i)", m_cellCoordinates.first, m_cellCoordinates.second);
}

void Cell::ClearSpawnList() {
	m_spawnList.clear();
}