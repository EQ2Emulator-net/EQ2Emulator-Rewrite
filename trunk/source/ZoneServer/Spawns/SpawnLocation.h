#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <memory>

enum class SpawnEntryType {
	ENPC = 0,
	EOBJECT = 1,
	EWIDGET = 2,
	ESIGN = 3,
	EGROUNDSPAWN = 4
};

struct SpawnEntry {
	uint32_t		spawn_entry_id;
	uint32_t		spawn_location_id;
	SpawnEntryType	spawn_type;
	uint32_t		spawn_id;
	float			spawn_percentage;
	uint32_t		condition;
	uint32_t        scriptID;
};

class SpawnLocation {
public:
	SpawnLocation() {
		placement_id = 0;
		x = 0;
		y = 0;
		z = 0;
		x_offset = 0;
		y_offset = 0;
		z_offset = 0;
		heading = 0;
		pitch = 0;
		roll = 0;
		respawn = 0;
		respawn_offset = 0;
		expire_time = 0;
		expire_offset = 0;
		grid_id = 0;
		total_percentage = 0;
		scriptID = 0;
	}

	~SpawnLocation() {
		entries.clear();
	}

	void AddEntry(std::shared_ptr<SpawnEntry> entry) { entries.push_back(entry); }

	std::vector<std::shared_ptr<SpawnEntry> > entries;
	uint32_t	placement_id;
	std::string	name;
	float		x;
	float		y;
	float		z;
	float		x_offset;
	float		y_offset;
	float		z_offset;
	float		heading;
	float		pitch;
	float		roll;
	uint32_t	respawn;
	uint32_t	respawn_offset;
	uint32_t	expire_time;
	uint32_t	expire_offset;
	uint32_t	grid_id;
	std::string	script;
	float		total_percentage;
	uint32_t    scriptID;
};