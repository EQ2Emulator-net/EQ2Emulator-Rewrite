#pragma once

#include "MovementLocationInfo.h"

class Spawn;

class NPCMovement {
public:

	NPCMovement();

	void AddLocation(std::shared_ptr<MovementLocationInfo> loc);
	void RemoveLocations();
	std::vector<std::shared_ptr<MovementLocationInfo> > GetLocations() { return m_locations; }

	void Process(std::shared_ptr<Spawn> spawn);
	
	void UpdateMovementTimestamp();

	void SetFollowTarget(std::shared_ptr<Spawn> spawn) { m_followTarget = spawn; }
	void ClearFollowTarget() { m_followTarget.reset(); }

private:
	// Actual movement calculations are here
	void CalculateChange(std::shared_ptr<Spawn> spawn);

	std::vector<std::shared_ptr<MovementLocationInfo> > m_locations;
	uint32_t m_lastMovementUpdateTimestamp;
	uint32_t m_movementLoopIndex;
	uint32_t m_delayMovementUntilServerTime;
	std::weak_ptr<Spawn> m_followTarget;
};