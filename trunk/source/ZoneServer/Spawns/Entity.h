#pragma once

#include "Spawn.h"

class Entity : public Spawn {
public:
	Entity() { };
	Entity(std::shared_ptr<Entity> in) : Spawn(in) { }

	bool IsNPC() { return (GetInfoStruct()->entityFlags & EntityIsNpc) != 0; }
private:

};