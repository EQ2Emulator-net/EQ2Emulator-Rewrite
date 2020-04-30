#include "stdafx.h"

#include "ZoneDatabase.h"
#include "../../common/log.h"
#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/ZoneOperator.h"
#include "../Spawns/Entity.h"
#include "../Spawns/Object.h"
#include "../Spawns/GroundSpawn.h"

extern ZoneOperator g_zoneOperator;

constexpr const char* GetSpawnTableFields();

ZoneDatabase::ZoneDatabase() {

}

ZoneDatabase::~ZoneDatabase() {
}

bool ZoneDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

bool ZoneDatabase::LoadZoneInfo(ZoneServer* z) {
	DatabaseResult result;
	
	bool success = Select(&result, "SELECT * FROM zones WHERE id = %u", z->GetID());
	if (!success)
		return success;

	if (result.Next()) {
		z->SetExpansionID(result.GetUInt8(1));
		z->SetName(result.GetString(2));
		z->SetFile(result.GetString(3));
		z->SetDescription(result.GetString(4));
		z->SetSafeX(result.GetFloat(5));
		z->SetSafeY(result.GetFloat(6));
		z->SetSafeZ(result.GetFloat(7));
		z->SetSafeHeading(result.GetFloat(8));
		z->SetUnderworld(result.GetFloat(9));
		z->SetEXPModifier(result.GetFloat(10));
		z->SetMinRecommended(result.GetUInt8(11));
		z->SetMaxRecomended(result.GetUInt8(12));
		z->SetZoneType(result.GetString(13));
		z->SetIsAlwaysLoaded(result.GetBool(14));
		z->SetCityZone(result.GetBool(15));
		z->SetWeatherAllowed(result.GetBool(16));
		z->SetMinStatus(result.GetUInt32(17));
		z->SetMinLevel(result.GetUInt32(18));
		z->SetMaxLevel(result.GetUInt32(19));
		z->SetStartingZone(result.GetUInt8(20));
		z->SetInstanceType(result.GetString(21));
		z->SetDefaultReenterTime(result.GetUInt32(22));
		z->SetDefaultResetTime(result.GetUInt32(23));
		z->SetDefaultLockoutTime(result.GetUInt32(24));
		z->SetForceGroupToZone(result.GetUInt16(25));
		z->SetLuaScript(result.GetString(26));
		z->SetShutdownTimer(result.GetUInt32(27));
		z->SetZoneMOTD(result.GetString(28));
		z->SetRulesetID(result.GetUInt32(29));
	}
	else
		return false;

	return true;
}

bool ZoneDatabase::LoadCharacter(uint32_t char_id, uint32_t account_id, std::shared_ptr<Entity> entity) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT `name`, `race`, `class`, `gender`, `deity`, `body_size`, `body_age`, `level`, `tradeskill_class`, `tradeskill_level`, `soga_wing_type`, `soga_chest_type`, `soga_legs_type`, `soga_hair_type`, `soga_facial_hair_type`, `soga_model_type`, `legs_type`, `chest_type`, `wing_type`, `hair_type`, `facial_hair_type`, `model_type`, `x`, `y`, `z`, `heading`, `starting_city`, DATEDIFF(curdate(), `created_date`) as accage FROM characters WHERE id = %u AND account_id = %u AND deleted = 0", char_id, account_id);
	if (!ret)
		return ret;

	if (result.Next()) {
		entity->SetName(result.GetString(0), false);
		entity->SetRace(result.GetUInt8(1), false);
		entity->SetAdventureClass(result.GetUInt8(2), false);
		entity->SetGender(result.GetUInt8(3), false);
		// Set Deity
		entity->SetBodySize(result.GetUInt8(5), false);
		entity->SetBodySizeUnknown(result.GetUInt8(6), false); // Not sure if this is actually the correct field for age
		entity->SetLevel(result.GetUInt8(7), false);
		// Set Tradeskill Class
		// Set Tradeskill Level
		// Set SOGA Wing Type
		// Set SOGA Chest Type
		// Set SOGA Legs Type
		entity->SetSogaHairType(result.GetUInt16(13), false);
		entity->SetSogaFacialHairType(result.GetUInt16(14), false);
		entity->SetSogaModelType(result.GetUInt32(15), false);
		entity->SetLegsType(result.GetUInt16(16), false);
		entity->SetChestType(result.GetUInt16(17), false);
		entity->SetWingType(result.GetUInt16(18), false);
		entity->SetHairType(result.GetUInt16(19), false);
		entity->SetFacialHairType(result.GetUInt16(20), false);
		entity->SetModelType(result.GetUInt32(21), false);
		entity->SetX(result.GetFloat(22), false);
		entity->SetY(result.GetFloat(23), false);
		entity->SetZ(result.GetFloat(24), false);
		entity->SetHeading(result.GetFloat(25), false);
		// Set Starting City
		// Set Account Age

		DatabaseResult result2;
		ret = Select(&result2, "SELECT char_id, type, red, green, blue FROM char_colors WHERE char_id = %u", char_id);
		if (!ret)
			return ret;

		//I know it looks stupid using a map for 1 entity but this lets us reuse the same code for characters and npcs
		std::unordered_map<uint32_t, std::shared_ptr<Entity> > colorLoadMap;
		colorLoadMap[char_id] = entity;

		ProcessEntityColors(result2, colorLoadMap);
	}

	return ret;
}

bool ZoneDatabase::LoadNPCsForZone(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT %s, npc.min_level, npc.max_level, npc.enc_level, npc.class_, npc.gender, npc.min_group_size, npc.max_group_size, npc.hair_type_id, npc.facial_hair_type_id, npc.wing_type_id, npc.chest_type_id, npc.legs_type_id, npc.soga_hair_type_id, npc.soga_facial_hair_type_id, npc.action_state, npc.mood_state, npc.initial_state, npc.activity_status, npc.attack_type, npc.ai_strategy+0, npc.spell_list_id, npc.secondary_spell_list_id, npc.skill_list_id, npc.secondary_skill_list_id, npc.equipment_list_id, npc.str, npc.sta, npc.wis, npc.intel, npc.agi, npc.heat, npc.cold, npc.magic, npc.mental, npc.divine, npc.disease, npc.poison, npc.aggro_radius, npc.cast_percentage, npc.randomize, npc.soga_model_type, npc.heroic_flag, npc.alignment, npc.elemental, npc.arcane, npc.noxious, npc.hide_hood, npc.emote_state \n"
									"FROM spawn s\n"
									"INNER JOIN spawn_npcs npc\n"
									"ON s.id = npc.spawn_id\n"
									"INNER JOIN spawn_location_entry le\n"
									"ON npc.spawn_id = le.spawn_id\n"
									"INNER JOIN spawn_location_placement lp\n"
									"ON le.spawn_location_id = lp.spawn_location_id\n"
									"WHERE lp.zone_id = %u\n"
									"GROUP BY s.id", GetSpawnTableFields(), z->GetID());
	if (!ret)
		return ret;

	std::unordered_map<uint32_t, std::shared_ptr<Entity> > npcs;
	npcs.reserve(result.GetNumRows());

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t id = result.GetUInt32(0);
		if (z->GetNPCFromMasterList(id))
			continue;

		std::shared_ptr<Entity> npc = std::make_shared<Entity>();

		//Base spawn data
		uint32_t i = ProcessSpawnTableFields(npc, result);

		npc->EnableEntityFlags(EntityIsNpc);

		npc->SetLevel(result.GetUInt8(i++));
		// TODO: min/max level
		//npc->SetMinLevel(result.GetUInt8(i++));
		i++;
		//npc->SetMaxLevel(result.GetUInt8(i++));
		npc->SetDifficulty(result.GetUInt8(i++));
		npc->SetAdventureClass(result.GetUInt8(i++));
		npc->SetGender(result.GetUInt8(i++));
		// min_group_size, no clue what this is for
		// max_group_size, no clue what this is for
		i += 2;
		npc->SetHairType(result.GetUInt32(i++));
		npc->SetFacialHairType(result.GetUInt32(i++));
		npc->SetWingType(result.GetUInt32(i++));
		npc->SetChestType(result.GetUInt32(i++));
		npc->SetLegsType(result.GetUInt32(i++));
		npc->SetSogaHairType(result.GetUInt32(i++));
		npc->SetSogaFacialHairType(result.GetUInt32(i++));
		npc->SetActionState(result.GetUInt32(i++));
		npc->SetMoodState(result.GetUInt32(i++));
		npc->SetState(result.GetUInt32(i++));
		//Activity status..think we already put these flags into EntityFlags via query
		i++;
		// Attack Type
		i++;
		// AI Strategy
		i++;
		// Primary Spell List
		i++;
		// Secondary Spell List
		i++;
		// Primary Skill List
		i++;
		// Secondary Skill List
		i++;
		// Equipment List ID
		i++;
		// Base Str
		i++;
		// Base Sta
		i++;
		// Base Wis
		i++;
		// Base Int
		i++;
		// Base Agi
		i++;
		// Base Heat
		i++;
		// Base Cold
		i++;
		// Base Magic
		i++;
		// Base Mental
		i++;
		// Base Divine
		i++;
		// Base Disease
		i++;
		// Base Poison
		i++;
		// Aggro Radius
		i++;
		// Cast Percentage 
		i++;
		// randomize
		i++;
		npc->SetSogaModelType(result.GetUInt32(i++));
		npc->SetHeroicFlag(result.GetUInt8(i++));
		// Alignment
		i++;
		// Base Elemental
		i++;
		// Base Arcane
		i++;
		// Base Noxious
		i++;
		//Hide hood
		if (result.GetBool(i++)) {
			npc->EnableInfoVisFlags(INFO_VIS_FLAG_HIDE_HOOD);
		}
		else {
			npc->DisableInfoVisFlags(INFO_VIS_FLAG_HIDE_HOOD);
		}
		npc->SetEmoteState(result.GetUInt32(i++));

		npcs[npc->GetDatabaseID()] = npc;

		count++;
		LogDebug(LOG_NPC, 5, "---Loading NPC: '%s' (%u)", npc->GetName().c_str(), id);
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u NPC(s).", count);

	LogInfo(LOG_NPC, 0, "--Loading NPC Appearances.");
	result.Clear();
	ret = Select(&result,
		"SELECT na.spawn_id, na.`type`, na.red, na.green, na.blue\n"
		"FROM spawn_location_placement slp\n"
		"INNER JOIN spawn_location_entry sle ON sle.spawn_location_id = slp.spawn_location_id\n"
		"INNER JOIN npc_appearance na ON sle.spawn_id = na.spawn_id\n"
		"WHERE slp.zone_id = %u\n"
		"ORDER BY spawn_id", z->GetID());

	if (ret) {
		ProcessEntityColors(result, npcs);

		result.Clear();

		ret = Select(&result, "SELECT na.spawn_id, na.slot_id, na.equip_type, na.red, na.green, na.blue, na.highlight_red, na.highlight_green, na.highlight_blue\n"
			"FROM spawn_location_placement slp\n"
			"INNER JOIN spawn_location_entry sle ON sle.spawn_location_id = slp.spawn_location_id\n"
			"INNER JOIN npc_appearance_equip na ON sle.spawn_id = na.spawn_id\n"
			"WHERE slp.zone_id = %u\n"
			"ORDER BY na.spawn_id;", z->GetID());

		ProcessNpcAppearanceEquipment(result, npcs);
	}

	for (auto& itr : npcs) {
		itr.second->PopUpdateFlags();
		z->AddNPCToMasterList(itr.second);
	}

	return ret;
}

bool ZoneDatabase::LoadObjectsForZone(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT %s,\n"
									"so.id, so.device_id\n"
									"FROM spawn s\n"
									"INNER JOIN spawn_objects so\n"
									"ON s.id = so.spawn_id\n"
									"INNER JOIN spawn_location_entry le\n"
									"ON so.spawn_id = le.spawn_id\n"
									"INNER JOIN spawn_location_placement lp\n"
									"ON le.spawn_location_id = lp.spawn_location_id\n"
									"WHERE lp.zone_id = %u\n"
									"GROUP BY s.id", GetSpawnTableFields(), z->GetID());

	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t index = 0;
		uint32_t id = result.GetUInt32(index++);
		if (z->GetNPCFromMasterList(id))
			continue;

		std::shared_ptr<Object> spawn = std::make_shared<Object>();

		// Spawn base info
		index = ProcessSpawnTableFields(spawn, result);

		// Object Info starts here
		spawn->SetObjectDatabaseID(result.GetUInt32(index++));
		spawn->SetDeviceID(result.GetUInt8(index++));
			
		count++;

		z->AddObjectToMasterList(spawn);
		LogDebug(LOG_NPC, 5, "---Loading Object: '%s' (%u)", spawn->GetName().c_str(), id);
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u Object(s).", count);
	return ret;
}

bool ZoneDatabase::LoadWidgetsForZone(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT %s,\n"
		"sw.id, sw.widget_id, sw.widget_x, sw.widget_y, sw.widget_z, sw.include_heading, sw.include_location, sw.type+0, sw.open_heading, sw.closed_heading, sw.open_x, sw.open_y, sw.open_z, sw.action_spawn_id, sw.open_sound_file, sw.close_sound_file, sw.open_duration, sw.close_x, sw.close_y, sw.close_z, sw.linked_spawn_id, sw.house_id\n"
		"FROM spawn s\n"
		"INNER JOIN spawn_widgets sw\n"
		"ON s.id = sw.spawn_id\n"
		"INNER JOIN spawn_location_entry le\n"
		"ON sw.spawn_id = le.spawn_id\n"
		"INNER JOIN spawn_location_placement lp\n"
		"ON le.spawn_location_id = lp.spawn_location_id\n"
		"WHERE lp.zone_id = %u\n"
		"GROUP BY s.id", GetSpawnTableFields(), z->GetID());

	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t index = 0;
		uint32_t id = result.GetUInt32(index++);
		if (z->GetWidgetFromMasterList(id))
			continue;

		std::shared_ptr<Spawn> spawn = std::make_shared<Spawn>();

		// Spawn base info
		index = ProcessSpawnTableFields(spawn, result);

		// Widget Info starts here
		std::unique_ptr<Widget>& wd = spawn->GetWidgetData();
		if (!wd)
			wd = std::make_unique<Widget>();

		wd->SetWidgetDatabaseID(result.GetUInt32(index++));
		wd->SetWidgetID(result.GetUInt32(index++));
		wd->SetWidgetX(result.GetFloat(index++));
		wd->SetWidgetY(result.GetFloat(index++));
		wd->SetWidgetZ(result.GetFloat(index++));
		wd->SetIncludeHeading(result.GetBool(index++));
		wd->SetIncludeLocation(result.GetBool(index++));
		wd->SetWidgetType((EWidgetType)result.GetUInt8(index++));
		wd->SetOpenHeading(result.GetFloat(index++));
		wd->SetCloseHeading(result.GetFloat(index++));
		wd->SetOpenX(result.GetFloat(index++));
		wd->SetOpenY(result.GetFloat(index++));
		wd->SetOpenZ(result.GetFloat(index++));
		wd->SetActionID(result.GetUInt32(index++));
		wd->SetOpenSoundFile(result.GetString(index++));
		wd->SetCloseSoundFile(result.GetString(index++));
		wd->SetOpenDuration(result.GetUInt32(index++));
		wd->SetCloseX(result.GetFloat(index++));
		wd->SetCloseY(result.GetFloat(index++));
		wd->SetCloseZ(result.GetFloat(index++));
		wd->SetLinkedSpawnID(result.GetUInt32(index++));
		wd->SetHouseID(result.GetUInt32(index++));

		count++;

		z->AddWidgetToMasterList(spawn);
		LogDebug(LOG_NPC, 5, "---Loading Widget: '%s' (%u)", spawn->GetName().c_str(), id);
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u Widget(s).", count);
	return ret;
}

bool ZoneDatabase::LoadSignsForZone(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT %s,\n"
		"ss.id, ss.type+0, ss.zone_id, ss.title, ss.description, ss.sign_distance, ss.zone_x, ss.zone_y, ss.zone_z, ss.zone_heading,\n"
		"ss.widget_id, ss.widget_x, ss.widget_y, ss.widget_z, ss.include_heading, ss.include_location\n"
		"FROM spawn s\n"
		"INNER JOIN spawn_signs ss\n"
		"ON s.id = ss.spawn_id\n"
		"INNER JOIN spawn_location_entry le\n"
		"ON ss.spawn_id = le.spawn_id\n"
		"INNER JOIN spawn_location_placement lp\n"
		"ON le.spawn_location_id = lp.spawn_location_id\n"
		"WHERE lp.zone_id = %u\n"
		"GROUP BY s.id", GetSpawnTableFields(), z->GetID());

	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t index = 0;
		uint32_t id = result.GetUInt32(index++);
		if (z->GetSignFromMasterList(id))
			continue;

		std::shared_ptr<Spawn> spawn = std::make_shared<Spawn>();

		// Spawn base info
		index = ProcessSpawnTableFields(spawn, result);

		// Sign info starts here
		std::unique_ptr<Sign>& sign = spawn->GetSignData();
		if (!sign)
			sign = std::make_unique<Sign>();

		sign->SetSignDatabseID(result.GetUInt32(index++));
		sign->SetSignType((ESignType)result.GetUInt8(index++));
		sign->SetZoneID(result.GetUInt32(index++));
		sign->SetTitle(result.GetString(index++));
		sign->SetDescription(result.GetString(index++));
		sign->SetDistance(result.GetFloat(index++));
		sign->SetZoneX(result.GetFloat(index++));
		sign->SetZoneY(result.GetFloat(index++));
		sign->SetZoneZ(result.GetFloat(index++));
		sign->SetZoneHeading(result.GetFloat(index++));

		// Widget info starts here
		uint32_t widget_id = result.GetUInt32(index++);
		if (widget_id != 0) {
			std::unique_ptr<Widget>& widget = spawn->GetWidgetData();
			if (!widget)
				widget = std::make_unique<Widget>();

			widget->SetWidgetID(widget_id);
			widget->SetWidgetX(result.GetFloat(index++));
			widget->SetWidgetY(result.GetFloat(index++));
			widget->SetWidgetZ(result.GetFloat(index++));
			widget->SetIncludeHeading(result.GetBool(index++));
			widget->SetIncludeLocation(result.GetBool(index++));
		}

		count++;

		z->AddSignToMasterList(spawn);
		LogDebug(LOG_NPC, 5, "---Loading Sign: '%s' (%u)", spawn->GetName().c_str(), id);
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u Sign(s).", count);
	return ret;
}

bool ZoneDatabase::LoadGroundSpawnsForZone(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT %s,\n"
		"sg.id, sg.number_harvests, sg.num_attempts_per_harvest, sg.groundspawn_id, sg.collection_skill\n"
		"FROM spawn s\n"
		"INNER JOIN spawn_ground sg\n"
		"ON s.id = sg.spawn_id\n"
		"INNER JOIN spawn_location_entry le\n"
		"ON sg.spawn_id = le.spawn_id\n"
		"INNER JOIN spawn_location_placement lp\n"
		"ON le.spawn_location_id = lp.spawn_location_id\n"
		"WHERE lp.zone_id = %u\n"
		"GROUP BY s.id", GetSpawnTableFields(), z->GetID());

	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		uint32_t index = 0;
		uint32_t id = result.GetUInt32(index++);
		if (z->GetGroundSpawnFromMasterList(id))
			continue;

		std::shared_ptr<GroundSpawn> spawn = std::make_shared<GroundSpawn>();

		// Spawn base info
		index = ProcessSpawnTableFields(spawn, result);

		// GroundSpawn info starts here
		spawn->SetGroundSpawnDatabaseID(result.GetUInt32(index++));
		spawn->SetNumberOfHarvests(result.GetUInt8(index++));
		spawn->SetNumberAttemptsPerHarvets(result.GetUInt8(index++));
		spawn->SetGroundSpawnID(result.GetUInt32(index++));
		spawn->SetCollectionSkill(result.GetString(index++));

		count++;

		z->AddGroundSpawnToMasterList(spawn);
		LogDebug(LOG_NPC, 5, "---Loading GroundSpawn: '%s' (%u)", spawn->GetName().c_str(), id);
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u GroundSpawn(s).", count);
	return ret;	
}

bool ZoneDatabase::CharacterUpdateBiography(uint32_t char_id, const char* bio) {
	return Query("UPDATE `character_details` SET `biography` = '%s' WHERE `char_id` = '%u'", bio, char_id);
}

bool ZoneDatabase::LoadNPCLocations(ZoneServer* z) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT sln.id, sln.name,\n"
		"slp.id, slp.x, slp.y, slp.z, slp.x_offset, slp.y_offset, slp.z_offset, slp.heading, slp.pitch, slp.roll, slp.respawn, slp.expire_timer, slp.expire_offset, slp.grid_id,\n"
		"sle.id, sle.spawn_id, sle.spawnpercentage, sle.condition\n"
		"FROM spawn_location_name sln, spawn_location_placement slp, spawn_location_entry sle, spawn_npcs sn\n"
		"WHERE sn.spawn_id = sle.spawn_id AND sln.id = sle.spawn_location_id AND sln.id = slp.spawn_location_id AND slp.zone_id=%u ORDER BY sln.id, sle.id",
		z->GetID());

	if (!ret)
		return ret;

	uint32_t count = 0;
	uint32_t location_id = 0xFFFFFFFF;
	std::shared_ptr<SpawnLocation> spawn_location = nullptr;
	while (result.Next()) {
		uint32_t id = result.GetUInt32(0);
		if (location_id == 0xFFFFFFFF || id != location_id) {
			if (spawn_location) {
				z->AddNPCSpawnLocation(location_id, spawn_location);
				count++;
			}

			spawn_location = std::make_shared<SpawnLocation>();
		}

		std::shared_ptr<SpawnEntry> entry = std::make_shared<SpawnEntry>();
		location_id = id;

		// spawn_location_name
		spawn_location->name = result.GetString(1);

		// spawn_location_placement
		spawn_location->placement_id = result.GetUInt32(2);
		spawn_location->x = result.GetFloat(3);
		spawn_location->y = result.GetFloat(4);
		spawn_location->z = result.GetFloat(5);
		spawn_location->x_offset = result.GetFloat(6);
		spawn_location->y_offset = result.GetFloat(7);
		spawn_location->z_offset = result.GetFloat(8);
		spawn_location->heading = result.GetFloat(9);
		spawn_location->pitch = result.GetFloat(10);
		spawn_location->roll = result.GetFloat(11);
		spawn_location->respawn = result.GetUInt32(12);
		// TODO: Respawn_offset
		spawn_location->expire_time = result.GetUInt32(13);
		spawn_location->expire_offset = result.GetUInt32(14);
		spawn_location->grid_id = result.GetUInt32(15);

		// spawn_location_entry
		entry->spawn_location_id = location_id;
		entry->spawn_entry_id = result.GetUInt32(16);
		entry->spawn_id = result.GetUInt32(17);
		entry->spawn_percentage = result.GetFloat(18);
		entry->condition = result.GetUInt32(19);
		entry->spawn_type = SpawnEntryType::ENPC;

		spawn_location->total_percentage += entry->spawn_percentage;
		spawn_location->AddEntry(entry);
	}

	if (spawn_location) {
		z->AddNPCSpawnLocation(location_id, spawn_location);
		count++;
	}

	LogInfo(LOG_NPC, 0, "--Loaded %u NPC spawn location(s).", count);
	return ret;
}

constexpr const char* GetSpawnTableFields() {
	return "s.id, s.name,s.sub_title,s.prefix,s.suffix,s.last_name,s.race,s.model_type,s.size,s.size_offset,s.entity_flags,s.targetable,s.show_name,"
		"s.command_primary,s.command_secondary,s.visual_state,s.attackable,s.show_level,s.show_command_icon,s.display_hand_icon,s.faction_id,"
		"s.collision_radius,s.hp,s.power,s.savagery,s.dissonance,s.merchant_id,s.transport_id,s.merchant_type";
}

uint32_t ZoneDatabase::ProcessSpawnTableFields(const std::shared_ptr<Spawn>& spawn, DatabaseResult& res) {
	uint32_t i = 0;

	spawn->SetDatabaseID(res.GetUInt32(i++));
	spawn->SetName(res.GetString(i++));
	spawn->SetGuild(res.GetString(i++));
	spawn->SetPrefixTitle(res.GetString(i++));
	spawn->SetSuffixTitle(res.GetString(i++));
	spawn->SetLastName(res.GetString(i++));
	spawn->SetRace(res.GetUInt8(i++));
	spawn->SetModelType(res.GetUInt32(i++));
	spawn->SetSize(res.GetFloat(i++));
	spawn->SetSizeOffset(res.GetFloat(i++));
	uint32_t entityFlags = 0;//res.GetUInt32(i++);
	i++;
	//Targetable
	if (res.GetBool(i++)) {
		entityFlags |= EntityFlagTargetable;
	}
	//Show name
	res.GetBool(i++);
	spawn->SetPrimaryCommandListID(res.GetUInt32(i++));
	spawn->SetSecondaryCommandListID(res.GetUInt32(i++));
	spawn->SetVisualState(res.GetUInt32(i++));
	//Attackable
	if (res.GetBool(i++)) {
		spawn->SetAttackable(true);
		entityFlags |= EntityFlagShowCommandIcon | EntityFlagShowLevel;
	}
	//Show level
	if (res.GetBool(i++)) {
		entityFlags |= EntityFlagShowLevel;
	}
	//Show command icon
	if (res.GetBool(i++)) {
		entityFlags |= EntityFlagShowCommandIcon;
	}
	//Display hand icon
	if (res.GetBool(i++)) {
		entityFlags |= EntityFlagInteractable;
	}
	spawn->SetFactionID(res.GetUInt32(i++));
	spawn->SetCollisionRadius(res.GetFloat(i++));
	spawn->SetHP(res.GetUInt32(i++));
	spawn->SetPower(res.GetUInt32(i++));
	spawn->SetSavagery(res.GetUInt32(i++));
	spawn->SetDissonance(res.GetUInt32(i++));
	spawn->SetMerchantID(res.GetUInt32(i++));
	if (spawn->GetMerchantID() > 0) {
		spawn->EnableEntityFlags(EntityFlagMerchant);
	}
	//Transport ID
	res.GetUInt32(i++);
	spawn->SetMerchantType(res.GetUInt32(i++));

	spawn->SetEntityFlags(entityFlags);
	spawn->PopUpdateFlags();

	return i;
}

//The input result to this function should be indexed by (id, type, red, green, blue)
void ZoneDatabase::ProcessEntityColors(DatabaseResult& result, std::unordered_map<uint32_t, std::shared_ptr<Entity> >& outEntities) {
	uint32_t lastID = 0;
	std::shared_ptr<Entity> entity;

	union {
		uint8_t uVal[3];
		int8_t sVal[3];
	};

	//Major hackery..
	EQ2Color& c = reinterpret_cast<EQ2Color&>(uVal[0]);
	int8_t* v = sVal;

	while (result.Next()) {
		uint32_t id = result.GetUInt32(0);
		if (id != lastID) {
			lastID = id;
			entity = outEntities[id];
		}

		assert(entity);

		CStringCmpNoCopy type = result.GetString(1);

		//Union + casting trickery to reduce the code we need here
		c.Red = static_cast<uint8_t>(result.GetInt32(2));
		c.Green = static_cast<uint8_t>(result.GetInt32(3));
		c.Blue = static_cast<uint8_t>(result.GetInt32(4));

		if (type == "skin_color") {
			entity->SetSkinColor(c, false);
		}
		else if (type == "eye_color") {
			entity->SetEyeColor(c, false);
		}
		else if (type == "hair_color1") {
			entity->SetHairColor1(c, false);
		}
		else if (type == "hair_color2") {
			entity->SetHairColor2(c, false);
		}
		else if (type == "hair_highlight") {
			entity->SetHairHighlight(c, false);
		}
		else if (type == "hair_type_color") {
			entity->SetHairColor(c, false);
		}
		else if (type == "hair_type_highlight_color") {
			entity->SetHairHighlightColor(c, false);
		}
		else if (type == "hair_face_color") {
			entity->SetFacialHairColor(c);
		}
		else if (type == "hair_face_highlight_color") {
			entity->SetFacialHairHighlightColor(c, false);
		}
		else if (type == "wing_color1") {
			entity->SetWingColor(c, false);
		}
		else if (type == "wing_color2") {
			entity->SetWingHighlightColor(c, false);
		}
		else if (type == "shirt_color") {
			entity->SetChestColor(c, false);
		}
		else if (type == "unknown_chest_color") {
			entity->SetChestHighlightColor(c, false);
		}
		else if (type == "pants_color") {
			entity->SetLegsColor(c, false);
		}
		else if (type == "unknown_legs_color") {
			entity->SetLegsHighlightColor(c, false);
		}
		else if (type == "unknown9") {
			//guessing since this is right before eye type this is skull?
			entity->SetSkullType(v[0], 0, false);
			entity->SetSkullType(v[1], 1, false);
			entity->SetSkullType(v[2], 2, false);
		}
		else if (type == "eye_type") {
			entity->SetEyeType(v[0], 0, false);
			entity->SetEyeType(v[1], 1, false);
			entity->SetEyeType(v[2], 2, false);
		}
		else if (type == "ear_type") {
			entity->SetEarType(v[0], 0, false);
			entity->SetEarType(v[1], 1, false);
			entity->SetEarType(v[2], 2, false);
		}
		else if (type == "eye_brow_type") {
			entity->SetEyeBrowType(v[0], 0, false);
			entity->SetEyeBrowType(v[1], 1, false);
			entity->SetEyeBrowType(v[2], 2, false);
		}
		else if (type == "cheek_type") {
			entity->SetCheekType(v[0], 0, false);
			entity->SetCheekType(v[1], 1, false);
			entity->SetCheekType(v[2], 2, false);
		}
		else if (type == "lip_type") {
			entity->SetLipType(v[0], 0, false);
			entity->SetLipType(v[1], 1, false);
			entity->SetLipType(v[2], 2, false);
		}
		else if (type == "chin_type") {
			entity->SetChinType(v[0], 0, false);
			entity->SetChinType(v[1], 1, false);
			entity->SetChinType(v[2], 2, false);
		}
		else if (type == "nose_type") {
			entity->SetNoseType(v[0], 0, false);
			entity->SetNoseType(v[1], 1, false);
			entity->SetNoseType(v[2], 2, false);
		}
		else if (type == "body_size") {
			entity->SetBodySize(v[0], false);
		}
		else if (type == "soga_skin_color") {
			entity->SetSogaSkinColor(c, false);
		}
		else if (type == "soga_eye_color") {
			entity->SetSogaEyeColor(c, false);
		}
		else if (type == "soga_hair_color1") {
			entity->SetSogaHairColor1(c, false);
		}
		else if (type == "soga_hair_color2") {
			entity->SetSogaHairColor2(c, false);
		}
		else if (type == "soga_hair_highlight") {
			entity->SetSogaHairHighlight(c, false);
		}
		else if (type == "soga_hair_type_color") {
			entity->SetSogaHairColor(c, false);
		}
		else if (type == "soga_hair_type_highlight_color") {
			entity->SetSogaHairHighlightColor(c, false);
		}
		else if (type == "soga_hair_face_color") {
			entity->SetSogaFacialHairColor(c, false);
		}
		else if (type == "soga_hair_face_highlight_color") {
			entity->SetSogaFacialHairHighlightColor(c, false);
		}
		else if (type == "soga_wing_color1") {
		}
		else if (type == "soga_wing_color2") {
		}
		else if (type == "soga_shirt_color") {
		}
		else if (type == "soga_unknown_chest_color") {
		}
		else if (type == "soga_pants_color") {
		}
		else if (type == "soga_unknown_legs_color") {
		}
		else if (type == "soga_unknown13") {
			entity->SetSogaSkullType(v[0], 0, false);
			entity->SetSogaSkullType(v[1], 1, false);
			entity->SetSogaSkullType(v[2], 2, false);
		}
		else if (type == "soga_eye_type") {
			entity->SetSogaEyeType(v[0], 0, false);
			entity->SetSogaEyeType(v[1], 1, false);
			entity->SetSogaEyeType(v[2], 2, false);
		}
		else if (type == "soga_ear_type") {
			entity->SetSogaEarType(v[0], 0, false);
			entity->SetSogaEarType(v[1], 1, false);
			entity->SetSogaEarType(v[2], 2, false);
		}
		else if (type == "soga_eye_brow_type") {
			entity->SetSogaEyeBrowType(v[0], 0, false);
			entity->SetSogaEyeBrowType(v[1], 1, false);
			entity->SetSogaEyeBrowType(v[2], 2, false);
		}
		else if (type == "soga_cheek_type") {
			entity->SetSogaCheekType(v[0], 0, false);
			entity->SetSogaCheekType(v[1], 1, false);
			entity->SetSogaCheekType(v[2], 2, false);
		}
		else if (type == "soga_lip_type") {
			entity->SetSogaLipType(v[0], 0, false);
			entity->SetSogaLipType(v[1], 1, false);
			entity->SetSogaLipType(v[2], 2, false);
		}
		else if (type == "soga_chin_type") {
			entity->SetSogaChinType(v[0], 0, false);
			entity->SetSogaChinType(v[1], 1, false);
			entity->SetSogaChinType(v[2], 2, false);
		}
		else if (type == "soga_nose_type") {
			entity->SetSogaNoseType(v[0], 0, false);
			entity->SetSogaNoseType(v[1], 1, false);
			entity->SetSogaNoseType(v[2], 2, false);
		}
	}
}

//The input result to this function should be indexed by (spawn_id, slot_id, equip_type, red, green, blue, 
//highlight_red, highlight_green, highlight_blue)
void ZoneDatabase::ProcessNpcAppearanceEquipment(DatabaseResult& result, std::unordered_map<uint32_t, std::shared_ptr<Entity> >& outEntities) {
	std::shared_ptr<Entity> entity;
	uint32_t lastID = 0;
	EQ2EquipmentItem item;

	while (result.Next()) {
		uint32_t id = result.GetUInt32(0);
		if (id != lastID) {
			entity = outEntities[id];
			lastID = id;
		}

		assert(entity);

		uint8_t slot = result.GetUInt8(1);
		
		item.type = result.GetUInt32(2);
		item.color.Red = result.GetUInt8(3);
		item.color.Green = result.GetUInt8(4);
		item.color.Blue = result.GetUInt8(5);
		item.highlight.Red = result.GetUInt8(6);
		item.highlight.Green = result.GetUInt8(7);
		item.highlight.Blue = result.GetUInt8(8);

		entity->SetAppearanceEquipmentItem(slot, item, false);
	}
}