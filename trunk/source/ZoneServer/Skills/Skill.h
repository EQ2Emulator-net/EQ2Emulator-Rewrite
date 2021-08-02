#pragma once

#include <string>
#include <cstdint>

class Skill {
public:
	Skill() : skillID(0), skillType(0), bDisplay(false) {}
	~Skill() = default;

	std::string shortName;
	std::string name;
	std::string description;
	uint32_t skillID;
	uint32_t skillType;
	bool bDisplay;
};