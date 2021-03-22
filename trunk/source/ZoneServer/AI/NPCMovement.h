#pragma once

#include "MovementLocationInfo.h"

class NPCMovement {
public:

	void AddLocation(std::shared_ptr<MovementLocationInfo> loc);
	void RemoveLocations();
	std::vector<std::shared_ptr<MovementLocationInfo> > GetLocations() { return m_locations; }

private:

	std::vector<std::shared_ptr<MovementLocationInfo> > m_locations;
};