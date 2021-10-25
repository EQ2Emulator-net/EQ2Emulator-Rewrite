#include <stdafx.h>

#include "ZoneServer.h"
#include "../../common/log.h"
#include "../Database/ZoneDatabase.h"
#include "ZoneOperator.h"
#include "Client.h"
#include "../../common/thread.h"
#include "../Controllers/PlayerController.h"
#include "../Controllers/NPCController.h"
#include "../Commands/CommandProcess.h"
#include "Cell.h"
#include "../../common/string.h"
#include "../Lua/LuaInterface.h"
#include "../Lua/LuaGlobals.h"
#include "MasterZoneLookup.h"
#include "../WorldTalk/WorldStream.h"
#include "../Players/SpawnCampDebug.h"
#include "../../common/Rules.h"
#include "../Skills/MasterSkillList.h"

// Packets
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_CreateGhostCmd_Packet.h"
#include "../Packets/OP_CreateSignWidgetCmd_Packet.h"
#include "../Packets/OP_CreateWidgetCmd_Packet.h"
#include "../Packets/OP_EqDestroyGhostCmd_Packet.h"
#include "../Packets/OP_MapFogDataInitMsg_Packet.h"
#include "../Packets/OP_BioUpdateMsg_Packet.h"
#include "../Packets/OP_PopulateSkillMapsMsg.h"
#include "../../common/Packets/EmuPackets/Emu_ClientSessionEnded_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_NotifyCharacterLinkdead_Packet.h"
#include "../../common/Packets/EmuPackets/Emu_CharacterLinkdeadTimeout_Packet.h"

// Spawns
#include "../Spawns/Spawn.h"
#include "../Spawns/Entity.h"
#include "../Spawns/Object.h"
#include "../Spawns/Widget.h"
#include "../Spawns/Sign.h"
#include "../Spawns/GroundSpawn.h"
#include "SpawnCamp.h"

extern ZoneDatabase database;
extern ZoneOperator g_zoneOperator;
extern CommandProcess g_commandProcess;
extern LuaGlobals g_luaGlobals;
extern MasterZoneLookup g_masterZoneLookup;
extern RuleManager g_ruleManager;
extern MasterSkillList g_masterSkillList;

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
	//5 minutes
	m_saveCharactersTimer.Start(60000 * 5);

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
			//Make a copy in case any players are removed from the process
			auto players = this->players;
			for (std::shared_ptr<Entity> player : players) {
				player->Process();
			}

			for (std::pair<std::pair<int32_t, int32_t>, std::shared_ptr<Cell> > kvp : m_spGrid) {
				if (kvp.second->IsActive())
					kvp.second->Process();
			}

			// Process spawn camps
			for (std::pair<uint32_t, std::shared_ptr<SpawnCamp> > camp : m_spawnCamps) {
				camp.second->Process();
			}
		}

		if (m_saveCharactersTimer.Check()) {
			SaveCharactersInZone();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Probably play with this value
	}
}

bool ZoneServer::AddClient(std::shared_ptr<Client> c, bool bInitialLogin) {
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
	zone->account_flags_1 = 0x460007ff81;
	zone->account_flags_2 = 0x7fffff1b5fffbf;
	zone->auction_website = "eq2emulator.net";
	zone->auction_port = 80;
	zone->upload_page = "test_upload.m";
	zone->upload_key = "dsya987yda9";
	zone->unknown7[0] = 1.0;
	zone->unknown7[1] = 1.0;
	zone->unknown9 = 13.f;

	// 1<<1 = arena, 1<<2 = pvp, 1<<9 = battlegrounds (hides most eq2 button menu options), 1<<10 show battlegrounds leaderboard, 1<<11 = dungeon maker zone, 1<<12 = dungeon maker build mode
	//Guesses : 1<<5 = instance, 1<<6 = no mounts
	//1<<7 and 1<<8 are values that seem to usually be sent but not sure what they do, maybe allowed illusions/mercs/size mods
	zone->zone_flags = 1 << 7 | 1 << 8;

	zone->unknown11 = -4;

	zone->client_cmd_array.emplace_back(c->GetVersion());
	zone->client_cmd_array.back().client_cmd = "census_paperdoll_enabled 0"; //not sure if paperdoll images are sent on a separate thread that isn't sync'd or what but they are BUGGY
	zone->client_cmd_array.emplace_back(c->GetVersion());
	zone->client_cmd_array.back().client_cmd = "chat_linkbrackets_item 1";

	if (zone->GetVersion() >= 67650) {
		//The expansion bytes changed from 2 8 byte vals to 2 9 byte arrays here
		//MOST flags are server side but a few do have client changes, such as enabling pvp interfaces or the prison servers
		static const unsigned char unk4Bytes[] = { 0x7F, 0xFF, 0xF8, 0x90, 0x44, 0x94, 0x00, 0x10, 0x10 };
		static const unsigned char unk7Bytes[] = { 0x7F, 0xFF, 0xFB, 0xF6, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF };

		zone->account_flags_1_string.assign(reinterpret_cast<const char*>(unk4Bytes), 9);
		zone->account_flags_2_string.assign(reinterpret_cast<const char*>(unk7Bytes), 9);
	}

	c->QueuePacket(zone);

	if (bInitialLogin) {
		g_commandProcess.SendCommandList(c);
		g_masterSkillList.SendSkillMapPacket(c);
	}

	//Send any map data we may have for this zone
	if (auto info = g_masterZoneLookup.GetZoneInfoByID(GetID())) {	
		if (info->mapData) {
			OP_MapFogDataInitMsg_Packet p(c->GetVersion());
			p.InsertMapData(*info->mapData);
			c->QueuePacket(p);
		}
	}

	return true;
}

std::shared_ptr<Entity> ZoneServer::LoadCharacter(const std::shared_ptr<Client>& client) {
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	std::shared_ptr<PlayerController> controller = client->GetController();
	controller->Possess(entity);
	entity->SetController(controller);
	CharacterSheet* sheet = controller->GetCharacterSheet();
	database.LoadCharacter(client->GetCharacterID(), client->GetAccountID(), entity, *sheet);
	entity->SetZone(shared_from_this());

	uint32_t oldZoneID = sheet->zoneID;

	sheet->zoneID = GetID();
	sheet->instanceID = GetInstanceID();

	entity->SetIsPlayer(true, false);

	// Set this in the spawn constructor
	entity->SetState(16512, false);
	entity->SetSize(0.875f, false);
	entity->SetCollisionRadius(1.0f, false);
	entity->SetSizeRatio(1.0f, false);
	entity->SetInteractionFlag(12, false);
	if (oldZoneID != GetID()) {
		//This player changed zones, update their location
		entity->SetLocation(GetSafeX(), GetSafeY(), GetSafeZ(), false);
		entity->SetHeading(GetSafeHeading(), false);
	}
	entity->SetMovementMode(0, false);

	return entity;
}

std::shared_ptr<Entity> ZoneServer::FindLinkdeadCharacter(uint32_t characterID) {
	for (auto& itr : players) {
		if (auto pc = std::dynamic_pointer_cast<PlayerController>(itr->GetController())) {
			if (pc->GetCharacterSheet()->characterID == characterID) {
				return itr;
			}
		}
	}

	return std::shared_ptr<Entity>();
}

void ZoneServer::SendCharacterInfo(std::shared_ptr<Client> client) {
	std::shared_ptr<Entity> entity = FindLinkdeadCharacter(client->GetCharacterID());

	bool bLinkdead = entity != nullptr;
	if (bLinkdead) {
		//Allow this new client to take over the ghost and character sheet of the existing linkdead character
		entity->DisableEntityFlags(EntityFlagLinkdead);
		auto controller = client->GetController();
		auto old_controller = dynamic_pointer_cast<PlayerController>(entity->GetController());
		controller->MoveCharacterSheetFrom(old_controller->GetCharacterSheet());
		controller->Possess(entity);
		entity->SetController(controller);
	}
	else {
		//Load a new character ghost from the DB
		entity = LoadCharacter(client);
	}

	m_playerClientList[entity->GetServerID()] = client;
	std::pair<int32_t, int32_t> cellCoords = entity->GetCellCoordinates();
	LogWarn(LOG_PLAYER, 0, "New player in cell (%i, %i) player loc = (%f, %f, %f)", cellCoords.first, cellCoords.second, entity->GetX(), entity->GetY(), entity->GetZ());
	cellCoords = GetCellCoordinatesForSpawn(entity);
	LogWarn(LOG_PLAYER, 0, "Cell coords check 2 (%i, %i)", cellCoords.first, cellCoords.second);

	CharacterSheet* sheet = client->GetController()->GetCharacterSheet();
	OP_BioUpdateMsg_Packet b(client->GetVersion());
	b.biography = sheet->biography;
	client->QueuePacket(b);

	SendSpawnToClient(entity, client);
	if (!bLinkdead) {
		AddPlayer(entity);
	}
	SendPlayersToNewClient(client);
	ActivateCellsForClient(client);
}

void ZoneServer::AddPlayer(std::shared_ptr<Entity> player) {
	players.push_back(player);

	for (std::pair<uint32_t, std::weak_ptr<Client> > c : Clients) {
		std::shared_ptr<Client> client = c.second.lock();
		//Wait to send this ghost until the other client has a player, that is needed for the vis struct
		if (client && client->GetController()->GetControlled()) {
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
		OP_CreateSignWidgetCmd_Packet* sign = new OP_CreateSignWidgetCmd_Packet(client->GetVersion());
		ghost = sign;
		sign->signData.title = spawn->GetSignData()->GetTitle();
		sign->signData.description = spawn->GetSignData()->GetDescription();
		sign->signData.distance = spawn->GetSignData()->GetDistance();
		sign->signData.show = true;
		// language...
	}
	if (spawn->GetWidgetData()) {
		OP_CreateWidgetCmd_Packet* widget;
		if (ghost) {
			widget = static_cast<OP_CreateWidgetCmd_Packet*>(ghost);		
		}
		else {
			widget = new OP_CreateWidgetCmd_Packet(client->GetVersion());
			ghost = widget;
		}
		widget->widgetData.widgetID = spawn->GetWidgetData()->GetWidgetID();
		widget->widgetData.widgetX = spawn->GetWidgetData()->GetWidgetX();
		widget->widgetData.widgetY = spawn->GetWidgetData()->GetWidgetY();
		widget->widgetData.widgetZ = spawn->GetWidgetData()->GetWidgetZ();
	}
	
	if (!ghost) {
		ghost = new OP_CreateGhostCmd_Packet(client->GetVersion());
	}

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

	uint16_t index = client->GetIndexForSpawn(spawn);
	OP_EqDestroyGhostCmd_Packet* p = new OP_EqDestroyGhostCmd_Packet(client->GetVersion());
	p->spawn_index = index;
	//This value doesn't actually do anything anymore (not sure if it has since the emu started)
	//So instead do not remove the XOR buffers, just reuse them for the next ghost at this index.
	p->bDelete = 1;
	client->QueuePacket(p);

	client->RemoveSpawnFromIndexMap(spawn);
	//client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnInfo, index);
	//client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnPos, index);
	//client->encoded_packets.RemoveBuffer(EEncodedPackets::EEncoded_UpdateSpawnVis, index);

	spawn->RemoveClient(client);
}

void ZoneServer::RemoveSpawn(std::shared_ptr<Spawn> spawn) {
	// Remove spawn from the lists
	if (spawn->IsSign()) {
		std::vector<std::shared_ptr<Spawn> >::iterator sign_itr;
		for (sign_itr = m_signList.begin(); sign_itr != m_signList.end(); sign_itr++) {
			if (spawn == *sign_itr) {
				m_signList.erase(sign_itr);
				break;
			}
		}
	}
	else if (spawn->IsWidget()) {
		std::vector<std::shared_ptr<Spawn> >::iterator widget_itr;
		for (widget_itr = m_widgetList.begin(); widget_itr != m_widgetList.end(); widget_itr++) {
			if (spawn == *widget_itr) {
				m_widgetList.erase(widget_itr);
				break;
			}
		}
	}
	else if (spawn->IsObject()) {
		std::vector<std::shared_ptr<Object> >::iterator object_itr;
		for (object_itr = m_objectList.begin(); object_itr != m_objectList.end(); object_itr++) {
			if (spawn == *object_itr) {
				m_objectList.erase(object_itr);
				break;
			}
		}
	}
	else if (spawn->IsGroundSpawn()) {
		std::vector<std::shared_ptr<GroundSpawn> >::iterator ground_itr;
		for (ground_itr = m_groundspawnList.begin(); ground_itr != m_groundspawnList.end(); ground_itr++) {
			if (spawn == *ground_itr) {
				m_groundspawnList.erase(ground_itr);
				break;
			}
		}
	}
	else {
		std::vector<std::shared_ptr<Entity> >::iterator entity_itr;
		for (entity_itr = m_entityList.begin(); entity_itr != m_entityList.end(); entity_itr++) {
			if (spawn == *entity_itr) {
				m_entityList.erase(entity_itr);
				break;
			}
		}
	}

	// Remove from global list or from cell list
	if (spawn->IsGlobalSpawn()) {
		std::vector<std::weak_ptr<Spawn> >::iterator global_itr;
		for (global_itr = m_globalSpawns.begin(); global_itr != m_globalSpawns.end(); global_itr++) {
			if (EmuWeakCmp(spawn, (*global_itr))) {
				m_globalSpawns.erase(global_itr);
				break;
			}
		}
	}
	else {
		std::shared_ptr<Cell> cell = GetCell(spawn->GetCellCoordinates());
		cell->RemoveSpawn(spawn);
	}

	// Remove from all clients
	RemoveSpawnFromAllClients(spawn);
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
	auto controller = client->GetController();
	std::shared_ptr<Entity> player = std::static_pointer_cast<Entity>(controller->GetControlled());

	auto ws = g_zoneOperator.GetWorldStream();

	if (player) {
		//Check if we were expecting this disconnect, otherwise start a linkdead timer
		if (client->bZoningDisconnect || player->IsCamping()) {
			RemovePlayer(player);
		}
		else if (!player->IsLinkdead()){
			static const uint32_t linkdeadTimeoutMS = g_ruleManager.GetGlobalRule(ERuleCategory::R_World, ERuleType::LinkDeadTimer)->GetUInt32();
			player->SetActivityTimer(Timer::GetServerTime() + linkdeadTimeoutMS);
			player->EnableEntityFlags(EntityFlagLinkdead);

			if (ws) {
				auto p = new Emu_NotifyCharacterLinkdead_Packet;
				p->characterID = client->GetCharacterID();
				ws->QueuePacket(p);
			}
		}
		//If the player disconnected from zoning we have already saved it so let the new zone take over
		if (!client->bZoningDisconnect) {
			if (auto charSheet = controller->GetCharacterSheet()) {
				database.SaveSingleCharacter(*charSheet);
			}
		}
		m_playerClientList.erase(player->GetServerID());
	}
	else if (ws) {
		//Player made it to the login function but not fully loaded before disconnecting, let the world server know to mark their char offline
		auto p = new Emu_CharacterLinkdeadTimeout_Packet;
		p->characterID = client->GetCharacterID();
		ws->QueuePacket(p);
	}

	if (ws) {
		auto p = new Emu_ClientSessionEnded_Packet;
		p->sessionID = client->GetSessionID();
		ws->QueuePacket(p);
	}
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

	if (!ret) {
		if (database.LoadObject(this, databaseID))
			ret = GetNewObject(databaseID);
	}

	if (!ret) {
		if (database.LoadWidget(this, databaseID))
			ret = GetNewWidget(databaseID);
	}

	if (!ret) {
		if (database.LoadSign(this, databaseID))
			ret = GetNewSign(databaseID);
	}

	if (!ret) {
		if (database.LoadGroundSpawn(this, databaseID))
			ret = GetNewGroundSpawn(databaseID);
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

		// If spawn was merged into a camp then skip
		if (entry->spawn_camp_id != 0)
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

	if (!spawn->GetController()) {
		std::shared_ptr<NPCController> controller = std::make_shared<NPCController>();
		controller->Possess(spawn);
		spawn->SetController(controller);
	}

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
	if (spawn->IsPlayer() || spawn->IsGlobalSpawn()) {
		LogError(LOG_ZONE, 0, "Tried to add a player or global spawn to a cell.");
		return;
	}

	std::shared_ptr<Cell> cell = GetCell(cellCoords);
	if (cell)
		cell->AddSpawn(spawn);

	// Send spawn to clients if cell is active
	if (cell->IsActive()) {
		for (std::pair<uint32_t, std::weak_ptr<Client> > kvp : Clients) {
			std::shared_ptr<Client> client = kvp.second.lock();
			if (client) {
				std::shared_ptr<PlayerController> controller = client->GetController();
				if (!controller)
					continue;

				std::shared_ptr<Spawn> player = controller->GetControlled();
				if (!player)
					continue;

				if (!client->WasSentSpawn(spawn)) {
					if (GetCellDistance(player->GetCellCoordinates(), cell->GetCellCoordinates()) < 2)
						SendSpawnToClient(spawn, client);
				}
			}
		}
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

	std::shared_ptr<Spawn> player = controller->GetControlled();
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

		AddSpawnToCell(spawn, newCell->GetCellCoordinates());
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
				 /*
				 * Moved to AddSpawnToCell()
				 * 
				// if new cell is active send it to players that need it
				if (newCell->IsActive()) {
					if (!client->WasSentSpawn(spawn)) {
						if (GetCellDistance(client->GetController()->GetControlled()->GetCellCoordinates(), newCell->GetCellCoordinates()) < 2)
							SendSpawnToClient(spawn, client);
					}
				}*/
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

	for (std::pair<uint32_t, std::shared_ptr<SpawnCamp> > sc : m_spawnCamps)
		sc.second->Reset();
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

	LogInfo(LOG_NPC, 0, "-Loading SpawnCamp data...");
	database.LoadSpawnCampsForZone(shared_from_this());
	LogInfo(LOG_NPC, 0, "-Load SpawnCamp data complete!");

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
		lua_getglobal(state, function);
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

void ZoneServer::SaveCharactersInZone() {
	if (Clients.empty()) {
		return;
	}

	uint32_t count = 0;

	std::ostringstream query;

	for (auto& itr : Clients) {
		auto client = itr.second.lock();
		if (!client) {
			continue;
		}

		CharacterSheet* charSheet = client->GetController()->GetCharacterSheet();

		if (!charSheet) {
			continue;
		}

		if (charSheet->updates.GenerateUpdate(query)) {
			++count;
		}
	}

	if (count == 0) {
		return;
	}

	std::string updates = query.str();

	auto saveHelper = [](std::string updates, uint32_t count) {
		LogInfo(LOG_ZONE, 0, "Pushing updates for %u characters to the database.", count);
		database.Query(updates.c_str());
	};

	//Detach a thread to run these queries since updates can be slow with several players
	ThreadManager::ThreadStart("ZoneServer::SaveCharactersInZone", std::bind(saveHelper, std::move(updates), count)).detach();
}

std::shared_ptr<SpawnCamp> ZoneServer::GetSpawnCamp(uint32_t id) {
	std::map<uint32_t, std::shared_ptr<SpawnCamp> >::iterator itr = m_spawnCamps.find(id);
	if (itr != m_spawnCamps.end())
		return itr->second;

	return nullptr;
}

void ZoneServer::AddSpawnCamp(std::shared_ptr<SpawnCamp> camp) {
	m_spawnCamps[camp->GetID()] = camp;

	//std::map<uint32_t, std::weak_ptr<Client> > Clients;
	for (std::pair<uint32_t, std::weak_ptr<Client> > kvp : Clients) {
		std::shared_ptr<Client> client = kvp.second.lock();
		if (client) {
			std::shared_ptr<PlayerController> controller = client->GetController();
			if (controller) {
				std::shared_ptr<SpawnCampDebug> scd = controller->GetSpawnCampDebug();
				if (scd) {
					scd->AddSpawnCamp(camp);
				}
			}
		}
	}
}

void ZoneServer::DeleteSpawnFromLocation(std::shared_ptr<Spawn> spawn, bool remove) {
	if (database.RemoveSpawnFromSpawnLocation(spawn)) {
		uint32_t entryID = spawn->GetSpawnEntryID();
		uint32_t locationID = spawn->GetSpawnLocationID();
		uint32_t spawnID = spawn->GetDatabaseID();

		std::map<uint32_t, std::shared_ptr<SpawnLocation> >* list = nullptr;
		if (spawn->IsObject()) {
			list = GetSpawnLocationList(SpawnEntryType::EOBJECT);
			m_masterObjectList.erase(spawnID);
		}
		else if (spawn->IsSign()) {
			list = GetSpawnLocationList(SpawnEntryType::ESIGN);
			m_masterSignList.erase(spawnID);
		}
		else if (spawn->IsWidget()) {
			list = GetSpawnLocationList(SpawnEntryType::EWIDGET);
			m_masterWidgetList.erase(spawnID);
		}
		else if (spawn->IsGroundSpawn()) {
			list = GetSpawnLocationList(SpawnEntryType::EGROUNDSPAWN);
			m_masterGroundSpawnList.erase(spawnID);
		}
		else if (spawn->IsNPC()) {
			list = GetSpawnLocationList(SpawnEntryType::ENPC);
			m_masterNPCList.erase(spawnID);
		}

		if (list != nullptr) {
			std::map<uint32_t, std::shared_ptr<SpawnLocation> >::iterator find_itr = list->find(locationID);
			if (find_itr != list->end())
				find_itr->second->RemoveEntry(entryID);
		}

		if (remove)
			RemoveSpawn(spawn);
		else
			spawn->SetSpawnLocationID(0);
	}
}