#include "stdafx.h"

#include "NPCMovement.h"
#include "../../common/log.h"
#include "../Spawns/Entity.h"
#include "../../common/timer.h"

NPCMovement::NPCMovement() {
	m_lastMovementUpdateTimestamp = Timer::GetServerTime();
	m_movementLoopIndex = 0;
}
void NPCMovement::AddLocation(std::shared_ptr<MovementLocationInfo> loc) {
	m_locations.push_back(loc);
}

void NPCMovement::RemoveLocations() {
	m_locations.clear();
}

void NPCMovement::Process(std::shared_ptr<Entity> spawn) {

	// movement loop
	if (m_locations.size() > 0) {
		std::shared_ptr<MovementLocationInfo> targetLocation = m_locations.at(m_movementLoopIndex);
		if (spawn->GetX() == targetLocation->x && spawn->GetY() == targetLocation->y && spawn->GetZ() == targetLocation->z) {
			m_movementLoopIndex++;
			if (m_movementLoopIndex >= m_locations.size())
				m_movementLoopIndex = 0;

			targetLocation = m_locations.at(m_movementLoopIndex);

			spawn->SetDestLocation(targetLocation->x, targetLocation->y, targetLocation->z);
			spawn->SetSpeed(targetLocation->speed);
		}
		else if (spawn->GetDestinationX() != targetLocation->x || spawn->GetDestinationY() != targetLocation->y || spawn->GetDestinationZ() != targetLocation->z) {
			spawn->SetDestLocation(targetLocation->x, targetLocation->y, targetLocation->z);
			spawn->SetSpeed(targetLocation->speed);
		}

		CalculateChange(spawn);
	}
}

void NPCMovement::CalculateChange(std::shared_ptr<Entity> spawn) {
	// Speed is per second so we need a time_step (amount of time since the last update) to modify movement by
	float delta_time = (Timer::GetServerTime() - m_lastMovementUpdateTimestamp) * 0.001f;
	// update the last timestamp to the current time now that we got the delta time
	m_lastMovementUpdateTimestamp = Timer::GetServerTime();

	// Get current poisiton
	float nx = spawn->GetX();
	float ny = spawn->GetY();
	float nz = spawn->GetZ();

	// Get forward vector
	float vx = spawn->GetDestinationX() - nx;
	float vy = spawn->GetDestinationY() - ny;
	float vz = spawn->GetDestinationZ() - nz;

	// Multiply speed by the delta_time to get how much should have changed over the last tick
	float speed = spawn->GetSpeed() * delta_time;

	// Normalize the forward vector and multiply by speed, this gives us our change in coords, just need to add them to our current coords
	float len = sqrtf(vx * vx + vy * vy + vz * vz);
	vx = (vx / len) * speed;
	vy = (vy / len) * speed;
	vz = (vz / len) * speed;

	// distance is less then or equal to 0.5 then just set the spawn to the new location
	if (spawn->GetDistance(spawn->GetDestinationX(), spawn->GetDestinationY(), spawn->GetDestinationZ(), spawn->IsWidget() ? false : true) <= 1.0f)
		spawn->SetLocation(spawn->GetDestinationX(), spawn->GetDestinationY(), spawn->GetDestinationZ(), false);
	else
		spawn->SetLocation(nx + vx, ny + vy, nz + vz, false);

}