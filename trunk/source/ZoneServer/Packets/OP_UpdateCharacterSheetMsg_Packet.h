#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"

#include "Substruct_SpellEffects.h"
#include "Substruct_PassiveEffects.h"
#include "Substruct_MaintainedEffects.h"
#include "Substruct_GroupMember.h"
#include "../Players/CharacterSheet.h"

struct SpellProps {
	//These are the blue stats on items and also appear in the character sheet, a good way to find the switch in the client is to search for
	//Blurs Vision ^ they appear in the same order as their id/index for that client

	float hp_regen;
	float power_regen;
	float ooc_hp_regen;
	float ooc_power_regen;
	float ic_hp_regen_bonus;
	float ic_power_regen_bonus;
	float hp_max;
	float hp_max_sta_bonus;
	float hp_max_perc_total;
	float ooc_run_speed;
	float slow;
	float speed_mount_ground;
	float speed_mount_air;
	float speed_mount_leap;
	float mount_leap_distance;
	float glide_efficiency;
	float ic_run_speed;
	float haste;
	float haste_spell;
	float power_max;
	float power_max_perc;
	float all_attributes;
	float blur_vision;
	float magic_level_immunity;
	float hate_gain;
	float combat_xp_mod;
	float ts_xp_mod;
	float aa_xp_mod;
	float status_earned_mod;
	float size_mod;
	float dps;
	float dps_spell;
	float stealth;
	float invis;
	float see_stealth;
	float see_invis;
	float effective_level_mod;
	float riposte_chance;
	float parry_chance;
	float dodge_chance;
	float ae_autoattack_chance;
	float ae_autoattack_chance_spell;
	float flurry_chance;
	float flurry_chance_spell;
	float multi_attack_chance;
	float multi_attack_chance_pvp;
	float multi_attack_chance_spell;
	float multi_attack_chance_spell_pvp;
	float doublecast_chance_spell;
	float doublecast_chance_ability;
	float doublecast_chance_spell_pvp;
	float melee_damage_multiplier;
	float bountiful_harvest;
	float block_chance_shield;
	float hp_regen_bonus;
	float power_regen_bonus;
	float crit_chance;
	float crit_avoidance;
	float crit_chance_beneficial;
	float crit_bonus;
	float crit_bonus_pvp;
	float hp_unconcious;
	float ability_reuse_speed;
	float ability_recovery_speed;
	float ability_casting_speed;
	float spell_reuse_speed;
	float melee_weapon_range;
	float ranged_weapon_range;
	float falling_damage_reduction;
	float riposte_damage;
	float block_chance_minimum;
	float movement_weave;
	float ic_hp_regen;
	float ic_power_regen;
	float speed_contest_only;
	float tracking_avoidance;
	float speed_stealth_invis;
	float loot_coin_mod;
	float mitigation_increase;
	float ammo_conservation_chance;
	float strikethrough;
	float status_bonus;
	float accuracy;
	float counter_strike;
	float shield_bash;
	float weapon_damage_bonus;
	float weapon_damage_bonus_spell;
	float weapon_damage_bonus_melee;
	float riposte_chance_additional;
	float crit_mit;
	float potency;
	float potency_pvp;
	float toughness;
	float lethality;
	float stamina_bonus;
	float wis_mit_bonus;
	float heals_received;
	float heals_received_pct;
	float power_received;
	float crit_mit_pvp;
	float avoidance_base_bonus;
	float ic_savagery_regen;
	float ooc_savagery_regen;
	float savagery_regen;
	float savagery_gain_mod;
	float savagery_level_max;
	float ic_dissonance_regen;
	float ooc_dissonance_regen;
	float dissonance_regen;
	float dissonance_gain_mod;
	float ae_autoattack_avoid;
	float agnostic_damage_bonus;
	float agnostic_heal_bonus;
	float tithe_gain;
	float fervor;
	float resolve;
	float combat_mit;
	float ability_mit;
	float multi_attack_avoidance;
	float doublecast_spell_avoidance;
	float doublecast_ability_avoidance;
	float dps_mit;
	float fervor_mit;
	float flurry_avoidance;
	float weapon_damage_bonus_mit;
	float weapon_damage_bonus_melee_mit;
	float ability_mod_mit;
	float melee_damage_multiplier_mit;
	float heals_received_pot_mit;
	float heals_received_critbonus_mit;
	float heals_received_fervor_mit;
	float heals_received_normalized_mit;

	SpellProps() {
		memset(this, 0, sizeof(*this));
	}
};

class Substruct_SpellProps : public PacketSubstruct, public SpellProps {
public:
	Substruct_SpellProps(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_SpellProps() = default;

	void RegisterElements() override {
		const uint32_t ver = GetVersion();

		RegisterFloat(hp_regen);
		RegisterFloat(power_regen);
		RegisterFloat(ooc_hp_regen);
		RegisterFloat(ooc_power_regen);
		RegisterFloat(ic_hp_regen_bonus);
		RegisterFloat(ic_power_regen_bonus);
		RegisterFloat(hp_max);
		RegisterFloat(hp_max_sta_bonus);
		RegisterFloat(hp_max_perc_total);
		RegisterFloat(ooc_run_speed);
		RegisterFloat(slow);
		RegisterFloat(speed_mount_ground);
		RegisterFloat(speed_mount_air);
		RegisterFloat(speed_mount_leap);
		RegisterFloat(mount_leap_distance);
		RegisterFloat(glide_efficiency);
		RegisterFloat(ic_run_speed);
		RegisterFloat(haste);
		if (ver < 60114) {
			RegisterFloat(haste_spell);
		}
		RegisterFloat(power_max);
		RegisterFloat(power_max_perc);
		RegisterFloat(all_attributes);
		RegisterFloat(blur_vision);
		RegisterFloat(magic_level_immunity);
		RegisterFloat(hate_gain);
		RegisterFloat(combat_xp_mod);
		RegisterFloat(ts_xp_mod);
		RegisterFloat(aa_xp_mod);
		if (ver >= 60114) {
			RegisterFloat(status_earned_mod);
		}
		RegisterFloat(size_mod);
		RegisterFloat(dps);
		if (ver < 60114) {
			RegisterFloat(dps_spell);
		}
		RegisterFloat(stealth);
		RegisterFloat(invis);
		RegisterFloat(see_stealth);
		RegisterFloat(see_invis);
		RegisterFloat(effective_level_mod);
		RegisterFloat(riposte_chance);
		RegisterFloat(parry_chance);
		RegisterFloat(dodge_chance);
		RegisterFloat(ae_autoattack_chance);
		if (ver < 60114) {
			RegisterFloat(ae_autoattack_chance_spell);
		}
		RegisterFloat(multi_attack_chance);
		if (ver < 60114) {
			RegisterFloat(multi_attack_chance_pvp);
			RegisterFloat(multi_attack_chance_spell);
			RegisterFloat(multi_attack_chance_spell_pvp);
		}
		RegisterFloat(doublecast_chance_spell);
		if (ver < 60114) {
			RegisterFloat(doublecast_chance_spell_pvp);
		}
		else if (ver >= 67650) {
			RegisterFloat(doublecast_chance_ability);
		}
		RegisterFloat(flurry_chance);
		if (ver < 60114) {
			RegisterFloat(flurry_chance_spell);
		}
		RegisterFloat(melee_damage_multiplier);
		RegisterFloat(bountiful_harvest);
		RegisterFloat(block_chance_shield);
		RegisterFloat(hp_regen_bonus);
		RegisterFloat(power_regen_bonus);
		RegisterFloat(crit_chance);
		RegisterFloat(crit_avoidance);
		RegisterFloat(crit_chance_beneficial);
		RegisterFloat(crit_bonus);
		if (ver < 60114) {
			RegisterFloat(crit_bonus_pvp);
		}
		RegisterFloat(potency);
		if (ver < 60114) {
			RegisterFloat(potency_pvp);
		}
		RegisterFloat(hp_unconcious);
		RegisterFloat(ability_reuse_speed);
		RegisterFloat(ability_recovery_speed);
		RegisterFloat(ability_casting_speed);
		RegisterFloat(spell_reuse_speed);
		RegisterFloat(melee_weapon_range);
		RegisterFloat(ranged_weapon_range);
		RegisterFloat(falling_damage_reduction);
		RegisterFloat(riposte_damage);
		RegisterFloat(block_chance_minimum);
		RegisterFloat(movement_weave);
		RegisterFloat(ic_hp_regen);
		RegisterFloat(ic_power_regen);
		RegisterFloat(speed_contest_only);
		RegisterFloat(tracking_avoidance);
		RegisterFloat(speed_stealth_invis);
		RegisterFloat(loot_coin_mod);
		RegisterFloat(mitigation_increase);
		RegisterFloat(ammo_conservation_chance);
		RegisterFloat(strikethrough);
		RegisterFloat(status_bonus);
		RegisterFloat(accuracy);
		RegisterFloat(counter_strike);
		RegisterFloat(shield_bash);
		if (ver < 67650) {
			RegisterFloat(weapon_damage_bonus);
		}
		if (ver >= 60114) {
			RegisterFloat(weapon_damage_bonus_spell);
		}
		RegisterFloat(weapon_damage_bonus_melee);
		RegisterFloat(riposte_chance_additional);
		RegisterFloat(crit_mit);
		RegisterFloat(toughness);
		RegisterFloat(lethality);
		RegisterFloat(stamina_bonus);
		RegisterFloat(wis_mit_bonus);
		RegisterFloat(heals_received);
		RegisterFloat(heals_received_pct);
		if (ver >= 67650) {
			RegisterFloat(power_received);
		}
		RegisterFloat(crit_mit_pvp);
		RegisterFloat(avoidance_base_bonus);
		RegisterFloat(ic_savagery_regen);
		RegisterFloat(ooc_savagery_regen);
		RegisterFloat(savagery_regen);
		RegisterFloat(savagery_gain_mod);
		RegisterFloat(savagery_level_max);
		if (ver >= 60114) {
			RegisterFloat(ic_dissonance_regen);
			RegisterFloat(ooc_dissonance_regen);
			RegisterFloat(dissonance_regen);
			RegisterFloat(dissonance_gain_mod);
			RegisterFloat(ae_autoattack_avoid);
			RegisterFloat(agnostic_damage_bonus);
			RegisterFloat(agnostic_heal_bonus);
			RegisterFloat(tithe_gain);
			if (ver >= 67650) {
				RegisterFloat(fervor);
				RegisterFloat(resolve);
				RegisterFloat(combat_mit);
				RegisterFloat(ability_mit);
				RegisterFloat(multi_attack_avoidance);
				RegisterFloat(doublecast_spell_avoidance);
				RegisterFloat(doublecast_ability_avoidance);
				RegisterFloat(dps_mit);
				RegisterFloat(fervor_mit);
				RegisterFloat(flurry_avoidance);
				RegisterFloat(weapon_damage_bonus_mit);
				RegisterFloat(weapon_damage_bonus_melee_mit);
				RegisterFloat(ability_mod_mit);
				RegisterFloat(melee_damage_multiplier_mit);
				RegisterFloat(heals_received_pot_mit);
				RegisterFloat(heals_received_critbonus_mit);
				RegisterFloat(heals_received_fervor_mit);
				RegisterFloat(heals_received_normalized_mit);
			}
		}
	}
};

//This is all the primitive data which can be zeroed
//Need to make this crap less ridiculous at some point
struct CharacterSheetMiscData {
	CharacterSheetMiscData() {
		memset(this, 0, sizeof(*this));
		breath = 30.f;
	}

	//Some elements are a different format in the packet from what we use, convert them here
	int64_t hp;
	int64_t maxHp;
	float advExp;
	float advExpNextLevel;
	float advExpDebt;
	float advVitality;
	float tsExp;
	float tsExpNextLevel;
	float tsExpDebt;
	float tsVitality;

	int32_t gmLevel;
	uint16_t account_age_base;
	uint16_t account_age_bonus;
	
	int32_t base_savagery;
	int32_t savagery_level;
	int32_t max_savagery_level;
	int32_t base_savagery_level;
	int32_t dissonance;
	int32_t max_dissonance;
	int32_t base_dissonance;
	uint32_t hp_regen;
	uint32_t power_regen;
	uint32_t unknown6[2];
	float ranged_attack_min_distance;
	float ranged_attack_max_distance;
	float stat_bonus_health;
	float stat_bonus_power;
	uint32_t bonus_health;
	uint32_t unknown8;
	uint32_t bonus_power;
	float stat_bonus_damage;
	uint16_t mitigation_pct_pve;
	uint16_t mitigation_pct_pvp;
	uint16_t toughness;
	float toughness_resist_dmg_pvp;
	uint16_t lethality;
	float lethality_pct;
	uint16_t avoidance_pct;
	uint16_t avoidance_reduction;
	uint16_t avoidance;
	uint16_t avoidance_max;
	uint16_t avoidance_base;
	uint16_t unknown10a;
	uint16_t parry;
	uint16_t unknown11;
	uint16_t block;
	uint16_t unknown12;
	uint16_t uncontested_block;
	uint16_t unknown13;
	uint16_t uncontested_riposte;
	uint16_t uncontested_dodge;
	uint16_t uncontested_parry;
	uint16_t unknown14;
	uint16_t elemental_absorb_pve;
	uint16_t noxious_absorb_pve;
	uint16_t arcane_absorb_pve;
	uint16_t unknown15;
	uint16_t elemental_absorb_pvp;
	uint16_t noxious_absorb_pvp;
	uint16_t arcane_absorb_pvp;
	uint16_t unknown16;
	uint16_t elemental_dmg_reduction;
	uint16_t noxious_dmg_reduction;
	uint16_t arcane_dmg_reduction;
	uint16_t unknown17;
	uint16_t elemental_dmg_reduction_pct;
	uint16_t noxious_dmg_reduction_pct;
	uint16_t arcane_dmg_reduction_pct;
	uint16_t unknown18[6];
	uint16_t server_bonus;
	uint16_t adventure_vet_bonus;
	uint16_t tradeskill_vet_bonus;
	uint16_t dungeon_finder_bonus;
	uint32_t recruit_friend_bonus;
	uint16_t unknown19;
	uint16_t adventure_vitality;
	uint16_t adventure_vitality_yellow_arrow;
	uint16_t adventure_vitality_blue_arrow;
	uint16_t tradeskill_vitality;
	uint16_t tradeskill_vitality_purple_arrow;
	uint16_t tradeskill_vitality_blue_arrow;
	uint16_t mentor_bonus;
	uint8_t unknown20;
	uint16_t assigned_aa;
	uint16_t max_aa;
	uint16_t unassigned_aa;
	uint16_t aa_green_bar;
	uint16_t adv_xp_to_aa_xp_slider;
	uint16_t unknown21;
	uint16_t aa_blue_bar;
	uint16_t bonus_achievement_xp;
	uint8_t unknown22[2];
	uint8_t unknown23[2];
	uint32_t items_found;
	uint32_t named_npcs_killed;
	uint32_t quests_completed;
	uint32_t exploration_events;
	uint32_t completed_collections;
	uint16_t unknown24[10];
	uint8_t unknown25;
	uint16_t total_prestige_points;
	uint16_t unassigned_prestige_points;
	uint16_t unknown26;
	uint16_t unknown27;
	uint16_t total_tradeskill_points;
	uint16_t unassigned_tradeskill_points;
	uint16_t unknown28;
	uint16_t unknown29;
	uint16_t total_tradeskill_prestige_points;
	uint16_t unassigned_tradeskill_prestige_points;
	uint16_t unknown30;
	uint16_t unknown31;
	uint16_t unknown32;
	uint16_t unknown33;
	uint32_t unknown34[5];
	
	bool passive_unk_bool[128];
	int32_t passive_last_index;

	uint8_t unknown35[354];
	uint8_t trauma_count;
	uint8_t arcane_count;
	uint8_t noxious_count;
	uint8_t elemental_count;
	uint8_t curse_count;
	
	float breath;
	uint32_t breathable_environment_flags;
	uint8_t unknown36;
	uint32_t melee_pri_dmg_min;
	uint32_t melee_pri_dmg_max;
	float melee_pri_delay;
	uint32_t melee_sec_dmg_min;
	uint32_t melee_sec_dmg_max;
	float melee_sec_delay;
	uint32_t ranged_dmg_min;
	uint32_t ranged_dmg_max;
	float ranged_delay;
	uint32_t unknown37;
	uint32_t unknown38;
	uint32_t unknown39;
	uint32_t unknown40;
	float ability_mod_pve;
	float unknown41;
	float unknown42;
	float unknown43;
	float unknown44;
	float ability_mod_pvp;
	float base_melee_crit;
	float base_spell_crit;
	float base_taunt_crit;
	float base_heal_crit;
	float durability_mod;
	float durability_add;
	float progress_mod;
	float progress_add;
	float success_mod;
	float crit_success_mod;
	float crit_failure_mod;
	float rare_harvest_chance;
	float max_crafting;
	float component_refund;
	float ex_attempts;
	float refine_quantity_mod;
	float ex_durability_mod;
	float ex_durability_add;
	float ex_crit_success_mod;
	float ex_crit_failure_mod;
	float ex_progress_mod;
	float ex_progress_add;
	float ex_success_mod;
	float unknown430;
	float unknown431;
	float unknown432;
	float unknown433;
	float unknown434;
	float unknown435;
	float unknown436;
	float unknown437;
	float unknown438;
	float unknown439;
	float unknown440;
	float unknown441;
	uint8_t unknown180;
	uint8_t unknown524;
	uint8_t unknown181a;
	uint8_t unknown181b;
	float unknown442;
	float unknown443;
	float unknown444;
	float unknown445;
	uint32_t hate_2;
	float unknown447;
	float unknown448;
	float unknown449;
	float unknown450;
	float unknown451;
	uint32_t hate_mod_2;
	float unknown453;
	float unknown454;
	float unknown455;
	float unknown456;
	float unknown457;
	uint32_t dps_2;
	float unknown459;
	float unknown460;
	float unknown461;
	float unknown462;
	float unknown463;
	float unknown464;
	float unknown465;
	float unknown466;
	uint32_t ae_autoattack_2;
	float unknown468;
	uint32_t spell_doublecast_2;
	uint32_t flurry_2;
	float unknown471;
	uint32_t bountiful_harvest_2;
	uint32_t block_chance_2;
	float unknown474;
	float unknown475;
	uint32_t crit_chance_2;
	float unknown477;
	float unknown478;
	uint32_t crit_bonus_2;
	float unknown480;
	float unknown481;
	uint32_t reuse_speed_2;
	uint32_t recovery_speed_2;
	uint32_t casting_speed_2;
	uint32_t spell_reuse_speed_2;
	float unknown486;
	float unknown487;
	float unknown488;
	float unknown489;
	float unknown490;
	float unknown491;
	float unknown492;
	float unknown493;
	float unknown494;
	float unknown495;
	float unknown496;
	float unknown497;
	float unknown498;
	float unknown499;
	uint32_t strikethrough_2;
	float unknown501;
	uint32_t accuracy_2;
	float unknown503;
	float unknown504;
	uint32_t wdb_2;
	uint32_t spell_wdb_2;
	float unknown507;
	float unknown508;
	float unknown509;
	float unknown510;
	float unknown511;
	float unknown512;
	float unknown513;
	float unknown514;
	uint32_t haste_tt;
	uint32_t dps_pve_tt;
	uint32_t dps_pvp_tt;
	float multi_attack_pve_tt;
	float multi_attack_pvp_tt;
	float unknown520;
	float unknown521;
	float unknown522;
	float unknown523;
	uint8_t unknown550;
	uint8_t unknown552[27];
	float adventure_effects_cap;
	float tradeskill_effects_cap;
	float aa_effects_cap;
	uint8_t unknown525a[36];
	//Known useful spell state values:
	//1<<1 = charm (prevents things like sitting and changing targets while charmed)
	//1<<7 = purple (ultravision)
	//1<<8 = heat (infravision)
	//1<<9 = piss (sonicvision)
	//1<<10 = grey
	//1<<11 = green
	//1<<12 = red
	//1<<13 = fish vision
	//a bunch of others but not sure that anything else actually "does" anything for the client
	uint64_t spell_state_flags;
	uint8_t unknown525b[6];
	float humidity;
	float wind_direction;
	uint8_t unknown527[4];
	uint64_t guild_status;
	uint8_t unknownAOMa;
	uint8_t unknown526[151];
	uint8_t unknown188[52];
};

class UpdateCharacterSheetMsgData : public CharacterSheetMiscData, public CharacterSheet, public PacketEncodedData {
public:
	UpdateCharacterSheetMsgData(uint32_t ver) : PacketEncodedData(ver), CharacterSheet(nullptr), groupSheet(ver) {
		for (uint8_t i = 0; i < 45; i++)
			spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 45; i++)
			detrimental_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 100; i++)
			passive_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 30; i++)
			maintained_effects[i].ResetVersion(version);
		RegisterElements();
	}

	UpdateCharacterSheetMsgData(uint32_t version, const CharacterSheet& sheet) : PacketEncodedData(version), CharacterSheet(sheet),
		groupSheet(version), pve_props(version), pvp_props(version), unk_props(version), unk_props2(version) {
		for (uint8_t i = 0; i < 45; i++)
			spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 45; i++)
			detrimental_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 100; i++)
			passive_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 30; i++)
			maintained_effects[i].ResetVersion(version);
		RegisterElements();
	}

	void PreWrite() override;

	Substruct_SpellEffects spell_effects[45];
	Substruct_SpellEffects detrimental_spell_effects[45];
	Substruct_PassiveEffects passive_spell_effects[100];
	Substruct_MaintainedEffects maintained_effects[30];
	Substruct_GroupSheet groupSheet;

	Substruct_SpellProps pve_props;
	Substruct_SpellProps unk_props;
	Substruct_SpellProps pvp_props;
	Substruct_SpellProps unk_props2;

private:
	double advExp_do_not_set;
	double advExpNextLevel_do_not_set;
	double tsExp_do_not_set;
	double tsExpNextLevel_do_not_set;

	void RegisterElements() {
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
		RegisterUInt16(account_age_bonus);

		//Replace deity/fb/twitter later
		static std::string deity = "deity";
		RegisterCharString(deity, 32);
		std::string& last_name = namingInfo->last_name;
		RegisterCharString(last_name, 21);
		std::string& twitter_prepend = namingInfo->name;
		RegisterCharString(twitter_prepend, 42);
		std::string& facebook_prepend = namingInfo->name;
		RegisterCharString(facebook_prepend, 42);


		RegisterInt64(hp);
		RegisterInt64(maxHp);
		int32_t& baseHp = attributes->hp.baseValue;
		RegisterInt32(baseHp);
		static int32_t unconciousHp = 0;
		RegisterInt32(unconciousHp);

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
		
		RegisterUInt32(hp_regen);
		RegisterUInt32(power_regen);

		uint32_t& Unknown6 = unknown6[0];
		RegisterUInt32(Unknown6)->SetCount(2);
		
		RegisterFloat(ranged_attack_min_distance);
		RegisterFloat(ranged_attack_max_distance);
		RegisterFloat(stat_bonus_health);
		RegisterFloat(stat_bonus_power);
		RegisterUInt32(bonus_health);
		RegisterUInt32(unknown8);
		RegisterUInt32(bonus_power);
		RegisterFloat(stat_bonus_damage);
		RegisterUInt16(mitigation_pct_pve);
		RegisterUInt16(mitigation_pct_pvp);
		RegisterUInt16(toughness);
		RegisterFloat(toughness_resist_dmg_pvp);
		RegisterUInt16(lethality);
		RegisterFloat(lethality_pct);
		RegisterUInt16(avoidance_pct);
		RegisterUInt16(avoidance_reduction);
		RegisterUInt16(avoidance);
		RegisterUInt16(avoidance_max);
		RegisterUInt16(avoidance_base);
		RegisterUInt16(unknown10a);
		RegisterUInt16(parry);
		RegisterUInt16(unknown11);
		RegisterUInt16(block);
		RegisterUInt16(unknown12);
		RegisterUInt16(uncontested_block);
		RegisterUInt16(unknown13);
		RegisterUInt16(uncontested_riposte);
		RegisterUInt16(uncontested_dodge);
		RegisterUInt16(uncontested_parry);
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

		RegisterUInt16(unknown14);
		RegisterUInt16(elemental_absorb_pve);
		RegisterUInt16(noxious_absorb_pve);
		RegisterUInt16(arcane_absorb_pve);
		RegisterUInt16(unknown15);
		RegisterUInt16(elemental_absorb_pvp);
		RegisterUInt16(noxious_absorb_pvp);
		RegisterUInt16(arcane_absorb_pvp);
		RegisterUInt16(unknown16);
		RegisterUInt16(elemental_dmg_reduction);
		RegisterUInt16(noxious_dmg_reduction);
		RegisterUInt16(arcane_dmg_reduction);
		RegisterUInt16(unknown17);
		RegisterUInt16(elemental_dmg_reduction_pct);
		RegisterUInt16(noxious_dmg_reduction_pct);
		RegisterUInt16(arcane_dmg_reduction_pct);
		
		RegisterFloat(advExp);
		RegisterFloat(advExpNextLevel);
		RegisterFloat(advExpDebt);
		RegisterFloat(tsExp);
		RegisterFloat(tsExpNextLevel);
		RegisterFloat(tsExpDebt);

		if (GetVersion() < 60114) {
			RescopeArrayElement(unknown18);
			RegisterUInt16(unknown18)->SetCount(6);
		}
		
		RegisterUInt16(server_bonus);
		RegisterUInt16(adventure_vet_bonus);
		RegisterUInt16(tradeskill_vet_bonus);
		RegisterUInt16(dungeon_finder_bonus);
		RegisterUInt32(recruit_friend_bonus);
		RegisterUInt16(unknown19);
		RegisterUInt16(adventure_vitality);
		RegisterUInt16(adventure_vitality_yellow_arrow);
		RegisterUInt16(adventure_vitality_blue_arrow);
		RegisterUInt16(tradeskill_vitality);
		RegisterUInt16(tradeskill_vitality_purple_arrow);
		RegisterUInt16(tradeskill_vitality_blue_arrow);
		RegisterUInt16(mentor_bonus);
		RegisterUInt8(unknown20);
		RegisterUInt16(assigned_aa);
		RegisterUInt16(max_aa);
		RegisterUInt16(unassigned_aa);
		RegisterUInt16(aa_green_bar);
		RegisterUInt16(adv_xp_to_aa_xp_slider);
		RegisterUInt16(unknown21);
		RegisterUInt16(aa_blue_bar);
		RegisterUInt16(bonus_achievement_xp);

		uint8_t& Unknown22 = unknown22[0];
		RegisterUInt8(Unknown22)->SetCount(2);
		
		uint8_t& Unknown23 = unknown23[0];
		RegisterUInt8(Unknown23)->SetCount(2);
		
		RegisterUInt32(items_found);
		RegisterUInt32(named_npcs_killed);
		RegisterUInt32(quests_completed);
		RegisterUInt32(exploration_events);
		RegisterUInt32(completed_collections);

		uint16_t& Unknown24 = unknown24[0];
		RegisterUInt16(Unknown24)->SetCount(10);
		
		RegisterUInt8(unknown25);
		RegisterUInt16(total_prestige_points);
		RegisterUInt16(unassigned_prestige_points);
		RegisterUInt16(unknown26);
		RegisterUInt16(unknown27);
		RegisterUInt16(total_tradeskill_points);
		RegisterUInt16(unassigned_tradeskill_points);
		RegisterUInt16(unknown28);
		RegisterUInt16(unknown29);
		RegisterUInt16(total_tradeskill_prestige_points);
		RegisterUInt16(unassigned_tradeskill_prestige_points);
		RegisterUInt16(unknown30);
		RegisterUInt16(unknown31);
		RegisterUInt16(unknown32);
		RegisterUInt16(unknown33);

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

		uint8_t& Unknown35 = unknown35[0];
		RegisterUInt8(Unknown35)->SetCount(354);

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
		RegisterSubstruct(unk_props);
		RegisterSubstruct(pvp_props);

		//Tradeskill sheet
		RegisterFloat(durability_mod);
		RegisterFloat(durability_add);
		RegisterFloat(progress_mod);
		RegisterFloat(progress_add);
		RegisterFloat(success_mod);
		RegisterFloat(crit_success_mod);
		RegisterFloat(crit_failure_mod);
		RegisterFloat(rare_harvest_chance);
		RegisterFloat(max_crafting);
		RegisterFloat(component_refund);
		RegisterFloat(ex_attempts);
		RegisterFloat(refine_quantity_mod);
		RegisterFloat(ex_durability_mod);
		RegisterFloat(ex_durability_add);
		RegisterFloat(ex_crit_success_mod);
		RegisterFloat(ex_crit_failure_mod);
		RegisterFloat(ex_progress_mod);
		RegisterFloat(ex_progress_add);
		RegisterFloat(ex_success_mod);

		RegisterFloat(unknown430);
		RegisterFloat(unknown431);
		RegisterFloat(unknown432);
		RegisterFloat(unknown433);
		RegisterFloat(unknown434);
		RegisterFloat(unknown435);
		RegisterFloat(unknown436);
		RegisterFloat(unknown437);
		RegisterFloat(unknown438);
		RegisterFloat(unknown439);
		RegisterFloat(unknown440);
		RegisterFloat(unknown441);
		RegisterUInt8(unknown180);
		RegisterUInt8(unknown524);
		RegisterUInt8(unknown181a);
		RegisterUInt8(unknown181b);
		RegisterFloat(unknown442);
		RegisterFloat(unknown443);
		RegisterFloat(unknown444);
		RegisterFloat(unknown445);
		RegisterUInt32(hate_2);
		RegisterFloat(unknown447);
		RegisterFloat(unknown448);
		RegisterFloat(unknown449);
		RegisterFloat(unknown450);
		RegisterFloat(unknown451);
		RegisterUInt32(hate_mod_2);
		RegisterFloat(unknown453);
		RegisterFloat(unknown454);
		RegisterFloat(unknown455);
		RegisterFloat(unknown456);
		RegisterFloat(unknown457);
		RegisterUInt32(dps_2);
		RegisterFloat(unknown459);
		RegisterFloat(unknown460);
		RegisterFloat(unknown461);
		RegisterFloat(unknown462);
		RegisterFloat(unknown463);
		RegisterFloat(unknown464);
		RegisterFloat(unknown465);
		RegisterFloat(unknown466);
		RegisterUInt32(ae_autoattack_2);
		RegisterFloat(unknown468);
		RegisterUInt32(spell_doublecast_2);
		RegisterUInt32(flurry_2);
		RegisterFloat(unknown471);
		RegisterUInt32(bountiful_harvest_2);
		RegisterUInt32(block_chance_2);
		RegisterFloat(unknown474);
		RegisterFloat(unknown475);
		RegisterUInt32(crit_chance_2);
		RegisterFloat(unknown477);
		RegisterFloat(unknown478);
		RegisterUInt32(crit_bonus_2);
		RegisterFloat(unknown480);
		RegisterFloat(unknown481);
		RegisterUInt32(reuse_speed_2);
		RegisterUInt32(recovery_speed_2);
		RegisterUInt32(casting_speed_2);
		RegisterUInt32(spell_reuse_speed_2);
		RegisterFloat(unknown486);
		RegisterFloat(unknown487);
		RegisterFloat(unknown488);
		RegisterFloat(unknown489);
		RegisterFloat(unknown490);
		RegisterFloat(unknown491);
		RegisterFloat(unknown492);
		RegisterFloat(unknown493);
		RegisterFloat(unknown494);
		RegisterFloat(unknown495);
		RegisterFloat(unknown496);
		RegisterFloat(unknown497);
		RegisterFloat(unknown498);
		RegisterFloat(unknown499);
		RegisterUInt32(strikethrough_2);
		RegisterFloat(unknown501);
		RegisterUInt32(accuracy_2);
		RegisterFloat(unknown503);
		RegisterFloat(unknown504);
		RegisterUInt32(wdb_2);
		RegisterUInt32(spell_wdb_2);
		RegisterFloat(unknown507);
		RegisterFloat(unknown508);
		RegisterFloat(unknown509);
		RegisterFloat(unknown510);
		RegisterFloat(unknown511);
		RegisterFloat(unknown512);
		RegisterFloat(unknown513);
		RegisterFloat(unknown514);
		RegisterUInt32(haste_tt);
		RegisterUInt32(dps_pve_tt);
		RegisterUInt32(dps_pvp_tt);
		RegisterFloat(multi_attack_pve_tt);
		RegisterFloat(multi_attack_pvp_tt);
		RegisterFloat(unknown520);
		RegisterFloat(unknown521);
		RegisterFloat(unknown522);
		RegisterFloat(unknown523);
		RegisterUInt8(unknown550);

		uint8_t& Unknown552 = unknown552[0];
		RegisterUInt8(Unknown552)->SetCount(27);

		RegisterFloat(adventure_effects_cap);
		RegisterFloat(tradeskill_effects_cap);
		RegisterFloat(aa_effects_cap);

		RescopeArrayElement(unknown525a);
		RegisterUInt8(unknown525a)->SetCount(36);
		RegisterUInt64(spell_state_flags);
		RescopeArrayElement(unknown525b);
		RegisterUInt8(unknown525b)->SetCount(6);
		
		RegisterSubstruct(groupSheet);

		RegisterFloat(humidity);
		RegisterFloat(wind_direction);

		RescopeToReference(statusPoints, uint32_t);
		RescopeToReference(guild_status, uint32_t);
		RegisterUInt32(statusPoints);
		RegisterUInt32(guild_status);

		RegisterUInt8(unknownAOMa);
		
		//house zone and the following 151 bytes (199 total) appear to be the same object
		static std::string house_zone = "house zone";
		RegisterCharString(house_zone, 48);
		uint8_t& Unknown526 = unknown526[0];
		RegisterUInt8(Unknown526)->SetCount(151);

		
		//bind zone and the following bytes (84 total) appear to be the same object
		static std::string bind_zone = "bind zone";
		RegisterCharString(bind_zone, 32);
		uint8_t& Unknown188 = unknown188[0];
		RegisterUInt8(Unknown188)->SetCount(42);
	}

	void RegisterElements67650() {
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
		static int16_t unknownBOLa;
		RegisterInt16(unknownBOLa);
		RegisterInt32(gmLevel); //0-15
		RegisterUInt16(account_age_base);
		RegisterUInt16(account_age_bonus);

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
		int32_t& baseHp = attributes->hp.baseValue;
		RegisterInt32(baseHp);
		static int32_t unconciousHp = 0;
		RegisterInt32(unconciousHp);

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

		RegisterUInt32(hp_regen);
		RegisterUInt32(power_regen);

		uint32_t& Unknown6 = unknown6[0];
		RegisterUInt32(Unknown6)->SetCount(2);

		RegisterFloat(ranged_attack_min_distance);
		RegisterFloat(ranged_attack_max_distance);
		RegisterFloat(stat_bonus_health);
		RegisterFloat(stat_bonus_power);
		RegisterUInt32(bonus_health);
		RegisterUInt32(unknown8);
		RegisterUInt32(bonus_power);
		RegisterFloat(stat_bonus_damage);
		RegisterUInt16(mitigation_pct_pve);
		RegisterUInt16(mitigation_pct_pvp);
		RegisterUInt16(toughness);
		RegisterFloat(toughness_resist_dmg_pvp);
		RegisterUInt16(lethality);
		RegisterFloat(lethality_pct);

		static uint8_t g_unknown168[0x180 - 0x168];
		uint8_t& unknown168 = g_unknown168[0];
		RegisterUInt8(unknown168)->SetCount(sizeof(g_unknown168));

		RegisterUInt16(uncontested_riposte);
		RegisterUInt16(uncontested_dodge);
		RegisterUInt16(uncontested_parry);
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

		RegisterUInt16(unknown14);
		RegisterUInt16(elemental_absorb_pve);
		RegisterUInt16(noxious_absorb_pve);
		RegisterUInt16(arcane_absorb_pve);

		RegisterUInt16(unknown15);
		RegisterUInt16(elemental_absorb_pvp);
		RegisterUInt16(noxious_absorb_pvp);
		RegisterUInt16(arcane_absorb_pvp);

		RegisterUInt16(unknown16);
		RegisterUInt16(elemental_dmg_reduction);
		RegisterUInt16(noxious_dmg_reduction);
		RegisterUInt16(arcane_dmg_reduction);

		RegisterUInt16(unknown17);
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
		RegisterUInt32(recruit_friend_bonus);

		static bool bCanLevelPast90 = true;
		RegisterBool(bCanLevelPast90);

		RegisterUInt16(adventure_vitality);
		RegisterUInt16(adventure_vitality_yellow_arrow);
		RegisterUInt16(adventure_vitality_blue_arrow);
		RegisterUInt16(tradeskill_vitality);
		RegisterUInt16(tradeskill_vitality_purple_arrow);
		RegisterUInt16(tradeskill_vitality_blue_arrow);

		static float mentor_xp_mod = 0.f;
		RegisterFloat(mentor_xp_mod);
		RegisterUInt16(assigned_aa);
		RegisterUInt16(max_aa);
		RegisterUInt16(unassigned_aa);
		RegisterUInt16(aa_green_bar);
		RegisterUInt16(adv_xp_to_aa_xp_slider);
		RegisterUInt16(unknown21);
		RegisterUInt16(aa_blue_bar);
		RegisterUInt16(bonus_achievement_xp);
		static int32_t aa_level_up_bonus_count = 0;
		RegisterInt32(aa_level_up_bonus_count);
		RegisterUInt32(items_found);
		RegisterUInt32(named_npcs_killed);
		RegisterUInt32(quests_completed);
		RegisterUInt32(exploration_events);
		RegisterUInt32(completed_collections);

		static uint8_t g_unknown251[0x266 - 0x251];
		uint8_t& unknown251 = g_unknown251[0];
		RegisterUInt8(unknown251)->SetCount(sizeof(g_unknown251));

		RegisterUInt16(total_prestige_points);
		RegisterUInt16(unassigned_prestige_points);
		RegisterUInt16(unknown26);
		RegisterUInt16(unknown27);
		RegisterUInt16(total_tradeskill_points);
		RegisterUInt16(unassigned_tradeskill_points);
		RegisterUInt16(unknown28);
		RegisterUInt16(unknown29);
		RegisterUInt16(total_tradeskill_prestige_points);
		RegisterUInt16(unassigned_tradeskill_prestige_points);
		RegisterUInt16(unknown30);
		RegisterUInt16(unknown31);
		RegisterUInt16(unknown32);
		RegisterUInt16(unknown33);

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

		//Tradeskill sheet
		RegisterFloat(durability_mod);
		RegisterFloat(durability_add);
		RegisterFloat(progress_mod);
		RegisterFloat(progress_add);
		RegisterFloat(success_mod);
		RegisterFloat(crit_success_mod);
		RegisterFloat(crit_failure_mod);
		RegisterFloat(rare_harvest_chance);
		RegisterFloat(max_crafting);
		RegisterFloat(component_refund);
		RegisterFloat(ex_attempts);
		RegisterFloat(refine_quantity_mod);
		RegisterFloat(ex_durability_mod);
		RegisterFloat(ex_durability_add);
		RegisterFloat(ex_crit_success_mod);
		RegisterFloat(ex_crit_failure_mod);
		RegisterFloat(ex_progress_mod);
		RegisterFloat(ex_progress_add);
		RegisterFloat(ex_success_mod);

		static uint8_t g_unknown35e9[0x37fd - 0x35e9];
		uint8_t& unknown35e9 = g_unknown35e9[0];
		RegisterUInt8(unknown35e9)->SetCount(sizeof(g_unknown35e9));

		RegisterUInt64(spell_state_flags);

		static uint8_t g_unknown3805[0x380f - 0x3805];
		uint8_t& unknown3805 = g_unknown3805[0];
		RegisterUInt8(unknown3805)->SetCount(sizeof(g_unknown3805));

		RegisterSubstruct(groupSheet);
		RegisterFloat(humidity);
		RegisterFloat(wind_direction);
		RegisterUInt64(statusPoints);
		RegisterUInt64(guild_status);


		RegisterUInt8(unknownAOMa);
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
};

class OP_UpdateCharacterSheetMsg_Packet : public UpdateCharacterSheetMsgData, public EQ2Packet {
public:
	OP_UpdateCharacterSheetMsg_Packet(uint32_t version) : EQ2Packet(version), packedData(version <= 283), UpdateCharacterSheetMsgData(version) {
		//NOTE this is not used, it is just used to prevent compiler errors for reading packets and the struct dumper
	}

	OP_UpdateCharacterSheetMsg_Packet(uint32_t version, const CharacterSheet& sheet) : UpdateCharacterSheetMsgData(version, sheet), EQ2Packet(version), packedData(version <= 283) {
		packedData.LinkSubstruct(*static_cast<UpdateCharacterSheetMsgData*>(this), "packed_data");
		//RegisterSubstruct(packedData);
		//Can't use the macro while inheriting from both PacketSubstruct and EQ2Packet
		auto e = new PacketSubstructParent<PacketPackedData>(packedData);
		e->SetName("packedData");
		EQ2Packet::elements.push_back(e);
	}
	
	PacketPackedData packedData;
};

//Below is a work in progress reference from aom

//struct ClientCharacterSheet {
//	/*0x0000*/ char name[42];
//	/*0x002a*/ unsigned char race;
//	/*0x002b*/ unsigned char gender;
//	/*0x002c*/ unsigned char alignment;
//	/*0x002d*/ unsigned int advArchetype;
//	/*0x0031*/ unsigned int advBaseClass;
//	/*0x0035*/ unsigned int advClass;
//	/*0x0039*/ unsigned int tsArchetype;
//	/*0x003d*/ unsigned int tsBaseClass;
//	/*0x0041*/ unsigned int tsClass;
//	/*0x0045*/ short level;
//	/*0x0047*/ short effective_level;
//	/*0x0049*/ short ts_level;
//	/*0x004b*/ int gm_level; //0-15
//	/*0x004f*/ unsigned short account_age_base;
//	/*0x0051*/ unsigned short account_age_bonus;
//	/*0x0053*/ char deity[32];
//	/*0x0073*/ char lastName[21];
//	/*0x0088*/ char twitter_prepend[42];
//	/*0x00b2*/ char fb_prepend[42];
//	/*0x00dc*/ __int64 hp;
//	/*0x00e4*/ __int64 maxHp;
//	/*0x00ec*/ int baseMaxHp;
//	/*0x00f0*/ int unconciousHp;
//	/*0x00f4*/ int power;
//	/*0x00f8*/ int power_max;
//	/*0x00fc*/ int power_base;
//	/*0x0100*/ unsigned char concentration_used;
//	/*0x0101*/ unsigned char concentration_max;
//	/*0x0102*/ int savagery;
//	/*0x0106*/ int savagery_max;
//	/*0x010a*/ int savagery_base;
//	/*0x010e*/ int savagery_level;
//	/*0x0112*/ int savagery_level_max;
//	/*0x0116*/ int savagery_level_base;
//	/*0x011a*/ int dissonance;
//	/*0x011e*/ int dissonance_max;
//	/*0x0122*/ int dissonance_base;
//	/*0x0126*/ int health_regen_per_tick;
//	/*0x012a*/ int power_regen_per_tick;
//	/*0x012e*/ unsigned char unknown12e[0x132 - 0x12e];
//	/*0x0132*/ int dissipation;
//	/*0x0136*/ unsigned char unknown136[0x13e - 0x136];
//	/*0x013e*/ float hp_per_sta;
//	/*0x0142*/ float power_per_primary_stat;
//	/*0x0146*/ __int64 hp_bonus;
//	/*0x014e*/ int power_bonus;
//	/*0x0152*/ float stat_bonus_damage;
//	/*0x0156*/ short absorb_pve;
//	/*0x0158*/ short absorb_pvp;
//	/*0x015a*/ short toughness;
//	/*0x015c*/ float toughness_percent;
//	/*0x0160*/ short lethality;
//	/*0x0162*/ float lethality_percent;
//	/*0x0166*/ short avoidance_chance;
//	/*0x0168*/ short avoidance_base;
//	/*0x016a*/ short avoidance_unknown; //riposte or parry maybe
//	/*0x016c*/ unsigned char unknown16c[0x16e - 0x16c];
//	/*0x016e*/ short avoidance_base_chance;
//	/*0x0170*/ short avoidance_base_chance_cap; //guess
//	/*0x0172*/ short avoidance_block_chance;
//	/*0x0174*/ short avoidance_parry_chance_cap;
//	/*0x0176*/ short block;
//	/*0x0178*/ short block_base;
//	/*0x017a*/ short block_uncontested;
//	/*0x017c*/ short block_bonus_or_max; //guess
//	/*0x017e*/ short riposte_uncontested;
//	/*0x0180*/ short dodge_uncontested;
//	/*0x0182*/ short parry_uncontested;
//	/*0x0184*/ int str;
//	/*0x0188*/ int sta;
//	/*0x018c*/ int agi;
//	/*0x0190*/ int wis;
//	/*0x0194*/ int intel;
//	/*0x0198*/ int str_base;
//	/*0x019c*/ int sta_base;
//	/*0x01a0*/ int agi_base;
//	/*0x01a4*/ int wis_base;
//	/*0x01a8*/ int intel_base;
//	/*0x01ac*/ int mitigation;
//	/*0x01b0*/ int elemental;
//	/*0x01b4*/ int noxious;
//	/*0x01b8*/ int arcane;
//	/*0x01bc*/ int mitigation_base;
//	/*0x01c0*/ int elemental_base;
//	/*0x01c4*/ int noxious_base;
//	/*0x01c8*/ int arcane_base;
//	/*0x01cc*/ short physical_absorb_pve;
//	/*0x01ce*/ short elemental_absorb_pve;
//	/*0x01d0*/ short noxious_absorb_pve;
//	/*0x01d2*/ short arcane_absorb_pve;
//	/*0x01d4*/ short physical_absorb_pvp;
//	/*0x01d6*/ short elemental_absorb_pvp;
//	/*0x01d8*/ short noxious_absorb_pvp;
//	/*0x01da*/ short arcane_absorb_pvp;
//	/*0x01dc*/ short physical_damage_reduction;
//	/*0x01de*/ short elemental_damage_reduction;
//	/*0x01e0*/ short noxious_damage_reduction;
//	/*0x01e2*/ short arcane_damage_reduction;
//	/*0x01e4*/ short physical_damage_reduction_pct;
//	/*0x01e6*/ short elemental_damage_reduction_pct;
//	/*0x01e8*/ short noxious_damage_reduction_pct;
//	/*0x01ea*/ short arcane_damage_reduction_ct;
//	/*0x01ec*/ float adv_experience_current;
//	/*0x01f0*/ float adv_experience_next_level;
//	/*0x01f4*/ float adv_experience_debt;
//	/*0x01f8*/ float ts_experience_current;
//	/*0x01fc*/ float ts_experience_next_level;
//	/*0x0200*/ float ts_experience_debt;
//	/*0x0204*/ short adv_experience_server_bonus;
//	/*0x0206*/ short adv_experience_vet_bonus;
//	/*0x0208*/ short ts_experience_vet_bonus;
//	/*0x020a*/ short df_experience_bonus;
//	/*0x020c*/ short recruit_a_friend_count;
//	/*0x020e*/ short recruit_a_friend_bonus;
//	/*0x0210*/ bool bCanLevelPast90; //if false a tooltip appears about needing velious + 280 aa to level past 90 on the xp bar
//	/*0x0211*/ short vitality;
//	/*0x0213*/ short vitality_up_arrow;
//	/*0x0215*/ short vitality_down_arrow;
//	/*0x0217*/ short ts_vitality;
//	/*0x0219*/ short ts_vitality_up_arrow;
//	/*0x021b*/ short ts_vitality_down_arrow;
//	/*0x021d*/ float mentor_xp_mod;
//	/*0x0221*/ short aa_points;
//	/*0x0223*/ short aa_max_points;
//	/*0x0225*/ short aa_available_points;
//	/*0x0227*/ short aa_experience_current;
//	/*0x0229*/ short aa_conversion;
//	/*0x022b*/ unsigned char unknown22b[0x22d - 0x22b];
//	/*0x022d*/ short aa_experience_next_level;
//	/*0x022f*/ short aa_experience_bonus;
//	/*0x0231*/ int aa_level_up_count;
//	/*0x0235*/ int aa_item_count;
//	/*0x0239*/ int aa_named_kill_count;
//	/*0x023d*/ int aa_quest_count;
//	/*0x0241*/ int aa_exploration_count;
//	/*0x0245*/ int aa_collection_count;
//	/*0x0249*/ unsigned char unknown249[0x25e - 0x249];
//	/*0x025e*/ short prestige_points;
//	/*0x0260*/ short prestige_max_points;
//	/*0x0262*/ float prestige_experience_current;
//	/*0x0266*/ short ts_aa_points;
//	/*0x0268*/ short ts_aa_max_points;
//	/*0x026a*/ float ts_aa_experience_current;
//	/*0x026e*/ short ts_prestige_points;
//	/*0x0270*/ short ts_prestige_max_points;
//	/*0x0272*/ float ts_prestige_experience_current;
//	/*0x0276*/ float pet_experience_current;
//	/*0x027a*/ int coins_copper;
//	/*0x027e*/ int coins_silver;
//	/*0x0282*/ int coins_gold;
//	/*0x0286*/ int coins_platinum;
//	/*0x028a*/ unsigned char unknown28a[0x29e - 0x28a];
//	/*0x029e*/ CharacterSheetSpellEffect spellEffects[45];
//	/*0x0979*/ CharacterSheetSpellEffect detrimentalEffects[45];
//	/*0x1054*/ CharacterSheetSpellEffect passiveEffects[100];
//	/*0x1f90*/ bool bPassiveEffectUnknown[128]; //maybe to hide or show it?
//	/*0x2010*/ int passiveCount;
//	/*0x2014*/ unsigned char unknown2014[0x2176 - 0x2014];
//	/*0x2176*/ char traumaCount;
//	/*0x2177*/ char arcaneCount;
//	/*0x2178*/ char noxiousCount;
//	/*0x2179*/ char elementalCount;
//	/*0x217a*/ char curseCount;
//	/*0x217b*/ CharacterSheetMaintainedEffect maintainedEffects[30];
//	/*0x2c61*/ float breath;
//	/*0x2c65*/ unsigned int breathableEnvironmentFlags;
//	/*0x2c69*/ unsigned char unknown2c69;
//	//2c69 may be a bitflag for the following bools?
//	//Data           :   this+0x9BE, Member, Type: float, fBreath
//	//Data           :   this+0x9C2, Member, Type: unsigned int, u32BreathableEnvironments
//	//Data           :   this+0x9C6, Member, Type: bool, bAutoAttackOn
//	//Data           :   this+0x9C7, Member, Type: bool, bCanCast
//	//Data           :   this+0x9C8, Member, Type: bool, bPreZoning
//	//Data           :   this+0x9C9, Member, Type: bool, bMaxLevel
//	//Data           :   this+0x9CA, Member, Type: bool, bMaxTSLevel
//	//Data           :   this+0x9CB, Member, Type: bool, bInFeigningDeathSpellState
//	/*0x2c6a*/ int primary_damage_low;
//	/*0x2c6e*/ int primary_damage_high;
//	/*0x2c72*/ float primary_delay;
//	/*0x2c76*/ int secondary_damage_low;
//	/*0x2c7a*/ int secondary_damage_high;
//	/*0x2c7e*/ float secondary_delay;
//	/*0x2c82*/ int ranged_damage_low;
//	/*0x2c86*/ int ranged_damage_high;
//	/*0x2c8a*/ float ranged_delay;
//	/*0x2c8e*/ unsigned char unknown2c8e[0x2c9e - 0x2c8e];
//	/*0x2c9e*/ float ability_mod;
//	/*0x2ca2*/ unsigned char unknown2ca2[0x2cb2 - 0x2ca2];
//	/*0x2cb2*/ float ability_mod_pvp;
//	/*0x2cb6*/ float base_melee_crit_multiplier;
//	/*0x2cba*/ float base_spell_crit_multiplier;
//	/*0x2cbe*/ float base_taunt_crit_multiplier;
//	/*0x2cc2*/ float base_heal_crit_multiplier;
//	/*0x2cc6*/ unsigned int character_flags1; //1 = combat adv xp, 0x10 = auto attack, 0x20 = ranged auto attack, 0x40 = quest xp, 0x400 = fd (to send the ClientFeignCancelled packet when standing), 0x1000 = anonymous, 0x2000 = RP, 0x4000 = AFK, 0x8000 = LFG, 0x10000 = LFW, 0x20000 = hide hood, 0x40000 = hide helm, 0x80000 = hide illus, 0x100000 = duel invites, 0x200000 = trade invites, 0x400000 = group invites, 0x800000 = raid invites, 0x1000000 = guild invites, 0x8000000 = defense skills not maxed for level, 0x10000000 = guild heraldry, 0x20000000 = hide cloak, 0x80000000 = is_hated
//	/*0x2cca*/ unsigned int character_flags2; //1 = not all armor slots filled cause lower reduction, 8 = LON invites, 0x10 = show ranged, 0x20 = voice invites, 0x40 = character bonus xp, 0x100 = hide achievements, 0x400 = house edit mode, 0x800 = facebook enabled, 0x1000 = twitter enabled, 0x2000 = census enabled, 0x4000 = census alts enabled, 0x20000 = khrono enabled, 0x400000 = spell filtering
//	/*0x2cce*/ char afk_message[0x100];
//	/*0x2dce*/ unsigned char unknown2dce[0x2dea - 0x2dce];
//	/*0x2dea*/ float stamina_bonus_hp;
//	/*0x2dee*/ unsigned char unknown2dee[0x2e12 - 0x2dee];
//	/*0x2e12*/ float haste;
//	/*0x2e16*/ unsigned char unknown2e16[0x2e22 - 0x2e16];
//	/*0x2e22*/ float drunkeness;
//	/*0x2e26*/ unsigned char unknown2e26[0x2e2a - 0x2e26];
//	/*0x2e2a*/ float hate_mod;
//	/*0x2e2e*/ float combat_experience_mod;
//	/*0x2e32*/ float ts_experience_mod;
//	/*0x2e36*/ float aa_experience_mod;
//	/*0x2e3a*/ unsigned char unknown2e3a[0x2e42 - 0x2e3a];
//	/*0x2e42*/ float dps;
//	/*0x2e46*/ unsigned char unknown2e46[0x2e66 - 0x2e46];
//	/*0x2e66*/ float ae_auto_attack_chance;
//	/*0x2e6a*/ float multi_attack;
//	/*0x2e6e*/ float spell_doublecast;
//	/*0x2e72*/ float flurry_chance;
//	/*0x2e76*/ unsigned char unknown2e76[0x2e7a - 0x2e76];
//	/*0x2e7a*/ float bountiful_harvest;
//	/*0x2e7e*/ float block_chance_mod;
//	/*0x2e82*/ unsigned char unknown2e82[0x2e8a - 0x2e82];
//	/*0x2e8a*/ float crit_chance;
//	/*0x2e8e*/ unsigned char unknown2e8e[0x2e96 - 0x2e8e];
//	/*0x2e96*/ float crit_bonus;
//	/*0x2e9a*/ float potency;
//	/*0x2e9e*/ unsigned char unknown2e9e[0x2ea2 - 0x2e9e];
//	/*0x2ea2*/ float ability_reuse_speed;
//	/*0x2ea6*/ float ability_recovery_speed;
//	/*0x2eaa*/ float ability_casting_speed;
//	/*0x2eae*/ float spell_reuse_speed;
//	/*0x2eb2*/ unsigned char unknown2eb2[0x2ec6 - 0x2eb2];
//	/*0x2ec6*/ float player_movement_unknown;
//	/*0x2eca*/ unsigned char unknown2eca[0x2eea - 0x2eca];
//	/*0x2eea*/ float strikethrough_pve;
//	/*0x2eee*/ unsigned char unknown2eee[0x2ef2 - 0x2eee];
//	/*0x2ef2*/ float accuracy;
//	/*0x2ef6*/ unsigned char unknown2ef6[0x2efe - 0x2ef6];
//	/*0x2efe*/ float weapon_damage_bonus;
//	/*0x2f02*/ float spell_weapon_damage_bonus;
//	/*0x2f06*/ unsigned char unknown2f06[0x2f1a - 0x2f06];
//	/*0x2f1a*/ float toughness_crit_dmg_reduction;
//	/*0x2f1e*/ unsigned char unknown2f1e[0x2f2a - 0x2f1e];
//	/*0x2f2a*/ float pvp_crit_mit;
//	/*0x2f2e*/ unsigned char unknown2f2e[0x2ff2 - 0x2f2e];
//	/*0x2ff2*/ float riposte_uncontested_gear_cap;
//	/*0x2ff6*/ float parry_uncontested_gear_cap;
//	/*0x2ffa*/ float dodge_uncontested_gear_cap;
//	/*0x2ffe*/ unsigned char unknown2ffe[0x3196 - 0x2ffe];
//	/*0x3196*/ float ae_auto_attack_chance_pve;
//	/*0x319a*/ unsigned char unknown319a[0x31a2 - 0x319a];
//	/*0x31a2*/ float flurry_chance_pvp;
//	/*0x31a6*/ unsigned char unknown31a6[0x31c6 - 0x31a6];
//	/*0x31c6*/ float crit_bonus_pvp;
//	/*0x31ca*/ float potency_pvp;
//	/*0x31ce*/ unsigned char unknown31ce[0x321a - 0x31ce];
//	/*0x321a*/ float strikethrough_pvp;
//	/*0x321e*/ unsigned char unknown321e[0x3222 - 0x321e];
//	/*0x3222*/ float accuracy_pvp;
//	/*0x3226*/ unsigned char unknown3226[0x322e - 0x3226];
//	/*0x322e*/ float weapon_damage_bonus_pvp;
//	/*0x3232*/ float spell_weapon_damage_bonus_pvp;
//	/*0x3236*/ unsigned char unknown3236[0x3296 - 0x3236];
//	/*0x3296*/ float durability_mod;
//	/*0x329a*/ float durability_add;
//	/*0x329e*/ float progress_mod;
//	/*0x32a2*/ float progress_add;
//	/*0x32a6*/ float success_mod;
//	/*0x32aa*/ float crit_success_mod;
//	/*0x32ae*/ float crit_failure_mod;
//	/*0x32b2*/ float rare_harvest_chance;
//	/*0x32b6*/ float max_crafting_quantity;
//	/*0x32ba*/ float build_components_returned;
//	/*0x32be*/ float experimentation_attempts;
//	/*0x32c2*/ float refine_quantity_mod;
//	/*0x32c6*/ float experiment_durability_mod;
//	/*0x32ca*/ float experiment_durability_add;
//	/*0x32ce*/ float experiment_crit_success_mod;
//	/*0x32d2*/ float experiment_crit_failure_mod;
//	/*0x32d6*/ float experiment_progress_mod;
//	/*0x32da*/ float experiment_progress_add;
//	/*0x32de*/ float experiment_success_mod;
//	/*0x32e2*/ unsigned char unknown32e2[0x3326 - 0x32e2];
//	/*0x3326*/ float haste_cap;
//	/*0x332a*/ unsigned char unknown332a[0x333e - 0x332a];
//	/*0x333e*/ float hate_mod_cap;
//	/*0x3342*/ unsigned char unknown3342[0x3356 - 0x3342];
//	/*0x3356*/ float dps_cap;
//	/*0x335a*/ unsigned char unknown335a[0x337a - 0x335a];
//	/*0x337a*/ float ae_auto_attack_chance_cap;
//	/*0x337e*/ unsigned char unknown337e[0x3382 - 0x337e];
//	/*0x3382*/ float spell_doublecast_cap;
//	/*0x3386*/ float flurry_cap;
//	/*0x339a*/ unsigned char unknown339a[0x339e - 0x339a];
//	/*0x338e*/ float bountiful_harvest_cap;
//	/*0x3392*/ float block_chance_cap;
//	/*0x3396*/ unsigned char unknown3396[0x339e - 0x3396];
//	/*0x339e*/ float crit_chance_cap;
//	/*0x33a2*/ unsigned char unknown33a2[0x33aa - 0x33a2];
//	/*0x33aa*/ float crit_bonus_cap;
//	/*0x33ae*/ unsigned char unknown33ae[0x33b6 - 0x33ae];
//	/*0x33b6*/ float ability_reuse_speed_cap;
//	/*0x33ba*/ float ability_recovery_speed_cap;
//	/*0x33be*/ float ability_casting_speed_cap;
//	/*0x33c2*/ float spell_reuse_speed_cap;
//	/*0x33c6*/ unsigned char unknown33c6[0x33fe - 0x33c6];
//	/*0x33fe*/ float strikethrough_cap;
//	/*0x3402*/ float weapon_damage_bonus_cap;
//	/*0x3406*/ float accuracy_cap;
//	/*0x340a*/ unsigned char unknown340a[0x3416 - 0x340a];
//	/*0x3416*/ float spell_weapon_damage_bonus_cap;
//	/*0x341a*/ unsigned char unknown341a[0x343e - 0x341a];
//	/*0x343e*/ float pvp_crit_mit_cap;
//	/*0x3442*/ unsigned char unknown3442[0x347e - 0x3442];
//	/*0x347e*/ float ts_experience_mod_cap;
//	/*0x3482*/ float aa_experience_mod_cap;
//	/*0x3486*/ float casting_speed_overcap_spell_dblcast;
//	/*0x348a*/ float combat_experience_mod_cap;
//	/*0x348e*/ float attack_speed_overcap_flurry_chance;
//	/*0x3492*/ unsigned char unknown3492[0x3496 - 0x3492];
//	/*0x3496*/ int haste_effective; //i think this is the effective value after applying the stat + caps
//	/*0x349a*/ float dps_damage_modifier;
//	/*0x349e*/ float flurry_chance_cap;
//	/*0x34a2*/ float multi_attack_extra_atk_count;
//	/*0x34a6*/ float multi_attack_extra_atk_chance;
//	/*0x34aa*/ unsigned __int64 spell_states;
//	/*0x34b2*/ unsigned char unknown34b2[0x34b8 - 0x34b2];
//	/*0x34b8*/ ClientGroupSheet group_sheet;
//	/*0x3e3c*/ float humidity;
//	/*0x3e40*/ float wind_direction;
//	/*0x3e44*/ unsigned int currentStatus;
//	/*0x3e48*/ unsigned int guildStatus;
//	/*0x3e4c*/ unsigned char unknown3e4c;
//	/*0x3e4d*/ char house_location[48];
//	/*0x3e7d*/ unsigned char unknown3e7d[0x3f0f - 0x3e7d];
//	/*0x3f0f*/ char num_unlocked_bags; //if -1 all are enabled, otherwise valid values are 1 through 6
//	/*0x3f10*/ unsigned char unknown3f10[0x3f14 - 0x3f10];
//	/*0x3f14*/ char bind_zone[32];
//	/*0x3f34*/ unsigned char unknown3f34[0x3f51 - 0x3f34];
//	/*0x3f51*/ unsigned short lon_booster_count;
//	/*0x3f53*/ short brokerCredits;
//	/*0x3f55*/ unsigned char unknown3f55[0x3f61 - 0x3f55];
//	/*0x3f61*/ float fUnknown3f61;
//	/*0x3f65*/ unsigned char unknown3f65[0x3f68 - 0x3f65];
//	/*0x3f68*/
//};