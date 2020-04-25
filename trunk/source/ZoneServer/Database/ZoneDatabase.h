#pragma once

#include "../../common/CommonDatabase.h"

class ZoneServer;
class Entity;
class CommandProcess;

class ZoneDatabase : public CommonDatabase {
public:
	ZoneDatabase();
	~ZoneDatabase();

	bool Start();

	bool LoadZoneInfo(ZoneServer* z);
	bool LoadCommands(CommandProcess& process);
	bool LoadCharacter(uint32_t char_id, uint32_t account_id, std::shared_ptr<Entity> entity);

	bool LoadNPCsForZone(ZoneServer* z);
	bool LoadObjectsForZone(ZoneServer* z);
	bool LoadWidgetsForZone(ZoneServer* z);
	bool LoadSignsForZone(ZoneServer* z);

	bool LoadChatFilters(class ChatFilterLookup& lookup);

private:

};