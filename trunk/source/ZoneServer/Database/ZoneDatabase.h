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
	bool LoadGroundSpawnsForZone(ZoneServer* z);
	//The input result to this function should be indexed by (id, type, red, green, blue)
	void ProcessEntityColors(class DatabaseResult& result, std::unordered_map<uint32_t, std::shared_ptr<class Entity> >& outEntities);

	bool LoadNPCLocations(ZoneServer* z);

	bool CharacterUpdateBiography(uint32_t char_id, const char* bio);

	bool LoadChatFilters(class ChatFilterLookup& lookup);

private:
	//The return value is the next indice in the query result
	uint32_t ProcessSpawnTableFields(const std::shared_ptr<class Spawn>& spawn, class DatabaseResult& result);
};