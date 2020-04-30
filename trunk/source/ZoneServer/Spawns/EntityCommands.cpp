#include "stdafx.h"

#include "EntityCommands.h"
#include "../../common/database-result.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/log.h"

std::shared_ptr<const EntityCommandList> MasterEntityCommandList::GetListByID(uint32_t id) {
	ReadLocker lock(masterList_lock);
	auto itr = masterList.find(id);
	if (itr != masterList.end()) {
		return itr->second;
	}

	return std::shared_ptr<const EntityCommandList>();
}

bool ZoneDatabase::LoadEntityCommands(MasterEntityCommandList& masterList) {
	DatabaseResult result;
	bool success = Select(&result, "SELECT command_list_id, command_text, distance, command, error_text, cast_time, spell_visual\n"
		"FROM entity_commands;");

	if (!success) {
		LogError(LOG_DATABASE, 0, "Error loading entity commands!");
		return success;
	}

	masterList.ProcessDBResult(result);

	return success;
}

void MasterEntityCommandList::ProcessDBResult(DatabaseResult& result) {
	std::map<uint32_t, std::shared_ptr<EntityCommandList> > newList;

	while (result.Next()) {
		std::shared_ptr<EntityCommandList>& cmdList = newList[result.GetUInt32(0)];

		if (!cmdList) {
			cmdList = std::make_shared<EntityCommandList>();
		}

		cmdList->commands.emplace_back();
		EntityCommand& newCommand = cmdList->commands.back();

		newCommand.commandText = result.GetString(1);
		newCommand.distance = result.GetFloat(2);
		newCommand.command = result.GetString(3);
		newCommand.errorText = result.GetString(4);
		newCommand.castTime = result.GetUInt16(5);
		newCommand.spellVisual = result.GetUInt32(6);
	}

	WriteLocker lock(masterList_lock);
	masterList = std::move(newList);
}