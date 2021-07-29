#include "stdafx.h"

#include "NPCPathDebug.h"
#include "../Spawns/PathPointSpawn.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/BaseController.h"
#include "../Controllers/PlayerController.h"
#include "../Controllers/NPCController.h"
#include "../AI/NPCMovement.h"
#include "../ZoneServer/ZoneServer.h"
#include "../../common/log.h"

#include <sstream>

bool NPCPathDebug::Process() {
	bool ret = true;

	for (std::shared_ptr<Spawn> s : m_locationSpawns)
		s->Process();

	return ret;
}

void NPCPathDebug::Start(std::shared_ptr<Spawn> target) {
	std::shared_ptr<Spawn> oldTarget = m_targetSpawn.lock();
	
	if (oldTarget && oldTarget == target) {
		SendClientMessage("You are already viewing this NPC's path");
		return;
	}

	if (oldTarget) {
		LogDebug(LOG_NPC, 0, "NPCPathDebug: old spawn found, clearing spawns");
		End();
	}

	m_targetSpawn = target;
	std::shared_ptr<NPCController> controller = std::dynamic_pointer_cast<NPCController>(target->GetController());
	if (controller) {
		m_locations = controller->GetNPCMovement()->GetLocations();

		// Create spawns for the locations if there are any
		if (GenerateSpawns()) {
			SendClientMessage("Showing the path for " + target->GetName());
			SendLocationSpawnsToClient();
		}
		else
			SendClientMessage("No path for " + target->GetName());
	}
}

bool NPCPathDebug::GenerateSpawns() {
	bool ret = false;
	if (m_locations.size() > 0) {
		uint32_t index = 0;
		for (std::shared_ptr<MovementLocationInfo> info : m_locations) {
			std::shared_ptr<PathPointSpawn> s = CreateLocationSpawn(info, ++index);
			if (s) {
				m_locationSpawns.push_back(s);
				LogDebug(LOG_NPC, 0, "NPCPathDebug: spawn created for location");
			}
		}

		ret = true;
	}

	return ret;
}

void NPCPathDebug::End() {
	std::shared_ptr<Spawn> target = m_targetSpawn.lock();
	if (target)
		SendClientMessage("Hiding the path for " + target->GetName());
	else
		SendClientMessage("Hiding the path for unknown");

	RemoveLocationsFromClient();
	ClearLocations();
	m_targetSpawn.reset();
}

void NPCPathDebug::SendLocationSpawnsToClient() {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client)
		return;

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;

	
	for (std::shared_ptr<Spawn> s : m_locationSpawns) {
		LogDebug(LOG_NPC, 0, "NPCPathDebug: sending location spawn %s to client", s->GetName().c_str());
		zone->SendSpawnToClient(s, client);
	}
}

void NPCPathDebug::RemoveLocationsFromClient() {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client)
		return;

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;


	for (std::shared_ptr<Spawn> s : m_locationSpawns) {
		zone->RemoveSpawnFromClient(s, client);
	}
}

void NPCPathDebug::ClearLocations() {
	m_locations.clear();
	m_locationSpawns.clear();
}

void NPCPathDebug::AddPathPoint() {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client)
		return;

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;

	std::shared_ptr<Spawn> player = client->GetController()->GetControlled();
	if (!player)
		return;

	std::shared_ptr<Spawn> target = m_targetSpawn.lock();
	if (!target)
		return;

	uint32_t index = m_locationSpawns.size() + 1;
	std::shared_ptr<MovementLocationInfo> loc = std::make_shared<MovementLocationInfo>();
	loc->x = player->GetX();
	loc->y = player->GetY();
	loc->z = player->GetZ();
	loc->speed = 2.0f;
	loc->delay = 0.0f;
	loc->callback = "";

	std::shared_ptr<PathPointSpawn> spawn = CreateLocationSpawn(loc, index);
	spawn->SetGridID(player->GetGridID(), false);
	m_locationSpawns.push_back(spawn);
	m_locations.push_back(loc);
	zone->SendSpawnToClient(spawn, client);

	std::string msg = "Added point " + std::to_string(index) + " to " + target->GetName() + "'s path";
	SendClientMessage(msg);
}

void NPCPathDebug::RemovePathPoint(std::shared_ptr<PathPointSpawn> spawn) {
	std::shared_ptr<Client> client = m_client.lock();
	if (!client)
		return;

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	if (!zone)
		return;

	uint32_t index = spawn->GetPointIndex();
	if (index == 0)
		return;

	// reduce the index by 1 to make it zero based
	index--;
	
	// Erase the location from the path
	m_locations.erase(m_locations.begin() + index);

	// Nuke spawns from client
	RemoveLocationsFromClient();

	// Clear the spawn list and regenerate it
	m_locationSpawns.clear();
	if (GenerateSpawns())
		SendLocationSpawnsToClient();

	SendClientMessage("Path point removed.");
}

std::shared_ptr<PathPointSpawn> NPCPathDebug::CreateLocationSpawn(std::shared_ptr<MovementLocationInfo> location, uint32_t pointIndex) {
	std::shared_ptr<Spawn> targetSpawn = m_targetSpawn.lock();
	if (!targetSpawn)
		return nullptr;

	std::shared_ptr<PathPointSpawn> ret = std::make_shared<PathPointSpawn>(pointIndex);

	/*
		Possible model ID's to visualize the path

		273 - willowisp, floats around in the air
		2681 - crystal ball on the floor

		flag colors
		12882 - blue
		12883 - green
		12884 - orange
		12885 - purple
		12886 - red
		12887 - white
		12888 - yellow
	*/

	uint32_t modelID = 12883;
	if (location->delay > 0 && !location->callback.empty())
		modelID = 12885;
	else if (location->delay > 0)
		modelID = 12886;
	else if (!location->callback.empty())
		modelID = 12882;

	ret->SetModelType(modelID, false);
	ret->SetLocation(location->x, location->y, location->z, false);
	ret->SetGridID(targetSpawn->GetGridID(), false);
	ret->SetHeading(MakeRandom(0, 360), false);

	std::string name = "Point " + std::to_string(pointIndex);
	ret->SetName(name, false);

	std::string info = "SPD: " + to_string_with_precision(location->speed) + " DLY: " + to_string_with_precision(location->delay) + (location->callback.empty() ? "" : (" CB: " + location->callback));
	ret->SetGuild(info, false);

	// consider making these default values for spawns
	ret->SetSize(1.0f, false);
	ret->SetSizeRatio(1.0f, false);
	ret->SetCollisionRadius(1.0f, false);
	ret->SetState(16384, false);

	return ret;
}

void NPCPathDebug::SendClientMessage(std::string message, std::string filter) {
	std::shared_ptr<Client> client = m_client.lock();
	if (client)
		client->chat.DisplayText(filter.c_str(), message, 0xff, false, "");
}

void NPCPathDebug::GenerateLua() {
	if (m_locations.size() == 0) {
		SendClientMessage("Generate Lua failed, there are no locations in the path", "Error Text");
		return;
	}

	std::shared_ptr<Spawn> target = m_targetSpawn.lock();
	if (!target) {
		SendClientMessage("Generate Lua failed, unable to get the spawn for this path", "Error Text");
		return;
	}

	std::string lua = "Generated Lua for " + target->GetName() + " (" + to_string(target->GetDatabaseID()) + "):\n";
	for (std::shared_ptr<MovementLocationInfo> loc : m_locations) {
		lua += "\tMovementLoopAddLocation(NPC, " + to_string_with_precision(loc->x) + ", " + to_string_with_precision(loc->y) + ", " + to_string_with_precision(loc->z) + ", " + to_string_with_precision(loc->speed);
		if (loc->delay != 0.0f)
			lua += ", " + to_string_with_precision(loc->delay);
		if (!loc->callback.empty())
			lua += ", \"" + loc->callback + "\"";

		lua += ")\n";
	}

	SendClientMessage(lua);
}

void NPCPathDebug::SetPathPointSpeed(std::shared_ptr<PathPointSpawn> spawn, float speed) {
	uint32_t index = spawn->GetPointIndex() - 1; // - 1 to make it zero based
	if (m_locations.size() <= index) {
		SendClientMessage("Index is out of bounds", "Error Text");
		return;
	}

	std::shared_ptr<MovementLocationInfo> location = m_locations.at(index);
	location->speed = speed;
	UpdatePathPointSpawn(spawn, location);
	SendClientMessage("Set speed to " + to_string_with_precision(speed) + " for point " + to_string(index));
}

void NPCPathDebug::SetPathPointDelay(std::shared_ptr<PathPointSpawn> spawn, float delay) {
	uint32_t index = spawn->GetPointIndex() - 1; // - 1 to make it zero based
	if (m_locations.size() <= index) {
		SendClientMessage("Index is out of bounds", "Error Text");
		return;
	}

	std::shared_ptr<MovementLocationInfo> location = m_locations.at(index);
	location->delay = delay;
	UpdatePathPointSpawn(spawn, location);
	SendClientMessage("Set delay to " + to_string_with_precision(delay) + " for point " + to_string(index));
}

void NPCPathDebug::SetPathPointCallback(std::shared_ptr<PathPointSpawn> spawn, std::string callback) {
	uint32_t index = spawn->GetPointIndex() - 1; // - 1 to make it zero based
	if (m_locations.size() <= index) {
		SendClientMessage("Index is out of bounds", "Error Text");
		return;
	}

	std::shared_ptr<MovementLocationInfo> location = m_locations.at(index);
	location->callback = callback;
	UpdatePathPointSpawn(spawn, location);
	SendClientMessage("Set callback to " + callback + " for point " + to_string(index));
}

void NPCPathDebug::UpdatePathPointSpawn(std::shared_ptr<PathPointSpawn> spawn, std::shared_ptr<MovementLocationInfo> location) {
	uint32_t modelID = 12883;
	if (location->delay > 0 && !location->callback.empty())
		modelID = 12885;
	else if (location->delay > 0)
		modelID = 12886;
	else if (!location->callback.empty())
		modelID = 12882;

	spawn->SetModelType(modelID);

	std::string info = "SPD: " + to_string_with_precision(location->speed) + " DLY: " + to_string_with_precision(location->delay) + (location->callback.empty() ? "" : (" CB: " + location->callback));
	spawn->SetGuild(info);
}

std::string NPCPathDebug::to_string_with_precision(float a_value, const int n) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}