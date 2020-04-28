#include <stdafx.h>

#include "ZoneServer.h"
#include "../../common/log.h"
#include "../Database/ZoneDatabase.h"
#include "ZoneOperator.h"
#include "Client.h"
#include "../../common/thread.h"
#include "../Controllers/PlayerController.h"
#include "../Commands/CommandProcess.h"
#include "Cell.h"

// Packets
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_CreateGhostCmd_Packet.h"
#include "../Packets/OP_EqDestroyGhostCmd_Packet.h"

// Spawns
#include "../Spawns/Spawn.h"
#include "../Spawns/Entity.h"
#include "../Spawns/Object.h"
#include "../Spawns/Widget.h"
#include "../Spawns/Sign.h"
#include "../Spawns/GroundSpawn.h"

extern ZoneDatabase database;
extern ZoneOperator g_zoneOperator;
extern CommandProcess g_commandProcess;

ZoneServer::ZoneServer(uint32_t zone_id):  chat(Clients, *this) {
	id = zone_id;
	instanceID = 0;
	expansionID = 0;
	//name = "";
	//file = "";
	//description = "";
	safeX = 0;
	safeY = 0;
	safeZ = 0;
	safeHeading = 0;
	underworld = -100000;
	expModifier = 0;
	minRecommended= 0;
	maxRecommended = 0;
	//zoneType = "";
	alwaysLoaded = false;
	cityZone = false;
	weatherAllowed = false;
	minStatus = 0;
	minLevel = 0;
	maxLevel = 0;
	startZone = 0;
	//instanceType = "";
	defaultReenterTime = 0;
	defaultResetTime = 0;
	defaultLockoutTime = 0;
	forceGroupToZone = 0;
	//luaScript = "";
	shutdownTimer = 0;
	//zoneMOTD = "";
	rulesetID = 0;
	isRunning = true;
	pendingClientAdd_lock.SetName("ZoneServer::pendingClientAdd");
	pendingClientRemoval_lock.SetName("ZoneServer::pendingClientRemoval");
}

ZoneServer::~ZoneServer() {
	process_thread.join();
}

bool ZoneServer::Init() {
	bool ret = true;

	LogDebug(LOG_ZONE, 0, "Starting up zone %u", id);

	ret = database.LoadZoneInfo(this);
	if (!ret) {
		LogError(LOG_ZONE, 0, "Failed to load info for the zone %u", id);
		return ret;
	}

	m_SpawnUpdateTimer.Start(200);

	LogDebug(LOG_ZONE, 0, "Zone %u (%s) started", id, description.c_str());

	process_thread = ThreadManager::ThreadStart("ZoneServer::Process", std::bind(&ZoneServer::Process, this));
	m_loadThread = ThreadManager::ThreadStart("ZoneServer::LoadThread", std::bind(&ZoneServer::LoadThread, this));

	return ret;
}

void ZoneServer::Process() {

	while (isRunning) {
		{
			//Check if we should remove any clients
			WriteLocker lock(pendingClientRemoval_lock);
			for (auto& client : pendingClientRemoval) {
				OnClientRemoval(client);
				auto itr = Clients.find(client->GetAccountID());
				if (itr != Clients.end() && EmuWeakCmp(client, itr->second)) {
					Clients.erase(itr);
				}
			}
			pendingClientRemoval.clear();
		}
		{
			//Check if we need to add any clients
			WriteLocker lock(pendingClientAdd_lock);
			for (auto& itr : pendingClientAdd) {
				Clients[itr->GetAccountID()] = itr;
			}
			pendingClientAdd.clear();
		}

		std::map<uint32_t, std::weak_ptr<Client> >::iterator itr;
		for (itr = Clients.begin(); itr != Clients.end(); ) {
 			std::shared_ptr<Client> client = itr->second.lock();
			if (client) {
				client->Process();
				itr++;
			}
			else {
				itr = Clients.erase(itr);
			}
		}

		if (m_SpawnUpdateTimer.Check()) {
			for (std::shared_ptr<Entity> player : players) {
				player->Process();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Probably play with this value
	}
}

bool ZoneServer::AddClient(std::shared_ptr<Client> c) {
	c->SetZone(shared_from_this());
	
	WriteLocker lock(pendingClientAdd_lock);
	pendingClientAdd.insert(c);
	lock.Unlock();

	OP_ZoneInfoMsg_Packet* zone = new OP_ZoneInfoMsg_Packet(c->GetVersion());
	zone->server1 = g_zoneOperator.GetWorldServerName();
	zone->server2 = zone->server1;
	zone->zone = file;
	zone->zone2 = name;
	zone->zone_desc = description;
	zone->char_name = "Foof";
	zone->x = safeX;
	zone->y = safeY;
	zone->z = safeZ;
	zone->year = 3956;
	zone->month = 5;
	zone->day = 5;
	zone->hour = 9;
	zone->minute = 35;
	zone->seconds = 44;
	zone->unknown1[1] = 1;
	zone->expansions_enabled = 524161;
	zone->unknown3[0] = 70;
	zone->unknown3[1] = 459276223;
	zone->unknown3[2] = 8388607;
	zone->auction_website = "eq2emulator.net";
	zone->auction_port = 80;
	zone->upload_page = "test_upload.m";
	zone->upload_key = "dsya987yda9";
	zone->unknown7[0] = 1.0;
	zone->unknown7[1] = 1.0;
	zone->unknown9 = 13;
	zone->zone_flags = 25189220;
	zone->unknown11 = 4294967292;

	c->QueuePacket(zone);
	g_commandProcess.SendCommandList(c);

	return true;
}

void ZoneServer::SendCharacterInfo(std::shared_ptr<Client> client) {
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	database.LoadCharacter(client->GetCharacterID(), client->GetAccountID(), entity);

	// Set this in the spawn constructor
	entity->SetState(16512, false);
	entity->SetSize(0.875f, false);
	entity->SetCollisionRadius(28, false);
	entity->SetSizeRatio(1.0f, false);
	entity->SetSizeMultiplierRatio(1.0f, false);
	entity->SetVisFlags(54, false);
	entity->SetInteractionFlag(12, false);
	entity->SetX(GetSafeX(), false);
	entity->SetY(GetSafeY(), false);
	entity->SetZ(GetSafeZ(), false);

	std::shared_ptr<PlayerController> controller = client->GetController();
	controller->SetControlled(entity);
	SendSpawnToClient(entity, client);

	AddPlayer(entity);

	SendPlayersToNewClient(client);
	ActivateCellsForClient(client);
}

void ZoneServer::AddPlayer(std::shared_ptr<Entity> player) {
	players.push_back(player);

	for (std::pair<uint32_t, std::weak_ptr<Client> > c : Clients) {
		std::shared_ptr<Client> client = c.second.lock();
		if (client) {
			SendSpawnToClient(player, client);
		}
	}
}

void ZoneServer::SendPlayersToNewClient(std::shared_ptr<Client> client) {
	for (std::shared_ptr<Entity> spawn : players) {
		SendSpawnToClient(spawn, client);
	}
}

void ZoneServer::SendSpawnToClient(std::shared_ptr<Spawn> spawn, std::shared_ptr<Client> client) {
	if (client->WasSentSpawn(spawn))
		return;

	OP_CreateGhostCmd_Packet* ghost = new OP_CreateGhostCmd_Packet(client->GetVersion());
	ghost->InsertSpawnData(spawn, client->AddSpawnToIndexMap(spawn));
	ghost->SetEncodedBuffers(client, ghost->header.index);
	client->QueuePacket(ghost);

	spawn->AddClient(client);
}

void ZoneServer::RemoveSpawnFromAllClients(std::shared_ptr<Spawn> spawn) {
	for (std::pair<uint32_t, std::weak_ptr<Client> > c : Clients) {
		std::shared_ptr<Client> client = c.second.lock();
		if (client) {
			SendDestroyGhost(client, spawn);
		}
	}
}

void ZoneServer::SendDestroyGhost(std::shared_ptr<Client> client, std::shared_ptr<Spawn> spawn) {
	if (!client->WasSentSpawn(spawn))
		return;

	OP_EqDestroyGhostCmd_Packet* p = new OP_EqDestroyGhostCmd_Packet(client->GetVersion());
	p->spawn_index = client->GetIndexForSpawn(spawn);
	p->bDelete = 1;
	client->QueuePacket(p);
}

void ZoneServer::RemovePlayer(std::shared_ptr<Entity> player) {
	std::vector<std::shared_ptr<Entity> >::iterator itr = std::find(players.begin(), players.end(), player);
	if (itr != players.end()) {
		RemoveSpawnFromAllClients(*itr);
		players.erase(itr);
	}
}

void ZoneServer::RemoveClient(std::shared_ptr<Client> client) {
	WriteLocker lock(pendingClientRemoval_lock);
	pendingClientRemoval.insert(client);
}

void ZoneServer::OnClientRemoval(const std::shared_ptr<Client>& client) {
	std::shared_ptr<Entity> player = std::static_pointer_cast<Entity>(client->GetController()->GetControlled());
	if (player)
		RemovePlayer(player);
}

void ZoneServer::AddNPCToMasterList(std::shared_ptr<Entity> npc) {
	m_masterNPCList[npc->GetDatabaseID()] = npc;
}

std::shared_ptr<Entity> ZoneServer::GetNPCFromMasterList(uint32_t databaseID) {
	std::map<uint32_t, std::shared_ptr<Entity> >::iterator itr = m_masterNPCList.find(databaseID);
	if (itr != m_masterNPCList.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddObjectToMasterList(std::shared_ptr<Object> object) {
	m_masterObjectList[object->GetDatabaseID()] = object;
}

std::shared_ptr<Object> ZoneServer::GetObjectFromMasterList(uint32_t databaseID) {
	std::map<uint32_t, std::shared_ptr<Object> >::iterator itr = m_masterObjectList.find(databaseID);
	if (itr != m_masterObjectList.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddSignToMasterList(std::shared_ptr<Spawn> sign) {
	m_masterSignList[sign->GetDatabaseID()] = sign;
}

std::shared_ptr<Spawn> ZoneServer::GetSignFromMasterList(uint32_t databaseID) {
	std::map<uint32_t, std::shared_ptr<Spawn> >::iterator itr = m_masterSignList.find(databaseID);
	if (itr != m_masterSignList.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddWidgetToMasterList(std::shared_ptr<Spawn> widget) {
	m_masterWidgetList[widget->GetDatabaseID()] = widget;
}

std::shared_ptr<Spawn> ZoneServer::GetWidgetFromMasterList(uint32_t databaseID) {
	std::map<uint32_t, std::shared_ptr<Spawn> >::iterator itr = m_masterWidgetList.find(databaseID);
	if (itr != m_masterWidgetList.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddGroundSpawnToMasterList(std::shared_ptr<GroundSpawn> groundSpawn) {
	m_masterGroundSpawnList[groundSpawn->GetDatabaseID()] = groundSpawn;
}

std::shared_ptr<GroundSpawn> ZoneServer::GetGroundSpawnFromMasterList(uint32_t databaseID) {
	std::map<uint32_t, std::shared_ptr<GroundSpawn> >::iterator itr = m_masterGroundSpawnList.find(databaseID);
	if (itr != m_masterGroundSpawnList.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddNPCSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator itr = m_npcSpawnLocations.find(id);
	if (itr == m_npcSpawnLocations.end())
		m_npcSpawnLocations[id] = location;
	else
		LogWarn(LOG_NPC, 0, "Attempt to load duplicate NPC Spawn Location (%u).", id);
}

void ZoneServer::ProcessSpawnLocations() {

	// TODO: For instances load spawns that should be removed

	// map is for spawn groups
	std::map<uint32_t, bool> processed_spawn_locations;
	for (std::pair<uint32_t, std::shared_ptr<SpawnLocation> > kvp : m_npcSpawnLocations) {
		std::shared_ptr<SpawnLocation> sl = kvp.second;
		if (!sl)
			continue;

		// If the placement id is in this map it means we already processed it with
		// another spawn in the same spawn group so don't process it again
		if (processed_spawn_locations.count(sl->placement_id) > 0)
			continue;

		// TODO: Spawn group stuff

		// This should be in the else for the spawn group check (TODO above)

		// TODO: instance check to prevent spawns from spawning in the instance

		ProcessSpawnLocation(sl);
	}

	TryActivateCells();
}

void ZoneServer::ProcessSpawnLocation(std::shared_ptr<SpawnLocation> sl, bool respawn) {

	float rndNum = MakeRandom(0, sl->total_percentage);
	for (std::shared_ptr<SpawnEntry> entry : sl->entries) {
		if (entry->spawn_percentage == 0)
			continue;

		if (entry->condition > 0) {
			// TODO: Check conditions
		}

		if (entry->spawn_percentage >= rndNum) {
			std::shared_ptr<Spawn> spawn = nullptr;

			switch (entry->spawn_type) {
			case SpawnEntryType::ENPC:
				spawn = AddNPCSpawn(sl, entry);
				break;
			case SpawnEntryType::EOBJECT:
				spawn = AddObjectSpawn(sl, entry);
				break;
			case SpawnEntryType::EWIDGET:
				spawn = AddWidgetSpawn(sl, entry);
				break;
			case SpawnEntryType::ESIGN:
				spawn = AddSignSpawn(sl, entry);
				break;
			case SpawnEntryType::EGROUNDSPAWN:
				spawn = AddGroundSpawnSpawn(sl, entry);
				break;
			default:
				LogError(LOG_ZONE, 0, "Error adding spawn (%u) to zone", entry->spawn_id);
				continue;
				break;
			}

			if (!spawn)
				continue;

			// TODO: set spawn script

			// TODO: call spawn or respawn
		}
	}
}

std::shared_ptr<Entity> ZoneServer::AddNPCSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	std::shared_ptr<Entity> npc = GetNewNPC(spawnEntry->spawn_id);
	if (npc) {
		DeterminePosition(spawnLocation, npc);
		npc->SetSpawnLocationID(spawnEntry->spawn_location_id);
		npc->SetSpawnEntryID(spawnEntry->spawn_entry_id);
		npc->SetRespawnTime(spawnLocation->respawn);
		npc->SetExpireTime(spawnLocation->expire_time);
		//if (spawnLocation->expire_time > 0)
			//AddSpawnExpireTimer(npc, spawnLocation->expire_time, spawnLocation->expire_offset);
		//AddLoot(npc);
		AddSpawn(npc);
	}

	return npc;
}

std::shared_ptr<Object> ZoneServer::AddObjectSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::AddWidgetSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::AddSignSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	return nullptr;
}

std::shared_ptr<GroundSpawn> ZoneServer::AddGroundSpawnSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	return nullptr;
}

std::shared_ptr<Entity> ZoneServer::GetNewNPC(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<Entity> >::iterator itr = m_masterNPCList.find(id);
	if (itr != m_masterNPCList.end())
		return std::make_shared<Entity>(itr->second);
	else
		return nullptr;
}

std::shared_ptr<Object> ZoneServer::GetNewObject(uint32_t id) {
	return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::GetNewWidget(uint32_t id) {
	return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::GetNewSign(uint32_t id) {
	return nullptr;
}

std::shared_ptr<GroundSpawn> ZoneServer::GetNewGroundSpawn(uint32_t id) {
	return nullptr;
}

void ZoneServer::DeterminePosition(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<Spawn> spawn) {
	if (spawnLocation->x_offset > 0)
		spawn->SetX(MakeRandom(spawnLocation->x, spawnLocation->x + spawnLocation->x_offset), false);
	else
		spawn->SetX(spawnLocation->x, false);

	if (spawnLocation->y_offset > 0)
		spawn->SetY(MakeRandom(spawnLocation->y, spawnLocation->y + spawnLocation->y_offset), false);
	else
		spawn->SetY(spawnLocation->y, false);
	
	if (spawnLocation->z_offset > 0)
		spawn->SetZ(MakeRandom(spawnLocation->z, spawnLocation->z + spawnLocation->z_offset), false);
	else
		spawn->SetZ(spawnLocation->z, false);


	spawn->SetHeading(spawnLocation->heading, false);
	spawn->SetPitch(spawnLocation->pitch, false);
	spawn->SetRoll(spawnLocation->roll, false);
	spawn->SetOrigX(spawn->GetX());
	spawn->SetOrigY(spawn->GetY());
	spawn->SetOrigZ(spawn->GetZ());
	spawn->SetOrigHeading(spawn->GetHeading());
	spawn->SetOrigPitch(spawnLocation->pitch);
	spawn->SetOrigRoll(spawnLocation->roll);
	spawn->SetGridID(spawnLocation->grid_id, false);
	spawn->SetSpawnLocationPlacementID(spawnLocation->placement_id);
}

void ZoneServer::AddSpawn(std::shared_ptr<Spawn> spawn) {
	spawn->SetZone(shared_from_this());
	spawn->PopUpdateFlags();

	// TODO: check for type to push to the correct list
	m_entityList.push_back(std::static_pointer_cast<Entity>(spawn));

	// wrong just being lazy to get spawn to show (100 is cell size, make it a rule)
	int32_t x = static_cast<int32_t>(std::floor(spawn->GetX() / 100.f));
	int32_t y = static_cast<int32_t>(std::floor(spawn->GetZ() / 100.f));
	std::pair<int32_t, int32_t> cellCoords = std::make_pair(x, y);
	AddSpawnToCell(spawn, cellCoords);
}

void ZoneServer::AddSpawnToCell(std::shared_ptr<Spawn> spawn, std::pair<int32_t, int32_t> cellCoords) {
	std::map<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> >::iterator itr = m_spGrid.find(cellCoords);
	if (itr != m_spGrid.end())
		itr->second->AddSpawn(spawn);
	else {
		std::shared_ptr<Cell> cell = std::make_shared<Cell>(cellCoords);
		m_spGrid[cellCoords] = cell;
		cell->AddSpawn(spawn);
	}
}

void ZoneServer::TryActivateCells() {
	if (Clients.size() == 0)
		return;

	for (std::pair<uint32_t, std::weak_ptr<Client> > c : Clients) {
		std::shared_ptr<Client> client = c.second.lock();
		if (client)
			ActivateCellsForClient(client);
	}
}

void ZoneServer::ActivateCellsForClient(std::shared_ptr<Client> client) {
	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller)
		return;

	std::shared_ptr<Entity> player = controller->GetControlled();
	if (!player)
		return;

	std::vector<std::shared_ptr<Cell> > cells = GetNeighboringCells(player->GetCellCoordinates());
	for (std::shared_ptr<Cell> cell : cells)
		cell->ActivateCell(client);
}

std::vector<std::shared_ptr<Cell> > ZoneServer::GetNeighboringCells(std::pair<int32_t, int32_t> cellCoords) {
	std::vector<std::shared_ptr<Cell> > ret;

	std::map<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> >::iterator itr = m_spGrid.find(cellCoords);
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first - 1, cellCoords.second - 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first, cellCoords.second - 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first + 1, cellCoords.second - 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first - 1, cellCoords.second));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first + 1, cellCoords.second));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first - 1, cellCoords.second + 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first, cellCoords.second + 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	itr = m_spGrid.find(std::make_pair(cellCoords.first + 1, cellCoords.second + 1));
	if (itr != m_spGrid.end())
		ret.push_back(itr->second);

	return ret;
}

std::pair<int32_t, int32_t> ZoneServer::GettCellCoordinatesForSpawn(std::shared_ptr<Spawn> spawn) {
	// !00 is cell size, should be a rule
	int32_t x = static_cast<int32_t>(std::floor(spawn->GetX() / 100));
	int32_t y = static_cast<int32_t>(std::floor(spawn->GetZ() / 100));
	return std::make_pair(x, y);
}

void ZoneServer::LoadThread() {

	// TODO: Entity Commands (Global?)

	LogInfo(LOG_NPC, 0, "-Loading NPC data...");
	database.LoadNPCsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load NPC data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Object data...");
	database.LoadObjectsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load Object data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Widget data...");
	database.LoadWidgetsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load Widget data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Sign data...");
	database.LoadSignsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load Sign data complete!");

	LogInfo(LOG_NPC, 0, "-Loading GroundSpawn data...");
	database.LoadGroundSpawnsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load GroundSpawn data complete!");

	// TODO: Spawn locations
	LogInfo(LOG_NPC, 0, "-Loading NPC spawn location data...");
	database.LoadNPCLocations(this);
	LogInfo(LOG_NPC, 0, "-Load NPC spawn location data complete!");

	// TODO: process spawn locations (put spawn in world)
	ProcessSpawnLocations();
}