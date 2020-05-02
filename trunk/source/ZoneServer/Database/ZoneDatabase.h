#pragma once

#include "../../common/CommonDatabase.h"
#include "../Spawns/SpawnLocation.h"

class ZoneServer;
class Entity;
class CommandProcess;
class DatabaseResult;

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

	bool LoadSpawnLocationGroups(ZoneServer* z);
	bool LoadSpawnGroupChances(ZoneServer* z);
	bool LoadSpawnLocationGroupAssociations(ZoneServer* z);

	uint32_t LoadSpawnLocation(std::string query, ZoneServer* z, SpawnEntryType type);
	bool LoadNPCLocations(ZoneServer* z);
	bool LoadObjectLocations(ZoneServer* z);
	bool LoadWidgetLocations(ZoneServer* z);
	bool LoadSignLocations(ZoneServer* z);
	bool LoadGroundSpawnLocations(ZoneServer* z);

	bool CharacterUpdateBiography(uint32_t char_id, const char* bio);

	bool LoadChatFilters(class ChatFilterLookup& lookup);

	bool LoadEntityCommands(class MasterEntityCommandList& masterList);

	bool LoadMasterZoneList(class MasterZoneLookup& lookup);

private:
	//The return value is the next indice in the query result
	uint32_t ProcessSpawnTableFields(const std::shared_ptr<class Spawn>& spawn, class DatabaseResult& result);
	//The input result to this function should be indexed by (id, type, red, green, blue)
	void ProcessEntityColors(DatabaseResult& result, std::unordered_map<uint32_t, std::shared_ptr<Spawn> >& outEntities);
	//The input result to this function should be indexed by (spawn_id, slot_id, equip_type, red, green, blue, highlight_red, highlight_green, highlight_blue)
	void ProcessNpcAppearanceEquipment(DatabaseResult& result, std::unordered_map<uint32_t, std::shared_ptr<Spawn> >& outEntities);
};