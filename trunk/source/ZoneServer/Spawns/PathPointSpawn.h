#pragma once

#include "Spawn.h"

class PathPointSpawn : public Spawn {
public:
	PathPointSpawn(uint32_t index) : Spawn() {
		m_pointIndex = index;
	}

	bool IsPathPoint() override { return true; }
	uint32_t GetPointIndex() { return m_pointIndex; }

private:
	uint32_t m_pointIndex;
};