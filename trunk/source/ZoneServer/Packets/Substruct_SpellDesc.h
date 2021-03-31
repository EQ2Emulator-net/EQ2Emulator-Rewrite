#pragma once

#include "Substruct_ExamineDescBase.h"
#include <string>
#include <vector>

struct SpellReagent {
	std::string reagent_name;
	int32_t quantity_required;
	bool bConsumed;
};

struct SpellEffect {
	std::string text;
	uint8_t sub_bullet_flag;
	uint8_t percentage;
};

struct SpellLevel {
	uint8_t adventure_class;
	uint8_t tradeskill_class;
	uint16_t spell_level;
};

struct SpellDescInfo {
	uint32_t id;
	uint16_t icon;
	uint16_t heroic_op_icon;
	uint16_t icon_backdrop;
	uint16_t spell_version;
	uint16_t spell_sub_version;
	uint8_t unknownmj3;
	int32_t unknownmj1;
	uint8_t type;
	uint32_t class_skill;
	uint32_t mastery_skill;
	int16_t min_class_skill_req;
	uint32_t spell_text_color;
	uint8_t num_levels;
	uint8_t tier;
	int64_t health_cost;
	int64_t health_upkeep;
	int64_t power_cost;
	int64_t power_upkeep;
	int16_t savagery_cost;
	int16_t savagery_upkeep;
	int16_t dissonance_cost;
	int16_t dissonance_upkeep;
	uint8_t concentration_cost;
	uint8_t unknown_rw1;
	int16_t cast_time;
	int16_t recovery;
	float recast;
	float radius;
	int16_t max_targets;
	bool bFriendly;
	uint8_t num_reagents;
	uint8_t num_effects;
	bool bDisplaySpellTier;
	uint8_t unknown1;
	float min_range;
	float max_range;
	int32_t duration1;
	int32_t duration2;
	uint8_t unknown9;
	uint8_t duration_flag;
	uint8_t target_type;
	bool bCan_effect_raid;
	bool bGroup_only;
	bool bMulti_target;
	float resistibility;
	float hit_bonus;
	uint16_t unknown_rw2;
	uint64_t unknown_rw3;
	uint8_t unknown_rw4;
	uint8_t unknown_rw5;
	uint16_t unknown_rw5_a[3];
	uint8_t unknown_rw5_b;	
	uint8_t unknown85b;
	uint8_t unknown13;

	SpellDescInfo() {
		//Zero out our primitive data
		memset(this, 0, reinterpret_cast<uint8_t*>(&spell_levels) - reinterpret_cast<uint8_t*>(this));
	}

	std::vector<SpellLevel> spell_levels;
	std::vector<SpellReagent> reagents;
	std::vector<SpellEffect> effects;
	std::string spell_name;
	std::string description;
	std::string unknown85a;
};

class Substruct_SpellLevel : public PacketSubstruct, public SpellLevel {
public:
	Substruct_SpellLevel(uint32_t ver = 0) : PacketSubstruct(ver, true) {

	}

	Substruct_SpellLevel(const SpellLevel& l, uint32_t ver = 0) : PacketSubstruct(ver), SpellLevel(l) {

	}

	~Substruct_SpellLevel() = default;

	void RegisterElements() override {
		RegisterUInt8(adventure_class);
		RegisterUInt8(tradeskill_class);
		RegisterUInt16(spell_level);
	}
};

class Substruct_SpellReagent : public PacketSubstruct, public SpellReagent {
public:
	Substruct_SpellReagent(uint32_t ver = 0) : PacketSubstruct(ver, true) {

	}

	Substruct_SpellReagent(const SpellReagent& r, uint32_t ver = 0) : PacketSubstruct(ver), SpellReagent(r) {

	}

	~Substruct_SpellReagent() = default;

	void RegisterElements() override {
		Register8String(reagent_name);
		RegisterBool(bConsumed);
		RegisterInt32(quantity_required);
	}
};

class Substruct_SpellEffect : public PacketSubstruct, public SpellEffect {
public:
	Substruct_SpellEffect(uint32_t ver = 0) : PacketSubstruct(ver, true) {

	}

	Substruct_SpellEffect(const SpellEffect& se, uint32_t ver = 0) : PacketSubstruct(ver), SpellEffect(se) {

	}

	~Substruct_SpellEffect() = default;

	void RegisterElements() override {
		RegisterUInt8(sub_bullet_flag);
		Register16String(text);
		RegisterUInt8(percentage);
	}
};

class Substruct_SpellDesc : public PacketSubstruct, public SpellDescInfo {
public:
	Substruct_SpellDesc(uint32_t ver = 0, uint8_t p_descVer = 0) : PacketSubstruct(ver), descVer(p_descVer),
	health_cost_do_not_set(0), health_upkeep_do_not_set(0), power_cost_do_not_set(0){
	}

	~Substruct_SpellDesc() = default;

	void PreWrite() override {
		if (GetVersion() >= 69195) {
			health_cost_int32_do_not_set = static_cast<int32_t>(health_cost);
			health_upkeep_int32_do_not_set = static_cast<int32_t>(health_upkeep);
			power_cost_int32_do_not_set = static_cast<int32_t>(power_cost);
			power_upkeep_int16_do_not_set = static_cast<int32_t>(power_upkeep);
		}
		else if (GetVersion() < 60114) {
			health_cost_do_not_set = static_cast<int16_t>(health_cost);
			health_upkeep_do_not_set = static_cast<int16_t>(health_upkeep);
			power_cost_do_not_set = static_cast<int16_t>(power_cost);
		}

		spell_levels.clear();
		for (auto& itr : SpellDescInfo::spell_levels) {
			spell_levels.emplace_back(itr, GetVersion());
		}

		reagents.clear();
		for (auto& itr : SpellDescInfo::reagents) {
			reagents.emplace_back(itr, GetVersion());
		}

		effects.clear();
		for (auto& itr : SpellDescInfo::effects) {
			effects.emplace_back(itr, GetVersion());
		}
		PacketSubstruct::PreWrite();
	}

	void PostRead() override {
		if (GetVersion() < 60114) {
			health_cost = health_cost_do_not_set;
			health_upkeep = health_upkeep_do_not_set;
			power_cost = power_cost_do_not_set;
		}
		else if (GetVersion() < 69195) {
			health_cost = health_cost_int32_do_not_set;
			health_upkeep = health_upkeep_int32_do_not_set;
			power_cost = power_cost_int32_do_not_set;
			power_upkeep = power_upkeep_int16_do_not_set;
		}

		SpellDescInfo::spell_levels.clear();
		for (auto& itr : spell_levels) {
			SpellDescInfo::spell_levels.emplace_back(static_cast<SpellLevel&>(itr));
		}

		SpellDescInfo::reagents.clear();
		for (auto& itr : reagents) {
			SpellDescInfo::reagents.emplace_back(static_cast<SpellReagent&>(itr));
		}

		SpellDescInfo::effects.clear();
		for (auto& itr : effects) {
			SpellDescInfo::effects.emplace_back(static_cast<SpellEffect&>(itr));
		}
		PacketSubstruct::PostRead();
	}

	void RegisterElements() override {
		if (descVer == 0) {
			descVer = GetDescriptionVersion(GetVersion());
		}

		RegisterUInt32(id);
		RegisterUInt16(icon);
		RegisterUInt16(heroic_op_icon);
		RegisterUInt16(icon_backdrop);
		RegisterUInt16(spell_version);
		RegisterUInt16(spell_sub_version);
		if (GetVersion() >= 60114) {
			RegisterUInt8(unknownmj3);
			RegisterInt32(unknownmj1);
		}
		RegisterUInt8(type);
		RegisterUInt32(class_skill);
		RegisterUInt32(mastery_skill);
		RegisterOversizedByte(min_class_skill_req);
		RegisterUInt32(spell_text_color);

		auto e = RegisterUInt8(num_levels);
		e->SetMyArray(RegisterArray(spell_levels, Substruct_SpellLevel));
		RegisterUInt8(tier);

		if (GetVersion() < 60114) {
			int16_t& health_cost = health_cost_do_not_set;
			int16_t& health_upkeep = health_upkeep_do_not_set;
			int16_t& power_cost = power_cost_do_not_set;
			int16_t& power_upkeep = power_upkeep_int16_do_not_set;

			RegisterOversizedByte(health_cost);
			RegisterOversizedByte(health_upkeep);
			RegisterOversizedByte(power_cost);
			RegisterOversizedByte(power_upkeep);
		}
		else if (GetVersion() < 69195) {
			int32_t& health_cost = health_cost_int32_do_not_set;
			int32_t& health_upkeep = health_upkeep_int32_do_not_set;
			int32_t& power_cost = power_cost_int32_do_not_set;
			int16_t& power_upkeep = power_upkeep_int16_do_not_set;

			RegisterInt32(health_cost);
			RegisterInt32(health_upkeep);
			RegisterInt32(power_cost);
			RegisterOversizedByte(power_upkeep);
		}
		else {
			RegisterInt64(health_cost);
			RegisterInt64(health_upkeep);
			RegisterInt64(power_cost);
			RegisterInt64(power_upkeep);
		}
		
		RegisterOversizedByte(savagery_cost);
		RegisterOversizedByte(savagery_upkeep);
		if (GetVersion() >= 60114) {
			RegisterOversizedByte(dissonance_cost);
			RegisterOversizedByte(dissonance_upkeep);
		}
		RegisterUInt8(concentration_cost);
		RegisterUInt8(unknown_rw1);
		RegisterOversizedByte(cast_time);
		RegisterOversizedByte(recovery);
		RegisterFloat(recast);
		RegisterFloat(radius);
		RegisterInt16(max_targets);
		RegisterBool(bFriendly);
		e = RegisterUInt8(num_reagents);
		e->SetMyArray(RegisterArray(reagents, Substruct_SpellReagent));
		e = RegisterUInt8(num_effects);
		e->SetMyArray(RegisterArray(effects, Substruct_SpellEffect));
		RegisterBool(bDisplaySpellTier);
		RegisterUInt8(unknown1);
		RegisterFloat(min_range);
		RegisterFloat(max_range);
		RegisterInt32(duration1);
		RegisterInt32(duration2);
		RegisterUInt8(unknown9);
		RegisterUInt8(duration_flag);
		RegisterUInt8(target_type);
		RegisterBool(bCan_effect_raid);
		RegisterBool(bGroup_only);
		RegisterBool(bMulti_target);
		RegisterFloat(resistibility);
		RegisterFloat(hit_bonus);
		RegisterUInt16(unknown_rw2);
		if (descVer >= 13) {
			//bitflag?
			if (GetVersion() < 60114) {
				RescopeToReference(unknown_rw3, uint32_t);
				RegisterUInt32(unknown_rw3);
			}
			else {
				RegisterUInt64(unknown_rw3);
			}
		}
		RegisterUInt8(unknown_rw4);
		if (descVer >= 58) {
			e = RegisterUInt8(unknown_rw5);
			RescopeArrayElement(unknown_rw5_a);
			auto e2 = RegisterUInt16(unknown_rw5_a);
			e2->SetCount(3);
			e2->SetIsVariableSet(e);
			RegisterUInt8(unknown_rw5_b)->SetIsVariableSet(e);
		}
		if (descVer >= 85) {
			Register16String(unknown85a);
			RegisterUInt8(unknown85b);
		}
		Register8String(spell_name);
		Register16String(description);
		RegisterUInt8(unknown13);
	}

	uint8_t descVer;

private:
	//These changed from oversized to int32
	int16_t health_cost_do_not_set;
	int16_t health_upkeep_do_not_set;
	int16_t power_cost_do_not_set;
	
	//Now these went from int32 to int64 as well as power_upkeep
	int32_t health_cost_int32_do_not_set;
	int32_t health_upkeep_int32_do_not_set;
	int32_t power_cost_int32_do_not_set;
	int16_t power_upkeep_int16_do_not_set;

	std::vector<Substruct_SpellLevel> spell_levels;
	std::vector<Substruct_SpellReagent> reagents;
	std::vector<Substruct_SpellEffect> effects;
};