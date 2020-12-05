#include "stdafx.h"

#include "CharacterSheet.h"
#include "../Packets/OP_UpdateCharacterSheetMsg_Packet.h"

void UpdateCharacterSheetMsgData::PreWrite() {
	//Convert data that is a different format in the packet vs what we use
	advExp = static_cast<float>(experience.currentAdvExp);
	advExpNextLevel = static_cast<float>(experience.nextAdvLevelExp);
	advExpDebt = static_cast<float>(experience.advExpDebt);
	advVitality = static_cast<float>(experience.advVitality);
	tsExp = static_cast<float>(experience.currentTsExp);
	tsExpNextLevel = static_cast<float>(experience.nextTsLevelExp);
	tsVitality = static_cast<float>(experience.tsVitality);
	hp = attributes->hp.currentValue;
	maxHp = attributes->hp.maxValue;
	maxHpBase = attributes->hp.baseValue;
	bonus_health_do_not_set = static_cast<int32_t>(bonus_health);

	str_do_not_set = static_cast<int16_t>(attributes->str.currentValue);
	sta_do_not_set = static_cast<int16_t>(attributes->sta.currentValue);
	wis_do_not_set = static_cast<int16_t>(attributes->wis.currentValue);
	int_do_not_set = static_cast<int16_t>(attributes->intel.currentValue);
	agi_do_not_set = static_cast<int16_t>(attributes->agi.currentValue);

	str_base_do_not_set = static_cast<int16_t>(attributes->str.baseValue);
	sta_base_do_not_set = static_cast<int16_t>(attributes->sta.baseValue);
	wis_base_do_not_set = static_cast<int16_t>(attributes->wis.baseValue);
	int_base_do_not_set = static_cast<int16_t>(attributes->intel.baseValue);
	agi_base_do_not_set = static_cast<int16_t>(attributes->agi.baseValue);

	if (GetVersion() >= 67730) {
		advExp_do_not_set = advExp;
		advExpNextLevel_do_not_set = advExpNextLevel;
		tsExp_do_not_set = tsExp;
		tsExpNextLevel_do_not_set = tsExpNextLevel;
	}
}

void CharacterUpdateGenerator::LinkUpdateFields(const CharacterSheet& sheet) {
	std::string charID = std::to_string(sheet.characterID);

	characterUpdates.m_tableName = "characters";
	characterUpdates.m_criteria = "WHERE id = " + charID;

	characterDetailsUpdates.m_tableName = "character_details";
	characterDetailsUpdates.m_criteria = "WHERE char_id = " + charID;

	LinkCharacterFields(sheet);
	LinkCharacterDetailsFields(sheet);
}

void CharacterUpdateGenerator::LinkCharacterFields(const CharacterSheet& sheet) {
	const auto& entity = sheet.entity;
	const SpawnInfoStruct* info = entity->GetInfoStruct();
	const SpawnPositionStruct* pos = entity->GetPosStruct();

	characterUpdates.RegisterField("name", sheet.namingInfo->name);
	characterUpdates.RegisterField("race", *sheet.race);
	characterUpdates.RegisterField("class", sheet.advClass);
	characterUpdates.RegisterField("gender", *sheet.gender);
	characterUpdates.RegisterField("deity", sheet.deityID);

	//body_size/body_age are converted from an int to float when saved to the db
	struct Int8ToFloatManip {
		float Transform(const int8_t& v) {
			return v / 127.f;
		}
	};

	characterUpdates.RegisterField<int8_t, Int8ToFloatManip>("body_size", entity->GetInfoStruct()->sliders.bodyscale);
	characterUpdates.RegisterField<int8_t, Int8ToFloatManip>("body_age", entity->GetInfoStruct()->sliders.bumpscale);

	characterUpdates.RegisterField("current_zone_id", sheet.zoneID);
	characterUpdates.RegisterField("level", *sheet.advLevel);
	characterUpdates.RegisterField("tradeskill_class", sheet.tsClass);
	characterUpdates.RegisterField("tradeskill_level", sheet.tsLevel);
	characterUpdates.RegisterField("soga_wing_type", info->soga_wing_type_id);
	characterUpdates.RegisterField("soga_chest_type", info->soga_chest_type_id);
	characterUpdates.RegisterField("soga_legs_type", info->soga_legs_type_id);
	characterUpdates.RegisterField("soga_hair_type", info->soga_hair_type_id);
	characterUpdates.RegisterField("soga_facial_hair_type", info->soga_facial_hair_type_id);
	characterUpdates.RegisterField("soga_model_type", info->soga_model_type);
	characterUpdates.RegisterField("legs_type", info->legs_type_id);
	characterUpdates.RegisterField("chest_type", info->chest_type_id);
	characterUpdates.RegisterField("wing_type", info->wing_type_id);
	characterUpdates.RegisterField("hair_type", info->hair_type_id);
	characterUpdates.RegisterField("facial_hair_type", info->facial_hair_type_id);
	characterUpdates.RegisterField("model_type", info->model_type);
	characterUpdates.RegisterField("x", pos->x);
	characterUpdates.RegisterField("y", pos->y);
	characterUpdates.RegisterField("z", pos->z);
	characterUpdates.RegisterField("heading", pos->heading);
	characterUpdates.RegisterField("instance_id", sheet.instanceID);
}

void CharacterUpdateGenerator::LinkCharacterDetailsFields(const CharacterSheet& sheet) {
	const auto& entity = sheet.entity;
	const SpawnInfoStruct* info = entity->GetInfoStruct();
	const EntityAttributeSheet* attributes = sheet.attributes;
	const CharacterCurrency& currency = sheet.currency;
	const CharacterExperience& exp = sheet.experience;

	characterDetailsUpdates.RegisterField("hp", attributes->hp.currentValue);
	characterDetailsUpdates.RegisterField("power", attributes->power.currentValue);
	characterDetailsUpdates.RegisterField("savagery", attributes->savagery.currentValue);
	characterDetailsUpdates.RegisterField("dissonance", attributes->dissonance.baseValue);
	characterDetailsUpdates.RegisterField("max_concentration", attributes->concentration.maxValue);
	characterDetailsUpdates.RegisterField("mitigation", attributes->mitigation.baseValue);
	characterDetailsUpdates.RegisterField("avoidance", attributes->avoidance.baseValue);
	characterDetailsUpdates.RegisterField("str", attributes->str.baseValue);
	characterDetailsUpdates.RegisterField("sta", attributes->sta.baseValue);
	characterDetailsUpdates.RegisterField("agi", attributes->agi.baseValue);
	characterDetailsUpdates.RegisterField("wis", attributes->wis.baseValue);
	characterDetailsUpdates.RegisterField("intel", attributes->intel.baseValue);
	characterDetailsUpdates.RegisterField("heat", attributes->heat.baseValue);
	characterDetailsUpdates.RegisterField("cold", attributes->cold.baseValue);
	characterDetailsUpdates.RegisterField("magic", attributes->magic.baseValue);
	characterDetailsUpdates.RegisterField("mental", attributes->mental.baseValue);
	characterDetailsUpdates.RegisterField("divine", attributes->divine.baseValue);
	characterDetailsUpdates.RegisterField("disease", attributes->disease.baseValue);
	characterDetailsUpdates.RegisterField("poison", attributes->poison.baseValue);
	characterDetailsUpdates.RegisterField("elemental", attributes->elemental.baseValue);
	characterDetailsUpdates.RegisterField("arcane", attributes->arcane.baseValue);
	characterDetailsUpdates.RegisterField("noxious", attributes->noxious.baseValue);
	characterDetailsUpdates.RegisterField("coin_copper", currency.copper);
	characterDetailsUpdates.RegisterField("coin_silver", currency.silver);
	characterDetailsUpdates.RegisterField("coin_gold", currency.gold);
	characterDetailsUpdates.RegisterField("coin_plat", currency.platinum);
	characterDetailsUpdates.RegisterField("pet_name", sheet.petName);
	characterDetailsUpdates.RegisterField("status_points", sheet.statusPoints);
	characterDetailsUpdates.RegisterField("max_power", attributes->power.baseValue);
	characterDetailsUpdates.RegisterField("max_hp", attributes->hp.baseValue);
	characterDetailsUpdates.RegisterField("max_savagery", attributes->savagery.maxValue);
	characterDetailsUpdates.RegisterField("max_dissonance", attributes->dissonance.maxValue);
	characterDetailsUpdates.RegisterField("xp", exp.currentAdvExp);
	characterDetailsUpdates.RegisterField("xp_needed", exp.nextAdvLevelExp);
	characterDetailsUpdates.RegisterField("xp_debt", exp.advExpDebt);
	characterDetailsUpdates.RegisterField("xp_vitality", exp.advVitality);
	characterDetailsUpdates.RegisterField("tradeskill_xp", exp.currentTsExp);
	characterDetailsUpdates.RegisterField("tradesekill_xp_needed", exp.nextTsLevelExp);
	characterDetailsUpdates.RegisterField("tradeskill_xp_vitality", exp.tsVitality);
	characterDetailsUpdates.RegisterField("bank_copper", currency.bankCopper);
	characterDetailsUpdates.RegisterField("bank_silver", currency.bankSilver);
	characterDetailsUpdates.RegisterField("bank_gold", currency.bankGold);
	characterDetailsUpdates.RegisterField("bank_plat", currency.bankPlatinum);
	characterDetailsUpdates.RegisterField("bind_zone_id", sheet.bindZone);
	characterDetailsUpdates.RegisterField("bind_x", sheet.bindLocation.x);
	characterDetailsUpdates.RegisterField("bind_y", sheet.bindLocation.y);
	characterDetailsUpdates.RegisterField("bind_z", sheet.bindLocation.z);
	characterDetailsUpdates.RegisterField("bind_heading", sheet.bindHeading);
	characterDetailsUpdates.RegisterField("house_zone_id", sheet.houseZoneId);
	characterDetailsUpdates.RegisterField("combat_voice", *sheet.combatVoice);
	characterDetailsUpdates.RegisterField("emote_voice", *sheet.emoteVoice);
	characterDetailsUpdates.RegisterField("biography", sheet.biography);
	characterDetailsUpdates.RegisterField("flags", sheet.flags);
	characterDetailsUpdates.RegisterField("flags2", sheet.flags2);
	//TODO titles
	characterDetailsUpdates.RegisterField("current_language", sheet.currentLanguage);
	characterDetailsUpdates.RegisterField("last_name", sheet.namingInfo->last_name);
}

bool CharacterUpdateGenerator::GenerateUpdate(std::ostringstream& ss) {
	bool ret = false;
	ret |= characterUpdates.CheckForUpdates(ss);
	ret |= characterDetailsUpdates.CheckForUpdates(ss);
	return ret;
}

void UpdateCharacterSheetMsgData::RegisterAttributeElements() {
	if (version >= 60114) {
		int32_t& str = attributes->str.currentValue;
		int32_t& sta = attributes->sta.currentValue;
		int32_t& agi = attributes->agi.currentValue;
		int32_t& wis = attributes->wis.currentValue;
		int32_t& intel = attributes->intel.currentValue;
		RegisterInt32(str);
		RegisterInt32(sta);
		RegisterInt32(agi);
		RegisterInt32(wis);
		RegisterInt32(intel);
		int32_t& str_base = attributes->str.baseValue;
		int32_t& sta_base = attributes->sta.baseValue;
		int32_t& agi_base = attributes->agi.baseValue;
		int32_t& wis_base = attributes->wis.baseValue;
		int32_t& intel_base = attributes->intel.baseValue;
		RegisterInt32(str_base);
		RegisterInt32(sta_base);
		RegisterInt32(agi_base);
		RegisterInt32(wis_base);
		RegisterInt32(intel_base);
	}
	else {
		int16_t& str = str_do_not_set;
		int16_t& sta = sta_do_not_set;
		int16_t& agi = agi_do_not_set;
		int16_t& wis = wis_do_not_set;
		int16_t& intel = int_do_not_set;	
		RegisterInt16(str);
		RegisterInt16(sta);
		RegisterInt16(agi);
		RegisterInt16(wis);
		RegisterInt16(intel);
		int16_t& str_base = str_base_do_not_set;
		int16_t& sta_base = sta_base_do_not_set;
		int16_t& agi_base = agi_base_do_not_set;
		int16_t& wis_base = wis_base_do_not_set;
		int16_t& intel_base = int_base_do_not_set;
		RegisterInt16(str_base);
		RegisterInt16(sta_base);
		RegisterInt16(agi_base);
		RegisterInt16(wis_base);
		RegisterInt16(intel_base);
	}
}

void UpdateCharacterSheetMsgData::RegisterElements() {
	const uint32_t version = this->version;
	if (version >= 67650) {
		RegisterElements67650();
		return;
	}

	static EntityAttributeSheet structDumperHackSheet;

	EntityAttributeSheet* attributes = this->attributes ? this->attributes : &structDumperHackSheet;

	std::string& char_name = namingInfo->name;
	RegisterCharString(char_name, 42);
	uint8_t& race = *this->race;
	RegisterUInt8(race);
	uint8_t& gender = *this->gender;
	RegisterUInt8(gender);
	RegisterUInt8(alignment);
	RegisterUInt32(advArchetype);
	RegisterUInt32(advBaseClass);
	RegisterUInt32(advClass);
	RegisterUInt32(tsArchetype);
	RegisterUInt32(tsBaseClass);
	RegisterUInt32(tsClass);
	uint16_t& level = *advOrigLevel;
	uint16_t& effective_level = *advLevel;
	RegisterUInt16(level);
	RegisterUInt16(effective_level);
	RegisterUInt16(tsLevel);
	RegisterInt32(gmLevel); //0-15
	RegisterUInt16(account_age_base);
	RescopeToReference(account_flags, uint16_t);
	RegisterUInt16(account_flags);

	//Replace deity/fb/twitter later
	static std::string deity = "deity";
	RegisterCharString(deity, 32);
	std::string& last_name = namingInfo->last_name;
	RegisterCharString(last_name, 21);
	std::string& twitter_prepend = namingInfo->name;
	RegisterCharString(twitter_prepend, 42);
	std::string& facebook_prepend = namingInfo->name;
	RegisterCharString(facebook_prepend, 42);

	if (version <= 1208) {
		int32_t& hp = attributes->hp.currentValue;
		int32_t& maxHp = attributes->hp.maxValue;
		int32_t& maxHpBase = attributes->hp.baseValue;

		RegisterInt32(hp);
		RegisterInt32(maxHp);
		RegisterInt32(maxHpBase);
	}
	else {
		RegisterInt64(hp);
		RegisterInt64(maxHp);
		RegisterInt64(maxHpBase);
	}

	int32_t& current_power = attributes->power.currentValue;
	int32_t& max_power = attributes->power.maxValue;
	int32_t& base_power = attributes->power.baseValue;
	RegisterInt32(current_power);
	RegisterInt32(max_power);
	RegisterInt32(base_power);

	auto& conc_used = reinterpret_cast<uint8_t&>(attributes->concentration.currentValue);
	auto& conc_max = reinterpret_cast<uint8_t&>(attributes->concentration.maxValue);
	RegisterUInt8(conc_used);
	RegisterUInt8(conc_max);

	int32_t& savagery = attributes->savagery.currentValue;
	int32_t& max_savagery = attributes->savagery.maxValue;
	int32_t& base_savagery = attributes->savagery.baseValue;
	RegisterInt32(savagery);
	RegisterInt32(max_savagery);
	RegisterInt32(base_savagery);
	RegisterInt32(savagery_level);
	RegisterInt32(max_savagery_level);
	RegisterInt32(base_savagery_level);

	if (version >= 60114) {
		RegisterInt32(dissonance);
		RegisterInt32(max_dissonance);
		RegisterInt32(base_dissonance);
	}

	RegisterInt32(hp_regen);
	RegisterInt32(power_regen);
	RegisterInt32(savagery_regen_per_sec);

	if (version >= 60114) {
		RegisterInt32(dissipation);
	}

	RegisterFloat(ranged_attack_min_distance);
	RegisterFloat(ranged_attack_max_distance);
	RegisterFloat(stat_bonus_health);
	RegisterFloat(stat_bonus_power);

	if (version < 57048) {
		int32_t& bonus_health = bonus_health_do_not_set;
		RegisterInt32(bonus_health);
	}
	else {
		RegisterInt64(bonus_health);
	}

	RegisterInt32(bonus_power);
	RegisterFloat(stat_bonus_damage);
	RegisterUInt16(mitigation_pct_pve);
	RegisterUInt16(mitigation_pct_pvp);
	RegisterUInt16(toughness);
	RegisterFloat(toughness_resist_dmg_pvp);
	RegisterUInt16(lethality);
	RegisterFloat(lethality_pct);
	RegisterInt16(avoidance_overall_chance);
	RegisterInt16(avoidance_base);
	RegisterInt16(avoidance_riposte_chance);
	RegisterInt16(avoidance_riposte_chance_base);
	RegisterInt16(avoidance_base_chance);
	RegisterInt16(avoidance_base_chance_base);
	RegisterInt16(avoidance_parry_chance);
	RegisterInt16(avoidance_parry_chance_base);
	RegisterInt16(avoidance_block_chance);
	RegisterInt16(avoidance_block_chance_base);
	RegisterInt16(avoidance_block_uncontested);
	if (version >= 60114) {
		RegisterInt16(avoidance_deflection_uncontested);
		RegisterInt16(avoidance_riposte_uncontested);
	}
	RegisterInt16(avoidance_dodge_uncontested);
	if (version >= 60114) {
		RegisterInt16(avoidance_parry_uncontested);
	}

	RegisterAttributeElements();

	int32_t& mitigation_cur = attributes->mitigation.currentValue;
	RegisterInt32(mitigation_cur);

	int32_t& noxious = attributes->noxious.currentValue;
	int32_t& elemental = attributes->elemental.currentValue;
	int32_t& arcane = attributes->arcane.currentValue;
	RegisterInt32(elemental);
	RegisterInt32(noxious);
	RegisterInt32(arcane);
	int32_t& mitigation_base = attributes->mitigation.baseValue;
	RegisterInt32(mitigation_base);
	int32_t& noxious_base = attributes->noxious.baseValue;
	int32_t& elemental_base = attributes->elemental.baseValue;
	int32_t& arcane_base = attributes->arcane.baseValue;
	RegisterInt32(elemental_base);
	RegisterInt32(noxious_base);
	RegisterInt32(arcane_base);

	RegisterUInt16(physical_absorb_pve);
	RegisterUInt16(elemental_absorb_pve);
	RegisterUInt16(noxious_absorb_pve);
	RegisterUInt16(arcane_absorb_pve);
	RegisterUInt16(physical_absorb_pvp);
	RegisterUInt16(elemental_absorb_pvp);
	RegisterUInt16(noxious_absorb_pvp);
	RegisterUInt16(arcane_absorb_pvp);
	RegisterUInt16(physical_dmg_reduction);
	RegisterUInt16(elemental_dmg_reduction);
	RegisterUInt16(noxious_dmg_reduction);
	RegisterUInt16(arcane_dmg_reduction);
	RegisterUInt16(physical_dmg_reduction_pct);
	RegisterUInt16(elemental_dmg_reduction_pct);
	RegisterUInt16(noxious_dmg_reduction_pct);
	RegisterUInt16(arcane_dmg_reduction_pct);

	RegisterFloat(advExp);
	RegisterFloat(advExpNextLevel);
	RegisterFloat(advExpDebt);
	RegisterFloat(tsExp);
	RegisterFloat(tsExpNextLevel);
	RegisterFloat(tsExpDebt);

	if (version < 60114) {
		//Not sure what this was, but COE has it
		static float pvpExp = 0.f;
		float& pvpExpNextLevel = pvpExp;
		float& pvpExpDebt = pvpExp;

		RegisterFloat(pvpExp);
		RegisterFloat(pvpExpNextLevel);
		RegisterFloat(pvpExpDebt);
	}

	RegisterUInt16(server_bonus);
	RegisterUInt16(adventure_vet_bonus);
	RegisterUInt16(tradeskill_vet_bonus);
	RegisterUInt16(dungeon_finder_bonus);
	RegisterUInt16(recruit_friend_count);
	RegisterUInt16(recruit_friend_bonus);
	RegisterBool(bCanLevelPast90);
	RegisterUInt16(adventure_vitality);
	RegisterUInt16(adventure_vitality_yellow_arrow);
	RegisterUInt16(adventure_vitality_blue_arrow);
	RegisterUInt16(tradeskill_vitality);
	RegisterUInt16(tradeskill_vitality_purple_arrow);
	RegisterUInt16(tradeskill_vitality_blue_arrow);
	RegisterFloat(mentor_bonus);
	RegisterUInt16(earned_aa);
	RegisterUInt16(max_aa);
	RegisterUInt16(unassigned_aa);
	RegisterUInt16(aa_green_bar);
	RegisterUInt16(adv_xp_to_aa_xp_slider);
	RegisterUInt16(aa_blue_bar);
	RegisterUInt16(aa_next_level);
	RegisterUInt16(aa_bonus_xp);

	RegisterUInt32(aa_level_up_events);
	RegisterUInt32(aa_items_found);
	RegisterUInt32(aa_named_npcs_killed);
	RegisterUInt32(aa_quests_completed);
	RegisterUInt32(aa_exploration_events);
	RegisterUInt32(aa_completed_collections);

	uint8_t& Unknown24 = unknown24[0];
	RegisterUInt8(Unknown24)->SetCount(16);

	RegisterUInt32(lastPictureSubmitionTime);
	RegisterUInt8(unknown25d);
	RegisterUInt16(prestige_earned_points);
	RegisterUInt16(prestige_max_points);
	RegisterFloat(prestige_xp_current);
	RegisterUInt16(ts_aa_earned_points);
	RegisterUInt16(ts_aa_max_points);
	RegisterFloat(ts_aa_experience_current);
	RegisterUInt16(ts_prestige_earned_points);
	RegisterUInt16(ts_prestige_max_points);
	RegisterFloat(ts_prestige_experience_current);
	RegisterFloat(pet_experience_current);

	uint32_t& coins_copper = currency.copper;
	uint32_t& coins_silver = currency.silver;
	uint32_t& coins_gold = currency.gold;
	uint32_t& coins_plat = currency.platinum;
	RegisterUInt32(coins_copper);
	RegisterUInt32(coins_silver);
	RegisterUInt32(coins_gold);
	RegisterUInt32(coins_plat);

	RescopeArrayElement(unknown34);
	RegisterUInt32(unknown34)->SetCount(5);

	// Error parsing the struct, unknown is an unknown type(spell_effects);
	RescopeArrayElement(spell_effects);
	RegisterSubstruct(spell_effects)->SetCount(45);

	// Error parsing the struct, unknown is an unknown type(detrimental_spell_effects);
	RescopeArrayElement(detrimental_spell_effects);
	RegisterSubstruct(detrimental_spell_effects)->SetCount(45);

	RescopeArrayElement(passive_spell_effects);
	RegisterSubstruct(passive_spell_effects)->SetCount(100);
	RescopeArrayElement(passive_unk_bool)
		auto pe = RegisterBool(passive_unk_bool);
	pe->SetCount(128);
	RegisterInt32(passive_last_index);

	if (version >= 60114) {
		uint8_t& Unknown35 = unknown35[0];
		RegisterUInt8(Unknown35)->SetCount(354);
	}

	RegisterUInt8(trauma_count);
	RegisterUInt8(arcane_count);
	RegisterUInt8(noxious_count);
	RegisterUInt8(elemental_count);
	RegisterUInt8(curse_count);

	// Error parsing the struct, unknown is an unknown type(maintained_effects);
	RescopeArrayElement(maintained_effects);
	RegisterSubstruct(maintained_effects)->SetCount(30);

	RegisterFloat(breath);
	RegisterUInt32(breathable_environment_flags);
	RegisterUInt8(unknown36);
	RegisterUInt32(melee_pri_dmg_min);
	RegisterUInt32(melee_pri_dmg_max);
	RegisterFloat(melee_pri_delay);
	RegisterUInt32(melee_sec_dmg_min);
	RegisterUInt32(melee_sec_dmg_max);
	RegisterFloat(melee_sec_delay);
	RegisterUInt32(ranged_dmg_min);
	RegisterUInt32(ranged_dmg_max);
	RegisterFloat(ranged_delay);
	RegisterUInt32(unknown37);
	RegisterUInt32(unknown38);
	RegisterUInt32(unknown39);
	RegisterUInt32(unknown40);
	RegisterFloat(ability_mod_pve);
	RegisterFloat(unknown41);
	RegisterFloat(unknown42);
	RegisterFloat(unknown43);
	RegisterFloat(unknown44);
	RegisterFloat(ability_mod_pvp);
	RegisterFloat(base_melee_crit);
	RegisterFloat(base_spell_crit);
	RegisterFloat(base_taunt_crit);
	RegisterFloat(base_heal_crit);
	RegisterUInt32(flags);
	RegisterUInt32(flags2);
	static std::string afk_message = "afk_message";
	RegisterCharString(afk_message, 256);
	RegisterSubstruct(pve_props);
	if (version >= 60114) {
		RegisterSubstruct(unk_props);
	}
	RegisterSubstruct(pvp_props);
	RegisterSubstruct(ts_props);
	RegisterSubstruct(prop_caps);

	RegisterFloat(adventure_effects_cap);
	RegisterFloat(tradeskill_effects_cap);
	RegisterFloat(aa_effects_cap);

	RescopeArrayElement(unknown525a);
	RegisterUInt8(unknown525a)->SetCount(version >= 60114 ? 36 : 28);
	RegisterUInt64(spell_state_flags);
	RescopeArrayElement(unknown525b);
	RegisterUInt8(unknown525b)->SetCount(version >= 60114 ? 6 : 4);

	RegisterSubstruct(groupSheet);

	RegisterFloat(humidity);
	RegisterFloat(wind_direction);

	RescopeToReference(statusPoints, uint32_t);
	RescopeToReference(guild_status, uint32_t);
	RegisterUInt32(statusPoints);
	RegisterUInt32(guild_status);

	RegisterUInt8(unknownCOEa);

	//house zone and the following 151 bytes (199 total) appear to be the same object
	static std::string house_zone = "house zone";
	RegisterCharString(house_zone, 48);
	uint8_t& Unknown526 = unknown526[0];
	RegisterUInt8(Unknown526)->SetCount(151);


	//bind zone and the following bytes (84 total) appear to be the same object
	static std::string bind_zone = "bind zone";
	RegisterCharString(bind_zone, 32);
	uint8_t& Unknown188 = unknown188[0];
	RegisterUInt8(Unknown188)->SetCount(version >= 60114 ? 42 : 49);
}

void UpdateCharacterSheetMsgData::RegisterElements67650() {
	const uint32_t ver = GetVersion();

	//Eventually break up the stat blocks into substructs so we can combine these register functions cleanly
	static EntityAttributeSheet structDumperHackSheet;

	EntityAttributeSheet* attributes = this->attributes ? this->attributes : &structDumperHackSheet;

	std::string& char_name = namingInfo->name;
	RegisterCharString(char_name, 42);
	uint8_t& race = *this->race;
	RegisterUInt8(race);
	uint8_t& gender = *this->gender;
	RegisterUInt8(gender);
	RegisterUInt8(alignment);
	RegisterUInt32(advArchetype);
	RegisterUInt32(advBaseClass);
	RegisterUInt32(advClass);
	RegisterUInt32(tsArchetype);
	RegisterUInt32(tsBaseClass);
	RegisterUInt32(tsClass);
	uint16_t& level = *advOrigLevel;
	uint16_t& effective_level = *advLevel;
	RegisterUInt16(level);
	RegisterUInt16(effective_level);
	RegisterUInt16(tsLevel);
	RegisterUInt16(account_age_base);
	RegisterInt32(gmLevel); //0-15
	RegisterUInt32(account_flags);

	//Replace deity/fb/twitter later
	static std::string deity = "deity";
	RegisterCharString(deity, 32);
	static std::string ascension_class = "ascension_class";
	RegisterCharString(ascension_class, 32);
	static int16_t ascension_level = 0;
	RegisterInt16(ascension_level);
	static int16_t ascension_unknown = 0;
	RegisterInt16(ascension_unknown);
	std::string& last_name = namingInfo->last_name;
	RegisterCharString(last_name, 21);
	std::string& twitter_prepend = namingInfo->name;
	RegisterCharString(twitter_prepend, 42);
	std::string& facebook_prepend = namingInfo->name;
	RegisterCharString(facebook_prepend, 42);


	RegisterInt64(hp);
	RegisterInt64(maxHp);
	RegisterInt64(maxHpBase);

	int32_t& current_power = attributes->power.currentValue;
	int32_t& max_power = attributes->power.maxValue;
	int32_t& base_power = attributes->power.baseValue;
	RegisterInt32(current_power);
	RegisterInt32(max_power);
	RegisterInt32(base_power);

	auto& conc_used = reinterpret_cast<uint8_t&>(attributes->concentration.currentValue);
	auto& conc_max = reinterpret_cast<uint8_t&>(attributes->concentration.maxValue);
	RegisterUInt8(conc_used);
	RegisterUInt8(conc_max);

	int32_t& savagery = attributes->savagery.currentValue;
	int32_t& max_savagery = attributes->savagery.maxValue;
	int32_t& base_savagery = attributes->savagery.baseValue;
	RegisterInt32(savagery);
	RegisterInt32(max_savagery);
	RegisterInt32(base_savagery);

	RegisterInt32(savagery_level);
	RegisterInt32(max_savagery_level);
	RegisterInt32(base_savagery_level);

	RegisterInt32(dissonance);
	RegisterInt32(max_dissonance);
	RegisterInt32(base_dissonance);

	RegisterInt32(hp_regen);
	RegisterInt32(power_regen);
	RegisterInt32(savagery_regen_per_sec);
	RegisterInt32(dissipation);

	RegisterFloat(ranged_attack_min_distance);
	RegisterFloat(ranged_attack_max_distance);
	RegisterFloat(stat_bonus_health);
	RegisterFloat(stat_bonus_power);
	RegisterInt64(bonus_health);
	RegisterInt32(bonus_power);
	RegisterFloat(stat_bonus_damage);
	RegisterUInt16(mitigation_pct_pve);
	RegisterUInt16(mitigation_pct_pvp);
	RegisterUInt16(toughness);
	RegisterFloat(toughness_resist_dmg_pvp);
	RegisterUInt16(lethality);
	RegisterFloat(lethality_pct);

	RegisterInt16(avoidance_overall_chance);
	RegisterInt16(avoidance_base);
	RegisterInt16(avoidance_riposte_chance);
	RegisterInt16(avoidance_riposte_chance_base);
	RegisterInt16(avoidance_base_chance);
	RegisterInt16(avoidance_base_chance_base);
	RegisterInt16(avoidance_parry_chance);
	RegisterInt16(avoidance_parry_chance_base);
	RegisterInt16(avoidance_block_chance);
	RegisterInt16(avoidance_block_chance_base);
	RegisterInt16(avoidance_block_uncontested);
	RegisterInt16(avoidance_deflection_uncontested); //i think this was just left in by accident, not used
	RegisterInt16(avoidance_riposte_uncontested);
	RegisterInt16(avoidance_dodge_uncontested);
	RegisterInt16(avoidance_parry_uncontested);

	RegisterAttributeElements();

	int32_t& mitigation_cur = attributes->mitigation.currentValue;
	int32_t& noxious = attributes->noxious.currentValue;
	int32_t& elemental = attributes->elemental.currentValue;
	int32_t& arcane = attributes->arcane.currentValue;
	RegisterInt32(mitigation_cur);
	RegisterInt32(elemental);
	RegisterInt32(noxious);
	RegisterInt32(arcane);


	int32_t& mitigation_base = attributes->mitigation.baseValue;
	int32_t& noxious_base = attributes->noxious.baseValue;
	int32_t& elemental_base = attributes->elemental.baseValue;
	int32_t& arcane_base = attributes->arcane.baseValue;
	RegisterInt32(mitigation_base);
	RegisterInt32(elemental_base);
	RegisterInt32(noxious_base);
	RegisterInt32(arcane_base);

	RegisterUInt16(physical_absorb_pve);
	RegisterUInt16(elemental_absorb_pve);
	RegisterUInt16(noxious_absorb_pve);
	RegisterUInt16(arcane_absorb_pve);

	RegisterUInt16(physical_absorb_pvp);
	RegisterUInt16(elemental_absorb_pvp);
	RegisterUInt16(noxious_absorb_pvp);
	RegisterUInt16(arcane_absorb_pvp);

	RegisterUInt16(physical_dmg_reduction);
	RegisterUInt16(elemental_dmg_reduction);
	RegisterUInt16(noxious_dmg_reduction);
	RegisterUInt16(arcane_dmg_reduction);

	RegisterUInt16(physical_dmg_reduction_pct);
	RegisterUInt16(elemental_dmg_reduction_pct);
	RegisterUInt16(noxious_dmg_reduction_pct);
	RegisterUInt16(arcane_dmg_reduction_pct);

	if (ver >= 67730) {
		double& advExp = advExp_do_not_set;
		double& advExpNextLevel = advExpNextLevel_do_not_set;
		RegisterDouble(advExp);
		RegisterDouble(advExpNextLevel);
	}
	else {
		RegisterFloat(advExp);
		RegisterFloat(advExpNextLevel);
	}
	RegisterFloat(advExpDebt);

	if (ver >= 67730) {
		double& tsExp = tsExp_do_not_set;
		double& tsExpNextLevel = tsExpNextLevel_do_not_set;
		RegisterDouble(tsExp);
		RegisterDouble(tsExpNextLevel);
	}
	else {
		RegisterFloat(tsExp);
		RegisterFloat(tsExpNextLevel);
	}
	RegisterFloat(tsExpDebt);

	if (ver >= 67730) {
		static double titheExp = 0;
		static double titheExpNextLevel = 0;
		RegisterDouble(titheExp);
		RegisterDouble(titheExpNextLevel);
	}
	else {
		static float titheExp = 0;
		static float titheExpNextLevel = 0;
		RegisterFloat(titheExp);
		RegisterFloat(titheExpNextLevel);
	}
	static float titheExpDebt = 0;
	RegisterFloat(titheExpDebt);

	if (ver >= 67730) {
		static double ascensionExp = 0;
		static double ascensionExpNextLevel = 0;
		RegisterDouble(ascensionExp);
		RegisterDouble(ascensionExpNextLevel);
	}
	else {
		static float ascensionExp = 0;
		static float ascensionExpNextLevel = 0;
		RegisterFloat(ascensionExp);
		RegisterFloat(ascensionExpNextLevel);
	}
	static float ascensionExpDebt = 0;
	uint8_t ascensionUnk = 0;
	RegisterUInt8(ascensionUnk);
	RegisterFloat(ascensionExpDebt); //guess

	if (ver >= 67730) {
		static float unknownBOLc = 0;
		RegisterFloat(unknownBOLc); //guess
	}

	RegisterUInt16(server_bonus);
	RegisterUInt16(adventure_vet_bonus);
	RegisterUInt16(tradeskill_vet_bonus);
	RegisterUInt16(dungeon_finder_bonus);
	RegisterUInt16(recruit_friend_count);
	RegisterUInt16(recruit_friend_bonus);

	//if false xp bar hover at level 90 gives message about needing 280 aas and a certain xpac to level past 90
	RegisterBool(bCanLevelPast90);

	RegisterUInt16(adventure_vitality);
	RegisterUInt16(adventure_vitality_yellow_arrow);
	RegisterUInt16(adventure_vitality_blue_arrow);
	RegisterUInt16(tradeskill_vitality);
	RegisterUInt16(tradeskill_vitality_purple_arrow);
	RegisterUInt16(tradeskill_vitality_blue_arrow);

	RegisterFloat(mentor_bonus);
	RegisterUInt16(earned_aa);
	RegisterUInt16(max_aa);
	RegisterUInt16(unassigned_aa);
	RegisterUInt16(aa_green_bar);
	RegisterUInt16(adv_xp_to_aa_xp_slider);
	RegisterUInt16(aa_blue_bar);
	RegisterUInt16(aa_next_level);
	RegisterUInt16(aa_bonus_xp);
	RegisterUInt32(aa_level_up_events);
	RegisterUInt32(aa_items_found);
	RegisterUInt32(aa_named_npcs_killed);
	RegisterUInt32(aa_quests_completed);
	RegisterUInt32(aa_exploration_events);
	RegisterUInt32(aa_completed_collections);

	static uint8_t g_unknown251[0x266 - 0x251];
	uint8_t& unknown251 = g_unknown251[0];
	RegisterUInt8(unknown251)->SetCount(sizeof(g_unknown251));

	RegisterUInt16(prestige_earned_points);
	RegisterUInt16(prestige_max_points);
	RegisterFloat(prestige_xp_current);
	RegisterUInt16(ts_aa_earned_points);
	RegisterUInt16(ts_aa_max_points);
	RegisterFloat(ts_aa_experience_current);
	RegisterUInt16(ts_prestige_earned_points);
	RegisterUInt16(ts_prestige_max_points);
	RegisterFloat(ts_prestige_experience_current);
	RegisterFloat(pet_experience_current);

	uint32_t& coins_copper = currency.copper;
	uint32_t& coins_silver = currency.silver;
	uint32_t& coins_gold = currency.gold;
	uint32_t& coins_plat = currency.platinum;
	RegisterUInt32(coins_copper);
	RegisterUInt32(coins_silver);
	RegisterUInt32(coins_gold);
	RegisterUInt32(coins_plat);

	static uint8_t g_unknown2c9[0x2dd - 0x2c9];
	uint8_t& unknown2c9 = g_unknown2c9[0];
	RegisterUInt8(unknown2c9)->SetCount(sizeof(g_unknown2c9));

	RescopeArrayElement(spell_effects);
	RegisterSubstruct(spell_effects)->SetCount(45);
	RescopeArrayElement(detrimental_spell_effects);
	RegisterSubstruct(detrimental_spell_effects)->SetCount(45);
	RescopeArrayElement(passive_spell_effects);
	RegisterSubstruct(passive_spell_effects)->SetCount(100);
	RescopeArrayElement(passive_unk_bool);
	auto pe = RegisterBool(passive_unk_bool);
	pe->SetCount(128);
	RegisterInt32(passive_last_index);

	static uint8_t g_unknown2053[0x21b5 - 0x2053];
	uint8_t& unknown2053 = g_unknown2053[0];
	RegisterUInt8(unknown2053)->SetCount(sizeof(g_unknown2053));

	RegisterUInt8(trauma_count);
	RegisterUInt8(arcane_count);
	RegisterUInt8(noxious_count);
	RegisterUInt8(elemental_count);
	RegisterUInt8(curse_count);
	RescopeArrayElement(maintained_effects);
	RegisterSubstruct(maintained_effects)->SetCount(30);
	RegisterFloat(breath);
	RegisterUInt32(breathable_environment_flags);
	RegisterUInt8(unknown36);

	static uint8_t g_unknown2ca9[0x2d05 - 0x2ca9];
	uint8_t& unknown2ca9 = g_unknown2ca9[0];
	RegisterUInt8(unknown2ca9)->SetCount(sizeof(g_unknown2ca9));

	RegisterUInt32(flags);
	RegisterUInt32(flags2);
	static std::string afk_message = "afk_message";
	RegisterCharString(afk_message, 256);
	RegisterSubstruct(pve_props);
	RegisterSubstruct(unk_props);
	RegisterSubstruct(unk_props2);
	RegisterSubstruct(pvp_props);
	RegisterSubstruct(ts_props);
	RegisterSubstruct(prop_caps);

	static uint8_t g_unknown37cd[0x37fd - 0x37cd];
	uint8_t& unknown37cd = g_unknown37cd[0];
	RegisterUInt8(unknown37cd)->SetCount(sizeof(g_unknown37cd));

	RegisterUInt64(spell_state_flags);

	static uint8_t g_unknown3805[0x380f - 0x3805];
	uint8_t& unknown3805 = g_unknown3805[0];
	RegisterUInt8(unknown3805)->SetCount(sizeof(g_unknown3805));

	RegisterSubstruct(groupSheet);
	RegisterFloat(humidity);
	RegisterFloat(wind_direction);
	RegisterUInt64(statusPoints);
	RegisterUInt64(guild_status);

	RegisterUInt8(unknownCOEa);
	//house zone and the following 151 bytes (199 total) appear to be the same object
	static std::string house_zone = "house zone";
	RegisterCharString(house_zone, 48);

	static uint8_t g_unknown3e1c[0x3eb3 - 0x3e1c];
	uint8_t& unknown3e1c = g_unknown3e1c[0];
	RegisterUInt8(unknown3e1c)->SetCount(sizeof(g_unknown3e1c));

	//bind zone and the following bytes (84 total) appear to be the same object
	static std::string bind_zone = "bind zone";
	RegisterCharString(bind_zone, 32);

	constexpr uint32_t filler_size = std::max<uint32_t>(0x3f33 - 0x3ef7, 0x3f0f - 0x3ed3);

	static uint8_t g_unknown3ed3[filler_size];
	uint8_t& unknown3ed3 = g_unknown3ed3[0];
	RegisterUInt8(unknown3ed3)->SetCount(ver >= 67730 ? (0x3f33 - 0x3ef7) : (0x3f0f - 0x3ed3));
}