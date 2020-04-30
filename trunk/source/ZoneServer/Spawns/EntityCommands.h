#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <map>
#include "../../common/Mutex.h"

struct EntityCommand {
	std::string commandText;
	std::string command;
	std::string errorText;
	float distance;
	uint32_t spellVisual;
	uint16_t castTime;
};

struct EntityCommandList {
	std::vector<EntityCommand> commands;
};

class MasterEntityCommandList {
public:
	MasterEntityCommandList() = default;
	~MasterEntityCommandList() = default;

	std::shared_ptr<const EntityCommandList> GetListByID(uint32_t id);
	void ProcessDBResult(class DatabaseResult& result);

private:
	Mutex masterList_lock;
	std::map<uint32_t, std::shared_ptr<EntityCommandList> > masterList;
};