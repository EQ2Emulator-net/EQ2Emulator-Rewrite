#include <stdafx.h>

#include "ZoneServer.h"
#include "../../common/log.h"
#include "../Database/ZoneDatabase.h"
#include "ZoneOperator.h"
#include "Client.h"
#include "../../common/thread.h"

// Packets
#include "../Packets/OP_ZoneInfoMsg_Packet.h"
#include "../Packets/OP_SetRemoteCmdsMsg_Packet.h"
#include "../Packets/OP_CreateGhostCmd_Packet.h"

// Spawns
#include "../Spawns/Spawn.h"
#include "../Spawns/Entity.h"
#include "../Spawns/Object.h"
#include "../Spawns/Widget.h"
#include "../Spawns/Sign.h"

extern ZoneDatabase database;
extern ZoneOperator z;

ZoneServer::ZoneServer(uint32_t zone_id) {
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

	LogDebug(LOG_ZONE, 0, "Zone %u (%s) started", id, description.c_str());

	process_thread = ThreadManager::ThreadStart("ZoneServer::Process", std::bind(&ZoneServer::Process, this));

	return ret;
}

void ZoneServer::Process() {

	while (isRunning) {
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

		std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Probably play with this value
	}
}

bool ZoneServer::AddClient(std::weak_ptr<Client> client) {
	std::shared_ptr<Client> c = client.lock();
	if (c) {
		c->SetZone(shared_from_this());
		Clients[c->GetAccountID()] = client;

		OP_ZoneInfoMsg_Packet* zone = new OP_ZoneInfoMsg_Packet(c->GetVersion());
		zone->server1 = "Test";
		zone->server2 = "Test";
		zone->zone = file;
		zone->zone2 = name;
		zone->zone_desc = description;
		zone->char_name = "Foof";
		zone->x = 242.55f;
		zone->y = 12.21f;
		zone->z = -978.47f;
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
	}

	OP_SetRemoteCmdsMsg_Packet* commands = z.GetCommandsPacket(c->GetVersion());
	c->QueuePacket(commands);

	return true;
}

void ZoneServer::SendCharacterInfo(std::shared_ptr<Client> client) {
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	database.LoadCharacter(client->GetCharacterID(), client->GetAccountID(), entity);

	players.push_back(entity);

	// Set this in the spawn constructor
	entity->SetState(16512, false);
	entity->SetSize(0, false);
	entity->SetCollisionRadius(28, false);
	entity->SetSizeRatio(1.0f, false);
	entity->SetSizeMultiplierRatio(1.0f, false);
	entity->SetVisFlags(55, false);
	entity->SetInteractionFlag(12);

	OP_CreateGhostCmd_Packet* ghost = new OP_CreateGhostCmd_Packet(client->GetVersion());
	ghost->InsertSpawnData(entity, 1);
	ghost->SetEncodedBuffers(client, ghost->header.index);
	client->QueuePacket(ghost);
}