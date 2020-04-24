#pragma once

#include "Spawn.h"

class Entity : public Spawn {
public:
	bool IsNPC() { return (GetInfoStruct()->entityFlags & EntityIsNpc) != 0; }
private:

};