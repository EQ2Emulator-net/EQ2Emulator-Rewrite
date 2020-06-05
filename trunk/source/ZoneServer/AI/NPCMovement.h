#pragma once

#include "MovementLocationInfo.h"

class NPCMovement {
public:

	void AddLocation(std::shared_ptr<MovementLocationInfo> loc);
	void RemoveLocations();
	std::vector<std::shared_ptr<MovementLocationInfo> >* GetLocations() { return &m_locations; }

private:

	std::vector<std::shared_ptr<MovementLocationInfo> > m_locations;
};

/*
Possible model ID's to visualize the path

273 - willowisp, floats around in the air
2681 - crystal ball on the floor

*/