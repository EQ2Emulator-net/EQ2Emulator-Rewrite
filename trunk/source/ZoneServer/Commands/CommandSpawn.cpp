#include "stdafx.h"

#include"CommandProcess.h"
#include "../ZoneServer/Client.h"
#include "../../common/Separator.h"
#include "../Database/ZoneDatabase.h"
#include "../Spawns/Spawn.h"
#include "../Spawns/Entity.h"
#include "../Controllers/PlayerController.h"
#include "../ZoneServer/ZoneServer.h"

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
		SpawnSet(target, masterSpawn, SetName(sep.GetString(1)));
	}

	if (cmd == "lastname") {
		SpawnSet(target, masterSpawn, SetLastName(sep.GetString(1)));
	}

	if (cmd == "suffixtitle") {
		SpawnSet(target, masterSpawn, SetSuffixTitle(sep.GetString(1)));
	}

	if (cmd == "prefixtitle") {
		SpawnSet(target, masterSpawn, SetPrefixTitle(sep.GetString(1)));
	}

	if (cmd == "pvptitle") {
		SpawnSet(target, masterSpawn, SetPvPTitle(sep.GetString(1)));
	}

	if (cmd == "guild") {
		SpawnSet(target, masterSpawn, SetGuild(sep.GetString(1)));
	}

	// Info struct
	if (cmd == "visual_state" && sep.IsNumber(1)) {
		SpawnSet(target, masterSpawn, SetVisualState(sep.GetUInt32(1)));
	}

	if (cmd == "difficulty_offset" && sep.IsNumber(1)) {
		SpawnSet(target, masterSpawn, SetDifficultyOffset(sep.GetUInt32(1)));
	}

	if (cmd == "model_type" && sep.IsNumber(1)) {
		SpawnSet(target, masterSpawn, SetModelType(sep.GetUInt32(1)));
	}

	// Vis struct

	// Pos struct
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