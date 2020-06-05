#pragma once

#include <stdint.h>
#include <string>

struct MovementLocationInfo {
public:
	float x;
	float y;
	float z;
	float speed;
	float delay;
	std::string callback;
};