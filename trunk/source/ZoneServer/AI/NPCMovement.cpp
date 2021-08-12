#include "stdafx.h"

#include "NPCMovement.h"
#include "../../common/log.h"
#include "../Spawns/Spawn.h"
#include "../../common/timer.h"

NPCMovement::NPCMovement() {
	m_lastMovementUpdateTimestamp = Timer::GetServerTime();
	m_movementLoopIndex = 0;
	m_delayMovementUntilServerTime = 0;
}
void NPCMovement::AddLocation(std::shared_ptr<MovementLocationInfo> loc) {
	m_locations.push_back(loc);
}

void NPCMovement::RemoveLocations() {
	m_locations.clear();
}

void NPCMovement::Process(std::shared_ptr<Spawn> spawn) {

	// Follow target
	std::shared_ptr<Spawn> FollowTarget = m_followTarget.lock();
	if (FollowTarget) {
		// Get distance between the points
		float distance = spawn->GetDistance(FollowTarget);

		// Get distance between spawns destination (if set) and the follow target
		float distance2 = 0.0f;
		if (spawn->GetDestinationX() != 0 || spawn->GetDestinationY() != 0 || spawn->GetDestinationZ() != 0) {
			distance2 = FollowTarget->GetDistance(spawn->GetDestinationX(), spawn->GetDestinationY(), spawn->GetDestinationZ());
		}

		// Maybe make follow distance a variable on spawn so we can adjust it for larger spawns?
		float follow_distance = 2.0f;

		// if distance is greater then follow distance and distance2 is not set or if set is greater then follow distance
		// then calculate a destination location for the spawn to run to
		if (distance > follow_distance && (distance2 == 0.0f || distance2 > follow_distance)) {

			// Calculate a direction vector (direction is from the follow target to the spawn)
			float dx = spawn->GetX() - FollowTarget->GetX();
			float dy = spawn->GetY() - FollowTarget->GetY();
			float dz = spawn->GetZ() - FollowTarget->GetZ();

			// normalize the direction vector
			dx = dx / distance;
			dy = dy / distance;
			dz = dz / distance;

			// calculate destination by multipling the normalized direction vector by distance we want to be from the follow target
			// then add that result to the location of the follow target, this gives us a point a set amount of distance away from
			// follow target but inbetween follow target and spawn
			float destX = FollowTarget->GetX() + (dx * follow_distance);
			float destY = FollowTarget->GetY() + (dy * follow_distance);
			float destZ = FollowTarget->GetZ() + (dz * follow_distance);

			// set default speed
			if (spawn->GetSpeed() <= 0.0f)
				spawn->SetSpeed(2.0f, false);

			// Set the spawns destination
			spawn->SetDestLocation(destX, destY, destZ, Timer::GetServerTime());
		}

		// if spawn has a destination then do the math to move it on the server
		if (spawn->GetDestinationX() != 0 || spawn->GetDestinationY() != 0 || spawn->GetDestinationZ() != 0) {
			CalculateChange(spawn);
		}
	}

	// MoveTo

	// movement loop
	else if (m_locations.size() > 0) {
		// Get the target location
		std::shared_ptr<MovementLocationInfo> targetLocation = m_locations.at(m_movementLoopIndex);

		//Check if we are at the target location
		if (spawn->GetX() == targetLocation->x && spawn->GetY() == targetLocation->y && spawn->GetZ() == targetLocation->z) {

			// if destination has a delay but dely timestamp is not set then we just arrived so set the timestamp now
			if (targetLocation->delay != 0.0f && m_delayMovementUntilServerTime == 0) {
				m_delayMovementUntilServerTime = Timer::GetServerTime() + (targetLocation->delay * 1000);
			}

			// if there is no delay (timestamp == 0) or we have delayed long enough get the next location
			if (m_delayMovementUntilServerTime == 0 || m_delayMovementUntilServerTime <= Timer::GetServerTime()) {
				// Get the next location
				m_movementLoopIndex++;
				if (m_movementLoopIndex >= m_locations.size())
					m_movementLoopIndex = 0;

				targetLocation = m_locations.at(m_movementLoopIndex);

				// if next location does not have a delay get the location after that for destination 2 values
				if (targetLocation->delay == 0.0f) {
					uint32_t nextIndex = m_movementLoopIndex + 1;
					if (nextIndex >= m_locations.size())
						nextIndex = 0;

					std::shared_ptr<MovementLocationInfo> targetLocation2 = m_locations.at(nextIndex);
					spawn->SetDest2Location(targetLocation2->x, targetLocation2->y, targetLocation2->z, false);
				}
				else {
					// Next location does have a delay so zero out destination 2 values
					spawn->SetDest2Location(0.0f, 0.0f, 0.0f, false);
				}

				// set the target location as the destination
				spawn->SetDestLocation(targetLocation->x, targetLocation->y, targetLocation->z, Timer::GetServerTime());

				// Set the speed based on targetLocation desired speed
				spawn->SetSpeed(targetLocation->speed);

				// reset the delay timestamp
				m_delayMovementUntilServerTime = 0;
			}
		}
		// This sets up the first location after a spawn spawns
		else if (spawn->GetDestinationX() != targetLocation->x || spawn->GetDestinationY() != targetLocation->y || spawn->GetDestinationZ() != targetLocation->z) {
			if (targetLocation->delay == 0.0f) {
				uint32_t nextIndex = m_movementLoopIndex + 1;
				if (nextIndex >= m_locations.size())
					nextIndex = 0;

				std::shared_ptr<MovementLocationInfo> targetLocation2 = m_locations.at(nextIndex);
				spawn->SetDest2Location(targetLocation2->x, targetLocation2->y, targetLocation2->z, false);
			}
			else {
				spawn->SetDest2Location(0.0f, 0.0f, 0.0f, false);
			}

			spawn->SetDestLocation(targetLocation->x, targetLocation->y, targetLocation->z, Timer::GetServerTime());
			spawn->SetSpeed(targetLocation->speed);
		}

		// finally do the math to move the spawn on the server
		CalculateChange(spawn);
	}

	// update the last timestamp to the current time
	m_lastMovementUpdateTimestamp = Timer::GetServerTime();
}

void NPCMovement::CalculateChange(std::shared_ptr<Spawn> spawn) {
	// Speed is per second so we need a time_step (amount of time since the last update) to modify movement by
	float delta_time = (Timer::GetServerTime() - m_lastMovementUpdateTimestamp) * 0.001f;

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

void NPCMovement::UpdateMovementTimestamp() {
	m_lastMovementUpdateTimestamp = Timer::GetServerTime();
}