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
	pendingClientAdd.push_back(c);
	lock.Unlock();

	OP_ZoneInfoMsg_Packet* zone = new OP_ZoneInfoMsg_Packet(c->GetVersion());
	zone->server1 = "Test";
	zone->server2 = "Test";
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
	pendingClientRemoval.push_back(client);
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

void ZoneServer::LoadThread() {
	LogInfo(LOG_NPC, 0, "-Loading NPC data...");
	database.LoadNPCsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load NPC data complete!");

	LogInfo(LOG_NPC, 0, "-Loading Object data...");
	database.LoadObjectsForZone(this);
	LogInfo(LOG_NPC, 0, "-Load Object data complete!");
}