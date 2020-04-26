#pragma once

#include "Spawn.h"

class GroundSpawn : public Spawn {
public:

	void SetGroundSpawnDatabaseID(uint32_t val) { m_groundSpawnDatabaseID = val; }
	void SetNumberOfHarvests(uint8_t val) { m_numberOfHarvests = val; }
	void SetNumberAttemptsPerHarvets(uint8_t val) { m_numberAttemptsPerHarvest = val; }
	void SetGroundSpawnID(uint32_t val) { m_groundSpawnID = val; }
	void SetCollectionSkill(std::string val) { m_collectionSkill = val; }

private:
	uint32_t m_groundSpawnDatabaseID;
	uint8_t m_numberOfHarvests;
	uint8_t m_numberAttemptsPerHarvest;
	uint32_t m_groundSpawnID;
	std::string m_collectionSkill;
};