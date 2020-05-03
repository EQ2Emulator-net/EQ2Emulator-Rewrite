#pragma once

#include "Spawn.h"

class GroundSpawn : public Spawn {
public:
	GroundSpawn() {
		m_groundSpawnDatabaseID = 0;
		m_numberOfHarvests = 0;
		m_numberAttemptsPerHarvest = 0;
		m_groundSpawnID = 0;
		SetState(129, false);
	}

	GroundSpawn(std::shared_ptr<GroundSpawn> in) : Spawn(in) {
		m_groundSpawnDatabaseID = in->GetGroundSpawnDatabaseID();
		m_numberOfHarvests = in->GetNumberOfHarvests();
		m_numberAttemptsPerHarvest = in->GetNumberAttemptsPerHarvest();
		m_groundSpawnID = in->GetGroundSpawnID();
		SetMoveType(0.5f, false);
		SetState(in->GetPosStruct()->positionState, false);
	}

	bool IsGroundSpawn() override { return true; }
	void SetGroundSpawnDatabaseID(uint32_t val) { m_groundSpawnDatabaseID = val; }
	uint32_t GetGroundSpawnDatabaseID() { return m_groundSpawnDatabaseID; }
	void SetNumberOfHarvests(uint8_t val) { m_numberOfHarvests = val; }
	uint8_t GetNumberOfHarvests() { return m_numberOfHarvests; }
	void SetNumberAttemptsPerHarvets(uint8_t val) { m_numberAttemptsPerHarvest = val; }
	uint8_t GetNumberAttemptsPerHarvest() { return m_numberAttemptsPerHarvest; }
	void SetGroundSpawnID(uint32_t val) { m_groundSpawnID = val; }
	uint32_t GetGroundSpawnID() { return m_groundSpawnID; }
	void SetCollectionSkill(std::string val) { m_collectionSkill = val; }
	std::string GetCollectionSkill() { return m_collectionSkill; }

private:
	uint32_t m_groundSpawnDatabaseID;
	uint8_t m_numberOfHarvests;
	uint8_t m_numberAttemptsPerHarvest;
	uint32_t m_groundSpawnID;
	std::string m_collectionSkill;
};