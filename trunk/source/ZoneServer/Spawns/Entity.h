#pragma once

#include "Spawn.h"

class Entity : public Spawn {
public:
	Entity() { };
	Entity(std::shared_ptr<Entity> in) : Spawn(in) {
		SetSwimmingSpeedMultiplier(0.78125f, false);
	}

private:

};