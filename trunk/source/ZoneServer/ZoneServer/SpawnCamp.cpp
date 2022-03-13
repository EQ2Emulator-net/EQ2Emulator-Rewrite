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
	m_totalWeight = 0;
}

void SpawnCamp::Process() {
	// no spawn entires, nothing to do
	if (m_spawnEntries.size() == 0)
		return;

	std::shared_ptr<SpawnCampLocationEntry> scle;
	if (m_locationEntries.size() == 0) {
		scle = std::make_shared<SpawnCampLocationEntry>();
		scle->x = m_x;
		scle->y = m_y;
		scle->z = m_z;
		scle->x_offset = 0;
		scle->y_offset = 0;
		scle->z_offset = 0;
		scle->heading = 0;
		scle->pitch = 0;
		scle->roll = 0;
		scle->grid = m_grid;
		scle->visual_state_override = 0;
		scle->action_state_override = 0;

		m_locationEntries.push_back(scle);
	}
	else {
		scle = m_locationEntries[0];
	}

	// no spawns in the zone
	if (m_spawns.size() == 0) {
		// TODO respawn check

		if (m_spawnEntries.size() > 1) {
			uint32_t roll = MakeRandomInt(1, m_totalWeight);
			LogWarn(LOG_NPC, 0, "roll = %u, total weight = %u", roll, m_totalWeight);
			uint32_t spawnID = 0;

			std::map<uint32_t, std::shared_ptr<SpawnCampSpawnEntry> >::iterator itr;
			for (itr = m_spawnEntries.begin(); itr != m_spawnEntries.end(); itr++) {
				if (roll <= itr->second->weight) {
					spawnID = itr->first;
					break;
				}

				roll -= itr->second->weight;
			}

			if (spawnID > 0) {
				std::shared_ptr<Spawn> spawn = CreateSpawn(spawnID, scle);
				AddSpawnToZone(spawn);
			}
		}
		else {
			std::shared_ptr<Spawn> spawn = CreateSpawn(m_spawnEntries.begin()->first, scle);
			AddSpawnToZone(spawn);
		}
	}
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
}

void SpawnCamp::AddSpawn(std::shared_ptr<SpawnCampSpawnEntry> scse) {
	m_spawnEntries[scse->spawnDBID] = scse;
	m_totalWeight += scse->weight;
}

std::shared_ptr<Spawn> SpawnCamp::CreateSpawn(uint32_t id, std::shared_ptr<SpawnCampLocationEntry> scle) {
	std::shared_ptr<ZoneServer> zone = GetZone();
	if (zone) {
		std::shared_ptr<Spawn> spawn = zone->GetNewSpawnFromMasterList(id);
		if (spawn) {
			// TODO: x/y/z offset
			spawn->SetLocation(scle->x, scle->y, scle->z, false);
			spawn->SetHeading(scle->heading, false);
			spawn->SetPitch(scle->pitch, false);
			spawn->SetRoll(scle->roll, false);
			spawn->SetGridID(scle->grid, false);

			if (scle->visual_state_override > 0)
				spawn->SetVisualState(scle->visual_state_override, false);
			
			if (scle->action_state_override > 0)
				spawn->SetActionState(scle->action_state_override, false);

			spawn->SetSpawnCamp(shared_from_this());
			spawn->SetSpawnLocationID(scle->id);
			
			m_spawns.push_back(spawn);
			return spawn;
		}
	}

	return nullptr;
}

void SpawnCamp::AddSpawnToZone(std::shared_ptr<Spawn> spawn) {
	if (!spawn)
		return;

	std::shared_ptr<ZoneServer> zone = GetZone();
	if (!zone)
		return;

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

	zone->AddSpawn(spawn, type);
}