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
#include "../../common/string.h"

// Packets
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_CreateGhostCmd_Packet.h"
#include "../Packets/OP_CreateSignWidgetCmd_Packet.h"
#include "../Packets/OP_CreateWidgetCmd_Packet.h"
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

			for (std::pair<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> > kvp : m_spGrid) {
				if (kvp.second->IsActive())
					kvp.second->Process();
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
	entity->SetZone(shared_from_this());

	// Set this in the spawn constructor
	entity->SetState(16512, false);
	entity->SetSize(0.875f, false);
	entity->SetCollisionRadius(28, false);
	entity->SetSizeRatio(1.0f, false);
	entity->SetInteractionFlag(12, false);
	entity->SetX(GetSafeX(), false);
	entity->SetY(GetSafeY(), false);
	entity->SetZ(GetSafeZ(), false);
	entity->SetIsPlayer(true, false);
	
	m_playerClientList[entity->GetID()] = client;

	std::pair<int32_t, int32_t> cellCoords = entity->GetCellCoordinates();
	LogWarn(LOG_PLAYER, 0, "New player in cell (%i, %i) player loc = (%f, %f, %f)", cellCoords.first, cellCoords.second, entity->GetX(), entity->GetY(), entity->GetZ());

	cellCoords = GetCellCoordinatesForSpawn(entity);
	LogWarn(LOG_PLAYER, 0, "Cell coords check 2 (%i, %i)", cellCoords.first, cellCoords.second);

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

	OP_CreateGhostCmd_Packet* ghost = nullptr;
	if (spawn->GetSignData()) {
		ghost = new OP_CreateSignWidgetCmd_Packet(client->GetVersion());
		OP_CreateSignWidgetCmd_Packet* sign = static_cast<OP_CreateSignWidgetCmd_Packet*>(ghost);
		sign->signData.title = spawn->GetSignData()->GetTitle();
		sign->signData.description = spawn->GetSignData()->GetDescription();
		sign->signData.distance = spawn->GetSignData()->GetDistance();
		sign->signData.show = true;
		// language...
		if (spawn->GetWidgetData()) {
			OP_CreateWidgetCmd_Packet* widget = static_cast<OP_CreateWidgetCmd_Packet*>(ghost);
			widget->widgetData.widgetID = spawn->GetWidgetData()->GetWidgetID();
			widget->widgetData.widgetX = spawn->GetWidgetData()->GetWidgetX();
			widget->widgetData.widgetY = spawn->GetWidgetData()->GetWidgetY();
			widget->widgetData.widgetZ = spawn->GetWidgetData()->GetWidgetZ();
		}
	}
	else if (spawn->GetWidgetData()) {
		ghost = new OP_CreateWidgetCmd_Packet(client->GetVersion());
		OP_CreateWidgetCmd_Packet* widget = static_cast<OP_CreateWidgetCmd_Packet*>(ghost);
		widget->widgetData.widgetID = spawn->GetWidgetData()->GetWidgetID();
		widget->widgetData.widgetX = spawn->GetWidgetData()->GetWidgetX();
		widget->widgetData.widgetY = spawn->GetWidgetData()->GetWidgetY();
		widget->widgetData.widgetZ = spawn->GetWidgetData()->GetWidgetZ();
	}
	else
		ghost = new OP_CreateGhostCmd_Packet(client->GetVersion());

	ghost->InsertSpawnData(client, spawn, client->AddSpawnToIndexMap(spawn));
	ghost->SetEncodedBuffers(client, ghost->header.index);
	spawn->AddClient(client, ghost->vis.CalculateCRC(), spawn->GetVisUpdateTag());
	client->QueuePacket(ghost);
}

void ZoneServer::RemoveSpawnFromClient(std::shared_ptr<Spawn> spawn, std::shared_ptr <Client> client) {
	SendDestroyGhost(client, spawn);
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

	uint32_t index = client->GetIndexForSpawn(spawn);
	OP_EqDestroyGhostCmd_Packet* p = new OP_EqDestroyGhostCmd_Packet(client->GetVersion());
	p->spawn_index = index;
	p->bDelete = 1;
	client->QueuePacket(p);

	client->RemoveSpawnFromIndexMap(spawn);
	client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnInfo, index);
	client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnPos, index);
	client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnVis, index);

	spawn->RemoveClient(client);
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

void ZoneServer::AddObjectSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator itr = m_objectSpawnLocations.find(id);
	if (itr == m_objectSpawnLocations.end())
		m_objectSpawnLocations[id] = location;
	else
		LogWarn(LOG_NPC, 0, "Attempt to load duplicate Object Spawn Location (%u).", id);
}

void ZoneServer::AddWidgetSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator itr = m_widgetSpawnLocations.find(id);
	if (itr == m_widgetSpawnLocations.end())
		m_widgetSpawnLocations[id] = location;
	else
		LogWarn(LOG_NPC, 0, "Attempt to load duplicate Widget Spawn Location (%u).", id);
}

void ZoneServer::AddSignSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator itr = m_signSpawnLocations.find(id);
	if (itr == m_signSpawnLocations.end())
		m_signSpawnLocations[id] = location;
	else
		LogWarn(LOG_NPC, 0, "Attempt to load duplicate Sign Spawn Location (%u).", id);
}

void ZoneServer::AddGroundSpawnSpawnLocation(uint32_t id, std::shared_ptr<SpawnLocation> location) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator itr = m_groundspawnSpawnLocations.find(id);
	if (itr == m_groundspawnSpawnLocations.end())
		m_groundspawnSpawnLocations[id] = location;
	else
		LogWarn(LOG_NPC, 0, "Attempt to load duplicate GroundSpawn Spawn Location (%u).", id);
}

void ZoneServer::ProcessSpawnLocations() {
	ProcessSpawnLocations(SpawnEntryType::ENPC);
	ProcessSpawnLocations(SpawnEntryType::EOBJECT);
	ProcessSpawnLocations(SpawnEntryType::EWIDGET);
	ProcessSpawnLocations(SpawnEntryType::ESIGN);
	ProcessSpawnLocations(SpawnEntryType::EGROUNDSPAWN);

	TryActivateCells();
}

void ZoneServer::ProcessSpawnLocations(SpawnEntryType type) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >* list = nullptr;
	switch (type) {
	case SpawnEntryType::ENPC:
		list = &m_npcSpawnLocations;
		break;
	case SpawnEntryType::EOBJECT:
		list = &m_objectSpawnLocations;
		break;
	case SpawnEntryType::EWIDGET:
		list = &m_widgetSpawnLocations;
		break;
	case SpawnEntryType::ESIGN:
		list = &m_signSpawnLocations;
		break;
	case SpawnEntryType::EGROUNDSPAWN:
		list = &m_groundspawnSpawnLocations;
		break;
	}

	if (!list)
		return;

	// TODO: For instances load spawns that should be removed

	std::map<uint32_t, bool> processed_spawn_locations;
	for (std::pair<uint32_t, std::shared_ptr<SpawnLocation> > kvp : *list) {
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
		AddSpawn(npc, SpawnEntryType::ENPC);
	}

	return npc;
}

std::shared_ptr<Object> ZoneServer::AddObjectSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	std::shared_ptr<Object> object = GetNewObject(spawnEntry->spawn_id);
	if (object) {
		DeterminePosition(spawnLocation, object);
		object->SetSpawnLocationID(spawnEntry->spawn_location_id);
		object->SetSpawnEntryID(spawnEntry->spawn_entry_id);
		object->SetRespawnTime(spawnLocation->respawn);
		object->SetExpireTime(spawnLocation->expire_time);
		//if (spawnLocation->expire_time > 0)
			//AddSpawnExpireTimer(object, spawnLocation->expire_time, spawnLocation->expire_offset);
		AddSpawn(object, SpawnEntryType::EOBJECT);
	}

	return object;
}

std::shared_ptr<Spawn> ZoneServer::AddWidgetSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	std::shared_ptr<Spawn> widget = GetNewWidget(spawnEntry->spawn_id);
	if (widget) {
		DeterminePosition(spawnLocation, widget);
		widget->SetSpawnLocationID(spawnEntry->spawn_location_id);
		widget->SetSpawnEntryID(spawnEntry->spawn_entry_id);
		widget->SetRespawnTime(spawnLocation->respawn);
		widget->SetExpireTime(spawnLocation->expire_time);
		//if (spawnLocation->expire_time > 0)
			//AddSpawnExpireTimer(widget, spawnLocation->expire_time, spawnLocation->expire_offset);
		AddSpawn(widget, SpawnEntryType::EWIDGET);
	}

	return widget;
}

std::shared_ptr<Spawn> ZoneServer::AddSignSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	std::shared_ptr<Spawn> sign = GetNewSign(spawnEntry->spawn_id);
	if (sign) {
		DeterminePosition(spawnLocation, sign);
		sign->SetSpawnLocationID(spawnEntry->spawn_location_id);
		sign->SetSpawnEntryID(spawnEntry->spawn_entry_id);
		sign->SetRespawnTime(spawnLocation->respawn);
		sign->SetExpireTime(spawnLocation->expire_time);
		//if (spawnLocation->expire_time > 0)
			//AddSpawnExpireTimer(sign, spawnLocation->expire_time, spawnLocation->expire_offset);
		AddSpawn(sign, SpawnEntryType::ESIGN);
	}

	return sign;
}

std::shared_ptr<GroundSpawn> ZoneServer::AddGroundSpawnSpawn(std::shared_ptr<SpawnLocation> spawnLocation, std::shared_ptr<SpawnEntry> spawnEntry) {
	std::shared_ptr<GroundSpawn> groundSpawn = GetNewGroundSpawn(spawnEntry->spawn_id);
	if (groundSpawn) {
		DeterminePosition(spawnLocation, groundSpawn);
		groundSpawn->SetSpawnLocationID(spawnEntry->spawn_location_id);
		groundSpawn->SetSpawnEntryID(spawnEntry->spawn_entry_id);
		groundSpawn->SetRespawnTime(spawnLocation->respawn);
		groundSpawn->SetExpireTime(spawnLocation->expire_time);
		//if (spawnLocation->expire_time > 0)
			//AddSpawnExpireTimer(groundSpawn, spawnLocation->expire_time, spawnLocation->expire_offset);
		AddSpawn(groundSpawn, SpawnEntryType::EGROUNDSPAWN);
	}

	return groundSpawn;
}

std::shared_ptr<Entity> ZoneServer::GetNewNPC(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<Entity> >::iterator itr = m_masterNPCList.find(id);
	if (itr != m_masterNPCList.end())
		return std::make_shared<Entity>(itr->second);
	else
		return nullptr;
}

std::shared_ptr<Object> ZoneServer::GetNewObject(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<Object> >::iterator itr = m_masterObjectList.find(id);
	if (itr != m_masterObjectList.end())
		return std::make_shared<Object>(itr->second);
	else
		return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::GetNewWidget(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<Spawn> >::iterator itr = m_masterWidgetList.find(id);
	if (itr != m_masterWidgetList.end())
		return std::make_shared<Spawn>(itr->second);
	else
		return nullptr;
}

std::shared_ptr<Spawn> ZoneServer::GetNewSign(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<Spawn> >::iterator itr = m_masterSignList.find(id);
	if (itr != m_masterSignList.end())
		return std::make_shared<Spawn>(itr->second);
	else
		return nullptr;
}

std::shared_ptr<GroundSpawn> ZoneServer::GetNewGroundSpawn(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<GroundSpawn> >::iterator itr = m_masterGroundSpawnList.find(id);
	if (itr != m_masterGroundSpawnList.end())
		return std::make_shared<GroundSpawn>(itr->second);
	else
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

void ZoneServer::AddSpawn(std::shared_ptr<Spawn> spawn, SpawnEntryType type) {
	spawn->SetZone(shared_from_this());
	spawn->PopUpdateFlags();

	// TODO: check for type to push to the correct list
	switch (type) {
	case SpawnEntryType::ENPC:
		m_entityList.push_back(std::static_pointer_cast<Entity>(spawn));
		break;
	case SpawnEntryType::EOBJECT:
		m_objectList.push_back(std::static_pointer_cast<Object>(spawn));
		break;
	case SpawnEntryType::EWIDGET:
		m_widgetList.push_back(spawn);
		break;
	case SpawnEntryType::ESIGN:
		m_signList.push_back(spawn);
		break;
	case SpawnEntryType::EGROUNDSPAWN:
		m_groundspawnList.push_back(std::static_pointer_cast<GroundSpawn>(spawn));
		break;
	}

	// wrong just being lazy to get spawn to show (100 is cell size, make it a rule)
	int32_t x = static_cast<int32_t>(std::floor(spawn->GetX() / 100.f));
	int32_t y = static_cast<int32_t>(std::floor(spawn->GetZ() / 100.f));
	std::pair<int32_t, int32_t> cellCoords = std::make_pair(x, y);
	AddSpawnToCell(spawn, cellCoords);
}

std::shared_ptr<Cell> ZoneServer::GetCell(std::pair<int32_t, int32_t> coordinates) {
	std::map<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> >::iterator itr = m_spGrid.find(coordinates);
	if (itr != m_spGrid.end())
		return itr->second;
	else {
		std::shared_ptr<Cell> cell = std::make_shared<Cell>(coordinates);
		m_spGrid[coordinates] = cell;
		return cell;
	}
}

void ZoneServer::AddSpawnToCell(std::shared_ptr<Spawn> spawn, std::pair<int32_t, int32_t> cellCoords) {
	std::shared_ptr<Cell> cell = GetCell(cellCoords);
	if (cell)
		cell->AddSpawn(spawn);
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

std::pair<int32_t, int32_t> ZoneServer::GetCellCoordinatesForSpawn(std::shared_ptr<Spawn> spawn) {
	// 100 is cell size, should be a rule
	int32_t x = static_cast<int32_t>(std::floor(spawn->GetX() / 100.0f));
	int32_t y = static_cast<int32_t>(std::floor(spawn->GetZ() / 100.0f));
	return std::make_pair(x, y);
}

uint32_t ZoneServer::GetCellDistance(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2) {
	return GetCellDistance(cell1->GetCellCoordinates(), cell2->GetCellCoordinates());
}

uint32_t ZoneServer::GetCellDistance(std::pair<int32_t, int32_t> coord1, std::pair<int32_t, int32_t> coord2) {
	return std::max<uint32_t>(std::abs(coord1.first - coord2.first), std::abs(coord1.second - coord2.second));
}

void ZoneServer::ChangeSpawnCell(std::shared_ptr<Spawn> spawn, std::pair<int32_t, int32_t> oldCellCoord) {
	// If not a player and not a global spawn move the spawn to the new cell
	std::shared_ptr<Cell> newCell = GetCell(spawn->GetCellCoordinates());
	std::shared_ptr<Cell> oldCell = GetCell(oldCellCoord);

	newCell->AddSpawn(spawn);
	oldCell->RemoveSpawn(spawn);
	// If Player deactivate old cells and activate new
	std::shared_ptr<Client> client = GetClientForSpawn(spawn);
	if (client) {
		ActivateCellsForClient(client);
		TryDeactivateCellsForClient(client, oldCellCoord);
	}
}

void ZoneServer::TryDeactivateCellsForClient(std::shared_ptr<Client> client, std::pair<int32_t, int32_t> cellCoord) {
	std::vector<std::shared_ptr<Cell> > cells = GetNeighboringCells(cellCoord);
	for (std::shared_ptr<Cell> cell : cells) {
		if (GetCellDistance(client->GetController()->GetControlled()->GetCellCoordinates(), cell->GetCellCoordinates()) > 1)
			cell->SendRemoveSpawnsForClient(client);
		

		uint32_t minDistance = 0xFFFFFFFF;
		for (std::shared_ptr<Entity> p : players)
			minDistance = std::min<uint32_t>(minDistance, GetCellDistance(p->GetCellCoordinates(), cell->GetCellCoordinates()));

		if (minDistance > 1)
			cell->DeactivateCell();
	}
}

std::shared_ptr<Client> ZoneServer::GetClientForSpawn(std::shared_ptr<Spawn> spawn) {
	std::map<uint32_t, std::weak_ptr<Client> >::iterator itr = m_playerClientList.find(spawn->GetID());
	if (itr != m_playerClientList.end())
		return itr->second.lock();

	return nullptr;
}

std::shared_ptr<Entity> ZoneServer::GetPlayerEntityByName(std::string player) {
	for (std::shared_ptr<Entity> p : players) {
		if (strcasecmp(p->GetName().c_str(),player.c_str()) == 0) {
			return p;
		}
	}

	return nullptr;
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

	LogInfo(LOG_NPC, 0, "-Loading NPC spawn location data...");
	database.LoadNPCLocations(this);
	LogInfo(LOG_NPC, 0, "-Load NPC spawn location data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Object spawn location data...");
	database.LoadObjectLocations(this);
	LogInfo(LOG_NPC, 0, "-Load Object spawn location data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Widget spawn location data...");
	database.LoadWidgetLocations(this);
	LogInfo(LOG_NPC, 0, "-Load Widget spawn location data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Sign spawn location data...");
	database.LoadSignLocations(this);
	LogInfo(LOG_NPC, 0, "-Load Sign spawn location data complete!");

	LogInfo(LOG_NPC, 0, "-Loading GroundSpawn spawn location data...");
	database.LoadGroundSpawnLocations(this);
	LogInfo(LOG_NPC, 0, "-Load GroundSpawn spawn location data complete!");

	// TODO: process spawn locations (put spawn in world)
	ProcessSpawnLocations();
}