#pragma once

#include "MovementLocationInfo.h"

class Entity;

class NPCMovement {
public:

	NPCMovement();

	void AddLocation(std::shared_ptr<MovementLocationInfo> loc);
	void RemoveLocations();
	std::vector<std::shared_ptr<MovementLocationInfo> > GetLocations() { return m_locations; }

	void Process(std::shared_ptr<Entity> spawn);
	

private:
	// Actual movement calculations are here
	void CalculateChange(std::shared_ptr<Entity> spawn);

	std::vector<std::shared_ptr<MovementLocationInfo> > m_locations;
	uint32_t m_lastMovementUpdateTimestamp;
	uint32_t m_movementLoopIndex;
};