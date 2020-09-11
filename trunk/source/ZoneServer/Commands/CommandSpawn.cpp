#include "stdafx.h"

#include"CommandProcess.h"
#include "../ZoneServer/Client.h"
#include "../../common/Separator.h"
#include "../Database/ZoneDatabase.h"
#include "../Spawns/Spawn.h"
#include "../Spawns/Entity.h"
#include "../Spawns/PathPointSpawn.h"
#include "../Controllers/PlayerController.h"
#include "../ZoneServer/ZoneServer.h"
#include "../Players/NPCPathDebug.h"
#include "../ZoneServer/SpawnCamp.h"
#include "../Spawns/SpawnCampSpawn.h"
#include "../Players/SpawnCampDebug.h"

#define SpawnSet(target, master, action) if(master) { master->action; } target->action

void CommandProcess::CommandSpawnSet(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() < 2) {
		//syntax error
		return;
	}

	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller)
		return;

	std::shared_ptr<Spawn> target = controller->GetTarget();
	if (!target)
		return;

	std::shared_ptr<Spawn> masterSpawn = client->GetZone()->GetSpawnFromMasterList(target->GetDatabaseID());

	// Title struct
	std::string cmd = sep.GetString(0);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	if (cmd == "name") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetName(value));
		client->chat.DisplayText("MOTD", "Setting spawns name to " + value, 0xff, false, "");
	}

	if (cmd == "lastname") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetLastName(value));
		client->chat.DisplayText("MOTD", "Setting spawns lastname to " + value, 0xff, false, "");
	}

	if (cmd == "suffixtitle") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetSuffixTitle(value));
		client->chat.DisplayText("MOTD", "Setting spawns suffixtitle to " + value, 0xff, false, "");
	}

	if (cmd == "prefixtitle") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetPrefixTitle(value));
		client->chat.DisplayText("MOTD", "Setting spawns prefixtitle to " + value, 0xff, false, "");
	}

	if (cmd == "pvptitle") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetPvPTitle(value));
		client->chat.DisplayText("MOTD", "Setting spawns pvptitle to " + value, 0xff, false, "");
	}

	if (cmd == "guild") {
		std::string value = sep.GetString(1);
		SpawnSet(target, masterSpawn, SetGuild(value));
		client->chat.DisplayText("MOTD", "Setting spawns guild to " + value, 0xff, false, "");
	}

	// Info struct
	if (cmd == "visual_state" && sep.IsNumber(1)) {
		uint32_t value = sep.GetUInt32(1);
		SpawnSet(target, masterSpawn, SetVisualState(value));
		client->chat.DisplayText("MOTD", "Setting spawns visual_state to " + to_string(value), 0xff, false, "");
	}

	if (cmd == "difficulty_offset" && sep.IsNumber(1)) {
		uint32_t value = sep.GetUInt32(1);
		SpawnSet(target, masterSpawn, SetDifficultyOffset(value));
		client->chat.DisplayText("MOTD", "Setting spawns difficulty_offset to " + to_string(value), 0xff, false, "");
	}

	if (cmd == "model_type" && sep.IsNumber(1)) {
		uint32_t value = sep.GetUInt32(1);
		SpawnSet(target, masterSpawn, SetModelType(value));
		client->chat.DisplayText("MOTD", "Setting spawns model_type to " + to_string(value), 0xff, false, "");
	}

	if (cmd == "model_color" && sep.IsNumber(1) && sep.IsNumber(2) && sep.IsNumber(3)) {
		EQ2Color color;
		color.Red = static_cast<uint8_t>(sep.GetUInt32(1));
		color.Green = static_cast<uint8_t>(sep.GetUInt32(2));
		color.Blue = static_cast<uint8_t>(sep.GetUInt32(3));
		SpawnSet(target, masterSpawn, SetModelColor(color));
		client->chat.DisplayText("MOTD", "Setting spawns model_color to " + to_string(color.Red) + ", " + to_string(color.Green) + ", " + to_string(color.Blue), 0xff, false, "");
	}

	if (cmd == "skin_color" && sep.IsNumber(1) && sep.IsNumber(2) && sep.IsNumber(3)) {
		EQ2Color color;
		color.Red = static_cast<uint8_t>(sep.GetUInt32(1));
		color.Green = static_cast<uint8_t>(sep.GetUInt32(2));
		color.Blue = static_cast<uint8_t>(sep.GetUInt32(3));
		SpawnSet(target, masterSpawn, SetSkinColor(color));
		client->chat.DisplayText("MOTD", "Setting spawns skin_color to " + to_string(color.Red) + ", " + to_string(color.Green) + ", " + to_string(color.Blue), 0xff, false, "");
	}

	// Vis struct

	// Pos struct
	if (cmd == "size" && sep.IsNumber(1)) {
		float value = sep.GetFloat(1);
		SpawnSet(target, masterSpawn, SetSize(value));
		client->chat.DisplayText("MOTD", "Setting spawns size to " + to_string(value), 0xff, false, "");
	}
}

void CommandProcess::CommandSpawnDetails(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<Entity> player = client->GetController()->GetControlled();
	if (!player) {
		return;
	}

	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller) {
		return;
	}

	std::shared_ptr<Spawn> spawn = controller->GetTarget();
	if (!spawn) {
		return;
	}

	auto zone = client->GetZone();
	if (!zone) {
		return;
	}

	std::string text = "";

	const char* type = "NPC";
	if (spawn->GetSignData()) {
		type = "Sign";
	}
	else if (spawn->GetWidgetData()) {
		type = "Widget";
	}

	//if (spawn->IsObject())
	//	type = "Object";
	//else if (spawn->IsGroundSpawn())
	//	type = "GroundSpawn";

	text = "Name: " + spawn->GetName() + ", Type: " + type + ", ID: " + to_string(spawn->GetDatabaseID());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Last Name: " + spawn->GetLastName() + ", Guild: " + spawn->GetGuildTitle() + ", Prefix: " + spawn->GetPrefixTitle() + ", Suffix: " + spawn->GetSuffixTitle();
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Spawn Location ID: " + to_string(spawn->GetSpawnLocationID()) + ", Spawn Group ID: " + to_string(spawn->GetSpawnGroupID());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Faction ID: " + to_string(spawn->GetFactionID()) + ", Merchant ID: " + to_string(spawn->GetMerchantID()) + ", Transporter ID: Not Used Yet"; //+ to_string(spawn->GetTransporterID());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Grid ID: " + to_string(spawn->GetGridID());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Race: " + to_string(spawn->GetRace()) + ", Class: " + to_string(spawn->GetAdventureClass()) + ", Gender: " + to_string(spawn->GetGender());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Level: " + to_string(spawn->GetAdventureLevel()) + ", HP: " + to_string(spawn->GetHP()) + ", Power: " + to_string(spawn->GetPower());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Respawn Time: " + to_string(spawn->GetRespawnTime()) + "(sec), X: " + to_string(spawn->GetX()) + ", Y: " + to_string(spawn->GetY()) + ", Z: " + to_string(spawn->GetZ()) + ", Heading: " + to_string(spawn->GetHeading());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Collision Radius: " + to_string(spawn->GetCollisionRadius()) + ", Size: " + to_string(spawn->GetSize()) + ", Difficulty: " + to_string(spawn->GetDifficulty()) + ", Heroic: " + to_string(spawn->GetHeroic());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Targetable: " + to_string(spawn->IsTargetable()) + ", Show Name: " + to_string(spawn->ShouldShowName()) + ", Attackable: " + to_string(spawn->IsAttackable()) + ", Show Level: " + to_string(spawn->ShouldShowLevel());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Show Command Icon: " + to_string(spawn->ShouldShowCommandIcon()) + ", Display Hand Icon: " + to_string(spawn->IsInteractable());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	//if (spawn->IsEntity()) {
	//	client->Message(CHANNEL_COLOR_YELLOW, "Facial Hair Type: %i, Hair Type: %i, Chest Type: %i, Legs Type: %i", ((Entity*)spawn)->GetFacialHairType(), ((Entity*)spawn)->GetHairType(), ((Entity*)spawn)->GetChestType(), ((Entity*)spawn)->GetLegsType());
	//	client->Message(CHANNEL_COLOR_YELLOW, "Soga Facial Hair Type: %i, Soga Hair Type: %i, Wing Type: %i", ((Entity*)spawn)->GetSogaFacialHairType(), ((Entity*)spawn)->GetSogaHairType(), ((Entity*)spawn)->GetWingType());
	//}

	text = "Model Type: " + to_string(spawn->GetModelType()) + ", Soga Race Type: " + to_string(spawn->GetSogaModelType());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Primary Command Type: " + to_string(spawn->GetPrimaryCommandListID()) + ", Secondary Command Type: " + to_string(spawn->GetSecondaryCommandListID());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Visual State: " + to_string(spawn->GetVisualState()) + ", Action State: " + to_string(spawn->GetActionState()) + ", Mood State: " + to_string(spawn->GetMoodState()); // +", Initial State: " + to_string(spawn->GetInitialState()) + ", Activity Status: " + to_string(spawn->GetActivityState());
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Emote State: " + to_string(spawn->GetEmoteState()) + ", Pitch: " + to_string(spawn->GetPitch()) + ", Roll: " + to_string(spawn->GetRoll()); // +", Hide Hood: " + to_string(spawn->appearance.hide_hood);
	client->chat.DisplayText("MOTD", text, 0xff, false, "");

	text = "Distance From Target: " + to_string(player->GetDistance(spawn));
	client->chat.DisplayText("MOTD", text, 0xff, false, "");
	
	//if (spawn->IsNPC()) {
	//	text = "Randomize: %u", ((NPC*)spawn)->GetRandomize();
	//	client->chat.DisplayText("MOTD", text, 0xff, false, "");
	//}

	text = "=========================================================";
	client->chat.DisplayText("MOTD", text, 0xff, false, "");
}

void CommandProcess::CommandSpawn(const std::shared_ptr<Client>& client, Separator& sep) {
	if (sep.GetSize() == 0 || !sep.IsNumber(0))
		return; // syntax error

	std::shared_ptr<ZoneServer> zone = client->GetZone();
	std::shared_ptr<Entity> player = client->GetController()->GetControlled();
	std::shared_ptr<Spawn> spawn = zone->GetNewSpawnFromMasterList(sep.GetUInt32(0));
	if (spawn) {
		spawn->SetLocation(player->GetX(), player->GetY(), player->GetZ(), false);
		spawn->SetGridID(player->GetGridID(), false);
		spawn->SetHeading(player->GetHeading(), false);
		client->chat.DisplayText("MOTD", "Adding spawn", 0xff, false, "");

		SpawnEntryType type;
		if (spawn->IsSign())
			type = SpawnEntryType::ESIGN;
		else if (spawn->IsWidget())
			type = SpawnEntryType::EWIDGET;
		else if (spawn->IsGroundSpawn())
			type = SpawnEntryType::EGROUNDSPAWN;
		else if (spawn->IsObject())
			type = SpawnEntryType::EOBJECT;
		else
			type = SpawnEntryType::ENPC;

		zone->AddSpawn(spawn, type);
		zone->SendSpawnToClient(spawn, client);
	}

}

void CommandProcess::CommandPath(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller) {
		LogDebug(LOG_NPC, 0, "CommandPath: unable to get player controller");
		return;
	}

	std::shared_ptr<Spawn> target = controller->GetTarget();
	if (!target) {
		LogDebug(LOG_NPC, 0, "CommandPath: unable to get player target");
		return;
	}

	std::shared_ptr<NPCPathDebug> path = controller->GetNPCPathDebug();
	if (!path) {
		path = std::make_shared<NPCPathDebug>(client);
		controller->SetNPCPathDebug(path);
	}
	std::string cmd = sep.GetString(0);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	if (cmd == "start")
		path->Start(target);

	if (cmd == "end")
		path->End();

	if (cmd == "add")
		path->AddPathPoint();

	if (cmd == "remove") {
		if (target->IsPathPoint())
			path->RemovePathPoint(dynamic_pointer_cast<PathPointSpawn>(target));
		else
			client->chat.DisplayText("Error Text", "Target must be a path point in order to remove it", 0xff, false, "");
	}

	if (cmd == "generate")
		path->GenerateLua();

	if (cmd == "set") {
		if (target->IsPathPoint()) {
			std::string set_cmd = sep.GetString(1);
			std::transform(set_cmd.begin(), set_cmd.end(), set_cmd.begin(), ::tolower);
			if (set_cmd == "speed")
				path->SetPathPointSpeed(dynamic_pointer_cast<PathPointSpawn>(target), sep.GetFloat(2));
			else if (set_cmd == "delay")
				path->SetPathPointDelay(dynamic_pointer_cast<PathPointSpawn>(target), sep.GetFloat(2));
			else if (set_cmd == "callback")
				path->SetPathPointCallback(dynamic_pointer_cast<PathPointSpawn>(target), sep.GetString(2));
			else 
				client->chat.DisplayText("MOTD", "Syntax: /path set <field>\nFields: speed, delay, callback", 0xff, false, "");
		}
		else {
			client->chat.DisplayText("Error Text", "Target must be a path point in order to change it", 0xff, false, "");
		}
	}
}

void CommandProcess::CommandSpawnCamp(const std::shared_ptr<Client>& client, Separator& sep) {
	std::shared_ptr<PlayerController> controller = client->GetController();
	if (!controller) {
		LogDebug(LOG_NPC, 0, "CommandSpawnCamp: unable to get player controller");
		return;
	}

	std::shared_ptr<SpawnCampDebug> scd = controller->GetSpawnCampDebug();
	if (!scd) {
		scd = std::make_shared<SpawnCampDebug>(client);
		controller->SetSpawnCampDebug(scd);
	}

	std::shared_ptr<Entity> player = controller->GetControlled();
	std::shared_ptr<ZoneServer> zone = client->GetZone();

	// Transform the sub command to lowercase
	std::string cmd = sep.GetString(0);
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	if (cmd == "show") {
		std::string cmd2 = sep.GetString(1);
		std::transform(cmd2.begin(), cmd2.end(), cmd2.begin(), ::tolower);
		if (cmd2 == "radius") {
			std::shared_ptr<Spawn> target = controller->GetTarget();
			if (target && target->IsSpawnCampSpawn()) {
				std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
				if (camp)
					scd->ShowSpawnCampRadius(camp);
			}
		}
		else {
			scd->Show();
			client->chat.DisplayText("MOTD", "Showing all spawn camps in the zone", 0xff, false, "");
		}
		return;
	}

	if (cmd == "new") {
		std::shared_ptr<SpawnCamp> sc = std::make_shared<SpawnCamp>(zone, player->GetX(), player->GetY(), player->GetZ(), player->GetGridID());
		zone->AddSpawnCamp(sc);
		client->chat.DisplayText("MOTD", "Created a new spawn camp", 0xff, false, "");
		return;
	}

	if (cmd == "add" && sep.IsNumber(1)) {
		uint32_t id = sep.GetUInt32(1);
		std::shared_ptr<Spawn> target = controller->GetTarget();
		if (target && target->IsSpawnCampSpawn()) {
			std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
			if (camp) {
				camp->AddSpawnID(id);
				client->chat.DisplayText("MOTD", "Added spawn id " + to_string(id) + " to the spawn camps list", 0xff, false, "");
				return;
			}		
		}
	}

	if (cmd == "reset") {
		std::shared_ptr<Spawn> target = controller->GetTarget();
		if (target && target->IsSpawnCampSpawn()) {
			std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
			if (camp) {
				client->chat.DisplayText("MOTD", "Resetting spawn camp", 0xff, false, "");
				camp->Reset();
			}
		}
	}

	if (cmd == "set") {
		std::string cmd2 = sep.GetString(1);
		std::transform(cmd2.begin(), cmd2.end(), cmd2.begin(), ::tolower);
		
		if (cmd2 == "radius" && sep.IsNumber(2)) {
			float radius = sep.GetFloat(2);
			std::shared_ptr<Spawn> target = controller->GetTarget();
			if (target && target->IsSpawnCampSpawn()) {
				std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
				if (camp) {
					camp->SetRadius(radius);
					client->chat.DisplayText("MOTD", "Setting the spawn camps radius to " + to_string(radius), 0xff, false, "");
					scd->UpdateSpawnCampRadius(camp);
				}
			}
		}

		if (cmd2 == "density" && sep.IsNumber(2)) {
			uint16_t numEncounters = sep.GetUInt32(2);
			std::shared_ptr<Spawn> target = controller->GetTarget();
			if (target && target->IsSpawnCampSpawn()) {
				std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
				if (camp) {
					camp->SetNumRadiusEncounter(numEncounters);
					client->chat.DisplayText("MOTD", "Setting spawn camps density to " + to_string(numEncounters), 0xff, false, "");
				}
			}
		}
	}

	if (cmd == "details") {
		std::shared_ptr<Spawn> target = controller->GetTarget();
		if (target && target->IsSpawnCampSpawn()) {
			std::shared_ptr<SpawnCamp> camp = dynamic_pointer_cast<SpawnCampSpawn>(target)->GetSpawnCamp();
			if (camp) {
				string text = "Radius = " + to_string(camp->GetRadius()) + "\n";
				text += "Density = " + to_string(camp->GetNumRadiusEncounters()) + "\n";

				client->chat.DisplayText("MOTD", text, 0xff, false, "");
			}
			else {
				client->chat.DisplayText("Error Text", "Target must be a spawn camp", 0xff, false, "");
			}

		}
	}
}