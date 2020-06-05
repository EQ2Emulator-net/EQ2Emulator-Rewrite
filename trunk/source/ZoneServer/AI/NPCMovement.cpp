#include "stdafx.h"

#include "NPCMovement.h"
#include "../../common/log.h"

void NPCMovement::AddLocation(std::shared_ptr<MovementLocationInfo> loc) {
	m_locations.push_back(loc);
}

void NPCMovement::RemoveLocations() {
	m_locations.clear();
}