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