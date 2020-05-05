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
#include "../Lua/LuaInterface.h"
#include "../Lua/LuaGlobals.h"

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
extern LuaGlobals g_luaGlobals;

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
	m_loadFlagsByte = 0xFF;
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

	return ret;
}

void ZoneServer::Process() {

	while (isRunning) {
		{
			if (m_loadFlagsByte) {
				CheckNeededLoads();
			}

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
	entity->SetMovementMode(0, false);
	
	m_playerClientList[entity->GetServerID()] = client;

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
	}
	if (spawn->GetWidgetData()) {
		if (!ghost) {
			ghost = new OP_CreateWidgetCmd_Packet(client->GetVersion());
		}
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

std::shared_ptr<Spawn> ZoneServer::GetSpawnFromMasterList(uint32_t databaseID) {
	std::shared_ptr<Spawn> ret = nullptr;
	ret = GetNPCFromMasterList(databaseID);

	if (!ret)
		ret = GetObjectFromMasterList(databaseID);

	if (!ret)
		ret = GetWidgetFromMasterList(databaseID);

	if (!ret)
		ret = GetSignFromMasterList(databaseID);

	if (!ret)
		ret = GetGroundSpawnFromMasterList(databaseID);

	return ret;
}

std::shared_ptr<Spawn> ZoneServer::GetNewSpawnFromMasterList(uint32_t databaseID) {
	std::shared_ptr<Spawn> ret = GetNewNPC(databaseID);

	if (!ret)
		ret = GetNewObject(databaseID);

	if (!ret)
		ret = GetNewWidget(databaseID);

	if (!ret)
		ret = GetNewSign(databaseID);

	if (!ret)
		ret = GetNewGroundSpawn(databaseID);

	// if ret is still null we need to try and load it from the db
	if (!ret) {
		if (database.LoadNPC(this, databaseID))
			ret = GetNewNPC(databaseID);
	}

	return ret;
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
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >* list = GetSpawnLocationList(type);
	if (!list)
		return;

	// TODO: For instances load spawns that should be removed

	std::map<uint32_t, bool> processed_spawn_locations; // placement_id, bool (true for processed)
	for (std::pair<uint32_t, std::shared_ptr<SpawnLocation> > kvp : *list) {
		std::shared_ptr<SpawnLocation>& sl = kvp.second;
		if (!sl)
			continue;

		// If the placement id is in this map it means we already processed it with
		// another spawn in the same spawn group so don't process it again
		if (processed_spawn_locations.count(sl->placement_id) > 0)
			continue;

		// Spawn group stuff
		if (m_spawnLocationGroups.count(sl->placement_id) > 0) {
			uint32_t group_id = CalculateSpawnGroup(sl, type);
			if (group_id > 0) {
				std::map<uint32_t, std::set<uint32_t> >::iterator sga_itr = m_spawnGroupAssociations.find(group_id);
				if (sga_itr != m_spawnGroupAssociations.end()) {

					for (uint32_t id : sga_itr->second) {
						std::map<uint32_t, std::map<uint32_t, uint32_t> >::iterator sgl_itr = m_spawnGroupLocations.find(id);
						if (sgl_itr != m_spawnGroupLocations.end()) {

							for (std::pair<uint32_t, uint32_t> kvp2 : sgl_itr->second) {
								processed_spawn_locations[kvp2.first] = true;
							}	
						}
					}
				}
			}
		}
		else {
			// TODO: instance check to prevent spawns from spawning in the instance
			ProcessSpawnLocation(sl);
		}
	}

}

std::map<uint32_t, std::shared_ptr<SpawnLocation> >* ZoneServer::GetSpawnLocationList(SpawnEntryType type) {
	std::map<uint32_t, std::shared_ptr<SpawnLocation> >* ret = nullptr;
	switch (type) {
	case SpawnEntryType::ENPC:
		ret = &m_npcSpawnLocations;
		break;
	case SpawnEntryType::EOBJECT:
		ret = &m_objectSpawnLocations;
		break;
	case SpawnEntryType::EWIDGET:
		ret = &m_widgetSpawnLocations;
		break;
	case SpawnEntryType::ESIGN:
		ret = &m_signSpawnLocations;
		break;
	case SpawnEntryType::EGROUNDSPAWN:
		ret = &m_groundspawnSpawnLocations;
		break;
	}

	return ret;
}

std::shared_ptr<Spawn> ZoneServer::ProcessSpawnLocation(std::shared_ptr<SpawnLocation> sl, bool respawn) {
	std::shared_ptr<Spawn> ret = nullptr;
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

			ret = spawn;
			break;
		}
		else
			rndNum -= entry->spawn_percentage;
	}

	return ret;
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
		if (spawnEntry->scriptID != 0) {
			npc->SetScriptID(spawnEntry->scriptID);
		}
		else if (spawnLocation->scriptID != 0) {
			npc->SetScriptID(spawnLocation->scriptID);
		}
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

	spawn->CallScript("spawn");
}

void ZoneServer::AddSpawnGroupLocation(uint32_t group_id, uint32_t placement_location_id, uint32_t spawn_location_id) {
	m_spawnGroupLocations[group_id][placement_location_id] = spawn_location_id;
	m_spawnLocationGroups[placement_location_id].push_back(group_id);
	m_spawnGroupAssociations[group_id].insert(group_id);
}

void ZoneServer::AddSpawnGroupChance(uint32_t group_id, float percent) {
	m_spawnGroupChances[group_id] = percent;
}

void ZoneServer::AddSpawnGroupAssociation(uint32_t group_id1, uint32_t group_id2) {
	//Associate groups 1 and 2 now
	std::set<uint32_t>& group_1 = m_spawnGroupAssociations[group_id1];
	std::set<uint32_t>& group_2 = m_spawnGroupAssociations[group_id2];
	group_1.insert(group_id2);
	group_2.insert(group_id1);

	//Associate the remaining groups together
	for (uint32_t id : group_1) {
		group_2.insert(id);
		m_spawnGroupAssociations[id].insert(group_id2);
	}

	for (uint32_t id : group_2) {
		group_1.insert(id);
		m_spawnGroupAssociations[id].insert(group_id1);
	}
}

uint32_t ZoneServer::CalculateSpawnGroup(std::shared_ptr<SpawnLocation> spawnLocation, SpawnEntryType type, bool respawn) {
	uint32_t group = 0;
	std::map<uint32_t, std::list<uint32_t> >::iterator slg_itr = m_spawnLocationGroups.find(spawnLocation->placement_id);
	if (slg_itr == m_spawnLocationGroups.end())
		return group;

	float chance = 0;
	float total_chance = 0;
	std::map<uint32_t, float> tmp_chances; // group_id, chance
	for (uint32_t id : slg_itr->second) {
		if (tmp_chances.count(id) > 0)
			continue;

		std::map<uint32_t, std::set<uint32_t> >::iterator sga_itr = m_spawnGroupAssociations.find(id);
		if (sga_itr != m_spawnGroupAssociations.end()) {

			for (uint32_t group_id : sga_itr->second) {
				std::map<uint32_t, float>::iterator sgc_itr = m_spawnGroupChances.find(group_id);
				if (sgc_itr != m_spawnGroupChances.end()) {
					chance = sgc_itr->second;
					if (chance > 0) {
						total_chance += chance;
						tmp_chances[group_id] = chance;
					}
					else
						tmp_chances[group_id] = 0;
				}
				else
					tmp_chances[group_id] = 0;
			}
		}
		else { //single group, no associations
			std::map<uint32_t, float>::iterator sgc_itr = m_spawnGroupChances.find(id);
			if (sgc_itr != m_spawnGroupChances.end()) {
				chance = sgc_itr->second;
				total_chance += chance;
				tmp_chances[id] = chance;
			}
			else
				tmp_chances[id] = 0;
		}
	}

	if (tmp_chances.size() > 1) {
		//set the default for any chances not set
		for (std::pair<uint32_t, float> kvp : tmp_chances) {
			if (kvp.second == 0) {
				chance = (float)(100 / tmp_chances.size());
				total_chance += chance;
				tmp_chances[kvp.first] = chance;
			}
		}
	}

	if (tmp_chances.size() > 1) {
		float roll = MakeRandom(0, total_chance);
		for (std::pair<uint32_t, float> kvp : tmp_chances) {
			if (kvp.second >= roll) {
				group = kvp.first;
				break;
			}
			else
				roll -= kvp.second;
		}
	}
	else if (tmp_chances.size() == 1)
		group = tmp_chances.begin()->first;

	if (group > 0) {
		std::map<uint32_t, std::map<uint32_t, uint32_t> >::iterator sgl_itr = m_spawnGroupLocations.find(group);
		if (sgl_itr != m_spawnGroupLocations.end()) {
			std::shared_ptr<SpawnGroupList> group_list = std::make_shared<SpawnGroupList>();

			for (std::pair<uint32_t, uint32_t> locations : sgl_itr->second) {
				std::map<uint32_t, std::shared_ptr<SpawnLocation> >* list = GetSpawnLocationList(type);
				if (!list)
					continue;

				std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator sl_itr = list->find(locations.second);
				if (sl_itr != list->end()) {
					std::shared_ptr<Spawn> spawn = ProcessSpawnLocation(sl_itr->second, respawn);
					if (spawn) {
						spawn->SetSpawnGroupID(group);
						group_list->AddSpawn(spawn);
						spawn->SetSpawnGroupList(group_list);
					}
				}
			}
		}
	}

	return group;
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
	// If Player deactivate old cells and activate new
	if (spawn->IsPlayer()) {
		std::shared_ptr<Client> client = GetClientForSpawn(spawn);
		if (client) {
			ActivateCellsForClient(client);
			TryDeactivateCellsForClient(client, oldCellCoord);
		}
	}

	// If not a player and not a global spawn move the spawn to the new cell
	if (!spawn->IsPlayer() && !spawn->IsGlobalSpawn()) {
		std::shared_ptr<Cell> newCell = GetCell(spawn->GetCellCoordinates());
		std::shared_ptr<Cell> oldCell = GetCell(oldCellCoord);

		newCell->AddSpawn(spawn);
		oldCell->RemoveSpawn(spawn);

		// loop to send or remove spawn from client depending on the distance
		for (std::pair<uint32_t, std::weak_ptr<Client> > kvp : Clients) {
			std::shared_ptr<Client> client = kvp.second.lock();
			if (client) {
				// send destroy if now out of range of a client
				if (client->WasSentSpawn(spawn)) {
					if (GetCellDistance(client->GetController()->GetControlled()->GetCellCoordinates(), newCell->GetCellCoordinates()) >= 2)
						SendDestroyGhost(client, spawn);
				}

				// if new cell is active send it to players that need it
				if (newCell->IsActive()) {
					if (!client->WasSentSpawn(spawn)) {
						if (GetCellDistance(client->GetController()->GetControlled()->GetCellCoordinates(), newCell->GetCellCoordinates()) < 2)
							SendSpawnToClient(spawn, client);
					}
				}
			}
		}
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
	std::map<uint32_t, std::weak_ptr<Client> >::iterator itr = m_playerClientList.find(spawn->GetServerID());
	if (itr != m_playerClientList.end())
		return itr->second.lock();

	return nullptr;
}

std::shared_ptr<Entity> ZoneServer::GetPlayerEntityByName(std::string player) {
	for (std::shared_ptr<Entity> p : players) {
		if (strcasecmp(p->GetName().c_str(), player.c_str()) == 0) {
			return p;
		}
	}

	return nullptr;
}

void ZoneServer::Depop() {
	for (std::pair<uint32_t, std::weak_ptr<Client> > c : Clients) {
		std::shared_ptr<Client> client = c.second.lock();
		if (client) {
			std::vector<std::shared_ptr<Cell> > cells = GetNeighboringCells(client->GetController()->GetControlled()->GetCellCoordinates());
			for (std::shared_ptr<Cell> cell : cells)
				cell->SendRemoveSpawnsForClient(client);

			// TODO: Global spawns
		}
	}

	for (std::pair<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> > c : m_spGrid) {
		c.second->ClearSpawnList();
	}

	m_entityList.clear();
	m_objectList.clear();
	m_signList.clear();
	m_widgetList.clear();
	m_groundspawnList.clear();
}

void ZoneServer::LoadSpawns() {
	auto SpawnInfoLoading = [this] {
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
	};

	//Launch a child thread to help split up the work
	std::thread loadHelper = ThreadManager::ThreadStart("ZoneServer::LoadSpawnsHelper", std::bind(SpawnInfoLoading));

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

	database.LoadSpawnLocationGroups(this);
	database.LoadSpawnGroupChances(this);
	database.LoadSpawnLocationGroupAssociations(this);

	loadHelper.join();

	//Now that the static info is loaded, populate the zone
	ProcessSpawnLocations();

	LoadScript();
	CallScript("init_zone_script");
}

std::shared_ptr<EmuLuaState> ZoneServer::LoadSpawnState(uint32_t id) {
	auto itr = m_spawnStates.find(id);
	if (itr != m_spawnStates.end()) {
		//We have already loaded this script, return it
		return itr->second;
	}

	//We need to load this script, cache it for later as well
	auto state = LuaInterface::LoadSpawnScript(id);
	if (state) {
		m_spawnStates[id] = state;
	}

	return state;
}

void ZoneServer::LoadScript() {
	m_luaState.reset();

	if (scriptID == 0) {
		return;
	}

	m_luaState = LuaInterface::LoadZoneScript(scriptID);
}

void ZoneServer::CallScript(const char* function, const std::shared_ptr<Spawn>& spawnArg, uint32_t gridID) {
	if (!m_luaState) {
		return;
	}

	std::shared_ptr<EmuLuaState> recursiveState;
	lua_State* state = m_luaState->state;

	lua_getglobal(state, function);

	if (!lua_isfunction(state, lua_gettop(state))) {
		//This function does not exist in the script, we don't need to spam an error here just return
		lua_pop(state, 1);
		return;
	}

	//NOTE: If we ever have more than one thread that could access this at once we'll need a recursive mutex on the EmuLuaState
	if (m_luaState->nUsers++) {
		lua_pop(state, 1);

		//We're calling this script recursively. Load a new copy of the state
		recursiveState = LuaInterface::LoadZoneScript(scriptID);
		if (!recursiveState) {
			--m_luaState->nUsers;
			return;
		}

		state = recursiveState->state;
	}

	LuaInterface::PushLuaZone(state, shared_from_this());
	int32_t nArgs = 1;
	if (spawnArg) {
		LuaInterface::PushLuaSpawn(state, spawnArg);
		++nArgs;
	}

	if (gridID > 0) {
		LuaInterface::PushLuaUInt32(state, gridID);
		++nArgs;
	}

	if (lua_pcall(state, nArgs, 0, 0) != LUA_OK) {
		LuaInterface::PrintStateError(state);
	}

	lua_settop(state, 0);

	--m_luaState->nUsers;
}

void ZoneServer::CheckNeededLoads() {
	ZoneLoadFlags flags = m_loadFlags;
	m_loadFlagsByte = 0;

	std::optional<std::thread> spawnThread;

	auto OptionalJoin = [](std::optional<std::thread>& t) {
		if (t) t->join();
	};

	if (flags.bNeedsSpawnLoad) {
		spawnThread = ThreadManager::ThreadStart("ZoneServer::LoadSpawns", std::bind(&ZoneServer::LoadSpawns, this));
	}

	OptionalJoin(spawnThread);
}