#pragma once

#include <stdint.h>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <memory>
#include "../../common/timer.h"
#include "../../common/Mutex.h"
#include "../Chat/ZoneChat.h"

#include "../Spawns/SpawnLocation.h"

class Client;
class Spawn;
class Entity;
class Object;
class GroundSpawn;
class Cell;
class EmuLuaState;
class SpawnCamp;

// This will be where every thing happens
class ZoneServer : public std::enable_shared_from_this<ZoneServer> {
public:
	ZoneServer(uint32_t zone_id);
	~ZoneServer();

	bool Init();

	void Process();

	bool AddClient(std::shared_ptr<Client> client, bool bInitialLogin);

	void SendCharacterInfo(std::shared_ptr<Client> client);
	std::shared_ptr<Entity> LoadCharacter(const std::shared_ptr<Client>& client);
	void AddPlayer(std::shared_ptr<Entity> player);
	void SendPlayersToNewClient(std::shared_ptr<Client> client);
	void SendSpawnToClient(std::shared_ptr<Spawn> spawn, std::shared_ptr<Client> client);
	
	void RemoveSpawnFromClient(std::shared_ptr<Spawn> spawn, std::shared_ptr <Client> client);
	void RemoveSpawnFromAllClients(std::shared_ptr<Spawn> spawn);
	void SendDestroyGhost(std::shared_ptr<Client> client, std::shared_ptr<Spawn> spawn);
	void RemoveSpawn(std::shared_ptr<Spawn> spawn);
	void RemovePlayer(std::shared_ptr<Entity> player);
	void RemoveClient(std::shared_ptr<Client> client);

	void AddNPCToMasterList(std::shared_ptr<Entity> npc);
	std::shared_ptr<Entity> GetNPCFromMasterList(uint32_t databaseID);
	void AddObjectToMasterList(std::shared_ptr<Object> object);
	std::shared_ptr<Object> GetObjectFromMasterList(uint32_t databaseID);
	void AddSignToMasterList(std::shared_ptr<Spawn> sign);
	std::shared_ptr<Spawn> GetSignFromMasterList(uint32_t databaseID);
	void AddWidgetToMasterList(std::shared_ptr<Spawn> widget);
	std::shared_ptr<Spawn> GetWidgetFromMasterList(uint32_t databaseID);
	void AddGroundSpawnToMasterList(std::shared_ptr<GroundSpawn> groundSpawn);
	std::shared_ptr<GroundSpawn> GetGroundSpawnFromMasterList(uint32_t databaseID);
	std::shared_ptr<Spawn> GetSpawnFromMasterList(uint32_t databaseID);
	std::shared_ptr<Spawn> GetNewSpawnFromMasterList(uint32_t databaseID);

	void AddNPCSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location);
	void AddObjectSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location);
	void AddWidgetSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location);
	void AddSignSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location);
	void AddGroundSpawnSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location);
	// TODO: locations for other types of spawns
	void ProcessSpawnLocations(); // private?
	void ProcessSpawnLocations(SpawnEntryType type); // private?
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >* GetSpawnLocationList(SpawnEntryType type);
	std::shared_ptr<Spawn> ProcessSpawnLocation(std::shared_ptr<SpawnLocation> sl, bool respawn = false); // private?
	std::shared_ptr<Entity> AddNPCSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry);
	std::shared_ptr<Object> AddObjectSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry);
	std::shared_ptr<Spawn> AddWidgetSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry);
	std::shared_ptr<Spawn> AddSignSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry);
	std::shared_ptr<GroundSpawn> AddGroundSpawnSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry);
	std::shared_ptr<Entity> GetNewNPC(uint32_t id);
	std::shared_ptr<Object> GetNewObject(uint32_t id);
	std::shared_ptr<Spawn> GetNewWidget(uint32_t id);
	std::shared_ptr<Spawn> GetNewSign(uint32_t id);
	std::shared_ptr<GroundSpawn> GetNewGroundSpawn(uint32_t id);
	void DeterminePosition(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<Spawn> spawn); // private?
	void AddSpawn(std::shared_ptr<Spawn> spawn, SpawnEntryType type);

	void AddSpawnGroupLocation(uint32_t group_id, uint32_t placement_location_id, uint32_t spawn_location_id);
	void AddSpawnGroupChance(uint32_t group_id, float percent);
	void AddSpawnGroupAssociation(uint32_t group_id1, uint32_t group_id2);
	uint32_t CalculateSpawnGroup(std::shared_ptr<SpawnLocation> spawnLocation, SpawnEntryType type, bool respawn = false);

	std::shared_ptr<Cell> GetCell(std::pair<int32_t, int32_t> coordinates);
	void AddSpawnToCell(std::shared_ptr<Spawn>, std::pair<int32_t, int32_t> cellCoords);
	void TryActivateCells();
	void ActivateCellsForClient(std::shared_ptr<Client> client);
	std::vector<std::shared_ptr<Cell> > GetNeighboringCells(std::pair<int32_t, int32_t> cellCoords);
	std::pair<int32_t, int32_t> GetCellCoordinatesForSpawn(std::shared_ptr<Spawn> spawn);
	uint32_t GetCellDistance(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2);
	uint32_t GetCellDistance(std::pair<int32_t, int32_t> coord1, std::pair<int32_t, int32_t> coord2);
	void ChangeSpawnCell(std::shared_ptr<Spawn> spawn, std::pair<int32_t, int32_t> oldCellCoord);
	void TryDeactivateCellsForClient(std::shared_ptr<Client> client, std::pair<int32_t, int32_t> cellCoord);

	std::shared_ptr<Client> GetClientForSpawn(std::shared_ptr<Spawn> spawn);
	std::shared_ptr<Entity> GetPlayerEntityByName(std::string player);
	void Depop();

	void LoadSpawns();

	std::shared_ptr<Entity> FindLinkdeadCharacter(uint32_t characterID);

	struct ZoneLoadFlags {
		bool bNeedsSpawnLoad : 1;
	};

	union {
		ZoneLoadFlags m_loadFlags;
		uint8_t m_loadFlagsByte;
	};

	ZoneChat chat;

	std::vector<std::shared_ptr<SpawnCamp> >* GetSpawnCamps() { return &m_spawnCamps; }
	void AddSpawnCamp(std::shared_ptr<SpawnCamp> camp);

	void DeleteSpawnFromLocation(std::shared_ptr<Spawn> spawn, bool remove);
private:

	std::vector<std::shared_ptr<Entity> > players;
	std::map<uint32_t, std::weak_ptr<Client> > Clients;
	std::map<uint32_t, std::weak_ptr<Client> > m_playerClientList;

	std::set<std::shared_ptr<Client> > pendingClientAdd;
	Mutex pendingClientAdd_lock;

	std::set<std::shared_ptr<Client> > pendingClientRemoval;
	Mutex pendingClientRemoval_lock;

	std::thread process_thread;
	bool isRunning;

	Timer m_SpawnUpdateTimer;

	std::map<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> > m_spGrid;
	std::vector<std::weak_ptr<Spawn> > m_globalSpawns;

	// Lists of stuff actually in the world
	std::vector<std::shared_ptr<Entity> > m_entityList;
	std::vector<std::shared_ptr<Object> > m_objectList;
	std::vector<std::shared_ptr<Spawn> > m_signList;
	std::vector<std::shared_ptr<Spawn> > m_widgetList;
	std::vector<std::shared_ptr<GroundSpawn> > m_groundspawnList;

	// Master lists loaded from the database, key = database id
	std::map<uint32_t, std::shared_ptr<Entity> > m_masterNPCList;
	std::map<uint32_t, std::shared_ptr<Object> > m_masterObjectList;
	std::map<uint32_t, std::shared_ptr<GroundSpawn> > m_masterGroundSpawnList;
	std::map<uint32_t, std::shared_ptr<Spawn> > m_masterWidgetList;
	std::map<uint32_t, std::shared_ptr<Spawn> > m_masterSignList;

	// Lists for spawn locations, loaded from the DB
	std::map<uint32_t, std::shared_ptr<SpawnLocation> > m_npcSpawnLocations;
	std::map<uint32_t, std::shared_ptr<SpawnLocation> > m_objectSpawnLocations;
	std::map<uint32_t, std::shared_ptr<SpawnLocation> > m_widgetSpawnLocations;
	std::map<uint32_t, std::shared_ptr<SpawnLocation> > m_signSpawnLocations;
	std::map<uint32_t, std::shared_ptr<SpawnLocation> > m_groundspawnSpawnLocations;

	// Spawn groups, loaded from the DB
	std::map<uint32_t, std::map<uint32_t, uint32_t> > m_spawnGroupLocations;	// group_id, map<placement_location_id, spawn_location_id>
	std::map<uint32_t, std::list<uint32_t> > m_spawnLocationGroups;				// placement_location_id, list<group_id>
	std::map<uint32_t, float> m_spawnGroupChances;								// group_id, chance
	std::map<uint32_t, std::set<uint32_t> > m_spawnGroupAssociations;			// group_id1, set<group_id2>

	std::vector<std::shared_ptr<SpawnCamp> > m_spawnCamps;

	// following is info from `zones` table in the DB
	uint32_t id;
	uint32_t instanceID;
	uint8_t expansionID;
	std::string name;
	std::string file;
	std::string description;
	float safeX;
	float safeY;
	float safeZ;
	float safeHeading;
	float underworld;
	float expModifier;
	uint8_t minRecommended;
	uint8_t maxRecommended;
	std::string zoneType;
	bool alwaysLoaded;
	bool cityZone;
	bool weatherAllowed;
	uint32_t minStatus;
	uint32_t minLevel;
	uint32_t maxLevel;
	uint8_t startZone;
	std::string instanceType;
	uint32_t defaultReenterTime;
	uint32_t defaultResetTime;
	uint32_t defaultLockoutTime;
	uint16_t forceGroupToZone;
	std::string luaScript;
	uint32_t shutdownTimer;
	std::string zoneMOTD;
	uint32_t rulesetID;
	uint32_t scriptID;

	std::shared_ptr<EmuLuaState> m_luaState;
	std::map<uint32_t, std::shared_ptr<EmuLuaState> > m_spawnStates;

	void OnClientRemoval(const std::shared_ptr<Client>& client);
	void CheckNeededLoads();

	Timer m_saveCharactersTimer;
	void SaveCharactersInZone();

public:
	//void SetID(uint32_t val) { id = val; }
	uint32_t GetID() { return id; }
	void SetInstanceID(uint32_t val) { instanceID = val; }
	uint32_t GetInstanceID() { return  instanceID; }
	void SetExpansionID(uint8_t val) { expansionID = val; }
	uint8_t GetExpansionID() { return expansionID; }
	void SetName(std::string val) { name = val; }
	std::string GetName() { return name; }
	void SetFile(std::string val) { file = val; }
	std::string GetFile() { return file; }
	void SetDescription(std::string val) { description = val; }
	std::string GetDescription() { return description; }
	void SetSafeX(float val) { safeX = val; }
	float GetSafeX() { return safeX; }
	void SetSafeY(float val) { safeY = val; }
	float GetSafeY() { return safeY; }
	void SetSafeZ(float val) { safeZ = val; }
	float GetSafeZ() { return safeZ; }
	void SetSafeHeading(float val) { safeHeading = val; }
	float GetSafeHeading() { return safeHeading; }
	void SetUnderworld(float val) { underworld = val; }
	float GetUnderworld() { return underworld; }
	void SetEXPModifier(float val) { expModifier = val; }
	float GetEXPModifier() { return expModifier; }
	void SetMinRecommended(uint8_t val) { minRecommended = val; }
	uint8_t GetMinRecommended() { return minRecommended; }
	void SetMaxRecomended(uint8_t val) { maxRecommended = val; }
	uint8_t GetMaxRecomended() { return maxRecommended; }
	void SetZoneType(std::string val) { zoneType = val; }
	std::string GetZoneType() { return zoneType; }
	void SetIsAlwaysLoaded(bool val) { alwaysLoaded = val; }
	bool GetAlwaysLoaded() { return alwaysLoaded; }
	void SetCityZone(bool val) { cityZone = val; }
	bool GetCityZone() { return cityZone; }
	void SetWeatherAllowed(bool val) { weatherAllowed = val; }
	bool GetWeatherAllowed() { return weatherAllowed; }
	void SetMinStatus(uint32_t val) { minStatus = val; }
	uint32_t GetMinStatus() { return minStatus; }
	void SetMinLevel(uint32_t val) { minLevel = val; }
	uint32_t GetMinLevel() { return minLevel; }
	void SetMaxLevel(uint32_t val) { maxLevel = val; }
	uint32_t GetMaxLevel() { return maxLevel; }
	void SetStartingZone(uint8_t val) { startZone = val; }
	uint8_t GetStartingZone() { return startZone; }
	void SetInstanceType(std::string val) { instanceType = val; }
	std::string GetInstanceType() { return instanceType; }
	void SetDefaultReenterTime(uint32_t val) { defaultReenterTime = val; }
	uint32_t GetDefaultReenterTime() { return defaultReenterTime; }
	void SetDefaultResetTime(uint32_t val) { defaultResetTime = val; }
	uint32_t GetDefaultResetTime() { return defaultResetTime; }
	void SetDefaultLockoutTime(uint32_t val) { defaultLockoutTime = val; }
	uint32_t GetDefaultLockoutTime() { return defaultLockoutTime; }
	void SetForceGroupToZone(uint16_t val) { forceGroupToZone = val; }
	uint16_t GetForceGroupToZone() { return forceGroupToZone; }
	void SetScriptID(uint32_t id) { scriptID = id; }
	std::string GetLuaScript() { return luaScript; }
	void SetShutdownTimer(uint32_t val) { shutdownTimer = val; }
	uint32_t GetShutdownTimer() { return shutdownTimer; }
	void SetZoneMOTD(std::string val) { zoneMOTD = val; }
	std::string GetZoneMOTD() { return zoneMOTD; }
	void SetRulesetID(uint32_t val) { rulesetID = val; }
	uint32_t GetRulesetID() { return rulesetID; }
	std::shared_ptr<EmuLuaState> LoadSpawnState(uint32_t id);
	void LoadScript();
	void CallScript(const char* function, const std::shared_ptr<Spawn>& spawnArg = std::shared_ptr<Spawn>(), uint32_t gridID = 0);
};