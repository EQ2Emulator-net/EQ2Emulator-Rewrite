#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"

#include "Substruct_SpellEffects.h"
#include "Substruct_PassiveEffects.h"
#include "Substruct_MaintainedEffects.h"
#include "Substruct_GroupMember.h"
#include "../Players/CharacterSheet.h"

struct TsSpellProps {
	TsSpellProps() {
		memset(this, 0, sizeof(*this));
	}

	float durability_mod;
	float durability_add;
	float progress_mod;
	float progress_add;
	float success_mod;
	float crit_success_mod;
	float crit_failure_mod;
	float rare_harvest_chance;
	float max_crafting_quantity;
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
};

class Substruct_TsSpellProps : public PacketSubstruct, public TsSpellProps {
public:
	Substruct_TsSpellProps(uint32_t ver = 0) : PacketSubstruct(ver) {
		RegisterElements();
	}

	~Substruct_TsSpellProps() = default;

	void RegisterElements() override {
		RegisterFloat(durability_mod);
		RegisterFloat(durability_add);
		RegisterFloat(progress_mod);
		RegisterFloat(progress_add);
		RegisterFloat(success_mod);
		RegisterFloat(crit_success_mod);
		RegisterFloat(crit_failure_mod);
		RegisterFloat(rare_harvest_chance);
		RegisterFloat(max_crafting_quantity);
		RegisterFloat(component_refund);
		RegisterFloat(ex_attempts);
		if (GetVersion() >= 60114) {
			RegisterFloat(refine_quantity_mod);
		}
		RegisterFloat(ex_durability_mod);
		RegisterFloat(ex_durability_add);
		RegisterFloat(ex_crit_success_mod);
		RegisterFloat(ex_crit_failure_mod);
		RegisterFloat(ex_progress_mod);
		RegisterFloat(ex_progress_add);
		RegisterFloat(ex_success_mod);
	}
};

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
	float power_received_fervor_mit;
	float heals_received_normalized_mit;
	float flurry_multiplier;
	float flurry_multiplier_mitigation;

	SpellProps() {
		memset(this, 0, sizeof(*this));
	}
};

class Substruct_SpellProps : public PacketSubstruct, public SpellProps {
public:
	Substruct_SpellProps(uint32_t ver = 0) : PacketSubstruct(ver) {
		RegisterElements();
	}

	~Substruct_SpellProps() = default;

	void RegisterElements() override {
		const uint32_t ver = GetVersion();

		//Being lazy :)
		if (ver <= 843) {
			static float g_blueStats[66] = { 0.f };
			float& blueStats = g_blueStats[0];
			RegisterFloat(blueStats)->SetCount(66);
			return;
		}

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
				if (version >= 67804) {
					RegisterFloat(power_received_fervor_mit);
				}
				RegisterFloat(heals_received_normalized_mit);
				if (version >= 67804) {
					RegisterFloat(flurry_multiplier);
					RegisterFloat(flurry_multiplier_mitigation);
				}
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
		bCanLevelPast90 = true;
		unknownCOEa = 6;
	}

	//Some elements are a different format in the packet from what we use, convert them here
	int64_t hp;
	int64_t maxHp;
	int64_t maxHpBase;
	int64_t current_power;
	int64_t max_power;
	int64_t base_power;
	int64_t hp_regen;
	int64_t power_regen;
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
	uint32_t account_flags; //this sets the account age bonus time
	
	int32_t base_savagery;
	int32_t savagery_level;
	int32_t max_savagery_level;
	int32_t base_savagery_level;
	int32_t dissonance;
	int32_t max_dissonance;
	int32_t base_dissonance;
	int32_t savagery_regen_per_sec;
	int32_t dissipation;
	float ranged_attack_min_distance;
	float ranged_attack_max_distance;
	float stat_bonus_health;
	float stat_bonus_power;
	int64_t bonus_health;
	int64_t bonus_power;
	float stat_bonus_damage;
	uint16_t mitigation_pct_pve;
	uint16_t mitigation_pct_pvp;
	uint16_t toughness;
	float toughness_resist_dmg_pvp;
	uint16_t lethality;
	float lethality_pct;
	int16_t avoidance_overall_chance;
	int16_t avoidance_base;
	int16_t avoidance_riposte_chance;
	int16_t avoidance_riposte_chance_base;
	int16_t avoidance_base_chance;
	int16_t avoidance_base_chance_base;
	int16_t avoidance_parry_chance;
	int16_t avoidance_parry_chance_base;
	int16_t avoidance_deflection_chance;
	int16_t avoidance_deflection_chance_base;
	int16_t avoidance_block_chance;
	int16_t avoidance_block_chance_base;
	int16_t avoidance_block_uncontested;
	int16_t avoidance_deflection_uncontested;
	int16_t avoidance_riposte_uncontested;
	int16_t avoidance_dodge_uncontested;
	int16_t avoidance_parry_uncontested;
	uint16_t physical_absorb_pve;
	uint16_t elemental_absorb_pve;
	uint16_t noxious_absorb_pve;
	uint16_t arcane_absorb_pve;
	uint16_t physical_absorb_pvp;
	uint16_t elemental_absorb_pvp;
	uint16_t noxious_absorb_pvp;
	uint16_t arcane_absorb_pvp;
	uint16_t physical_dmg_reduction;
	uint16_t elemental_dmg_reduction;
	uint16_t noxious_dmg_reduction;
	uint16_t arcane_dmg_reduction;
	uint16_t physical_dmg_reduction_pct;
	uint16_t elemental_dmg_reduction_pct;
	uint16_t noxious_dmg_reduction_pct;
	uint16_t arcane_dmg_reduction_pct;
	uint16_t server_bonus;
	uint16_t adventure_vet_bonus;
	uint16_t tradeskill_vet_bonus;
	uint16_t dungeon_finder_bonus;
	uint16_t recruit_friend_bonus;
	uint16_t recruit_friend_count;
	bool bCanLevelPast90;
	uint16_t adventure_vitality;
	uint16_t adventure_vitality_yellow_arrow;
	uint16_t adventure_vitality_blue_arrow;
	uint16_t tradeskill_vitality;
	uint16_t tradeskill_vitality_purple_arrow;
	uint16_t tradeskill_vitality_blue_arrow;
	float mentor_bonus;
	uint16_t earned_aa;
	uint16_t max_aa;
	uint16_t unassigned_aa;
	uint16_t aa_green_bar;
	uint16_t adv_xp_to_aa_xp_slider;
	uint16_t unknown21;
	uint16_t aa_blue_bar;
	uint16_t aa_bonus_xp;
	uint16_t aa_next_level;
	uint32_t aa_items_found;
	uint32_t aa_named_npcs_killed;
	uint32_t aa_quests_completed;
	uint32_t aa_exploration_events;
	uint32_t aa_completed_collections;
	uint32_t aa_level_up_events;
	uint8_t unknown24[16];
	uint32_t lastPictureSubmitionTime;
	uint8_t unknown25d;
	uint16_t prestige_earned_points;
	uint16_t prestige_max_points;
	float prestige_xp_current;
	uint16_t ts_aa_earned_points;
	uint16_t ts_aa_max_points;
	float ts_aa_experience_current;
	uint16_t ts_prestige_earned_points;
	uint16_t ts_prestige_max_points;
	float ts_prestige_experience_current;
	float pet_experience_current;
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
	float adventure_effects_cap;
	float tradeskill_effects_cap;
	float aa_effects_cap;
	uint8_t unknown525a[36];
	//Known useful spell state values:
	//1<<1 = charm (prevents things like sitting and changing targets while charmed)
	//1<<7 = purple (ultravision)
	//1<<8 = heat (infravision)
	//1<<9 = piss (sonicvision)
	//1<<10 = colorblind vision (grey)
	//1<<11 = green vision
	//1<<12 = red vision
	//1<<13 = fish vision
	//a bunch of others but not sure that anything else actually "does" anything for the client
	uint64_t spell_state_flags;
	uint8_t unknown525b[6];
	float humidity;
	float wind_direction;
	uint8_t unknown527[4];
	uint64_t guild_status;

	//Not sure if this was added in coe but that is the earliest struct so far i've added with it
	uint8_t unknownCOEa;
	uint8_t unknown526[151];
	uint8_t unknown188[52];

protected:
	int32_t bonus_health_do_not_set;
	int32_t bonus_power_do_not_set;
	int32_t hp_regen_do_not_set;
	int32_t power_regen_do_not_set;

	int16_t adv_xp_do_not_set;
	int16_t adv_xp_next_level_do_not_set;
	int16_t adv_xp_debt_do_not_set;

	int16_t ts_xp_do_not_set;
	int16_t ts_xp_next_level_do_not_set;
	int16_t ts_xp_debt_do_not_set;

	//Legacy resists
	int16_t resist_slashing_do_not_set;
	int16_t resist_crushing_do_not_set;
	int16_t resist_piercing_do_not_set;
	int16_t resist_heat_do_not_set;
	int16_t resist_cold_do_not_set;
	int16_t resist_magic_do_not_set;
	int16_t resist_mental_do_not_set;
	int16_t resist_divine_do_not_set;
	int16_t resist_disease_do_not_set;
	int16_t resist_poison_do_not_set;

	int16_t resist_base_slashing_do_not_set;
	int16_t resist_base_crushing_do_not_set;
	int16_t resist_base_piercing_do_not_set;
	int16_t resist_base_heat_do_not_set;
	int16_t resist_base_cold_do_not_set;
	int16_t resist_base_magic_do_not_set;
	int16_t resist_base_mental_do_not_set;
	int16_t resist_base_divine_do_not_set;
	int16_t resist_base_disease_do_not_set;
	int16_t resist_base_poison_do_not_set;

	int16_t resist_absorb_slashing_do_not_set;
	int16_t resist_absorb_crushing_do_not_set;
	int16_t resist_absorb_piercing_do_not_set;
	int16_t resist_absorb_heat_do_not_set;
	int16_t resist_absorb_cold_do_not_set;
	int16_t resist_absorb_magic_do_not_set;
	int16_t resist_absorb_mental_do_not_set;
	int16_t resist_absorb_divine_do_not_set;
	int16_t resist_absorb_disease_do_not_set;
	int16_t resist_absorb_poison_do_not_set;
};

class UpdateCharacterSheetMsgData : public CharacterSheetMiscData, public CharacterSheet, public PacketEncodedData {
public:
	UpdateCharacterSheetMsgData(uint32_t ver) : PacketEncodedData(ver), CharacterSheet(nullptr), groupSheet(ver),
		pve_props(version), pvp_props(version), gear_cap_props(version), hard_cap_props(version), ts_props(version), base_props(version) {
		for (uint8_t i = 0; i < 45; i++)
			spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 45; i++)
			detrimental_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 100; i++)
			passive_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 30; i++)
			maintained_effects[i].ResetVersion(version);
		RegisterElements();
		
		bDumpOffsets = true;
	}

	UpdateCharacterSheetMsgData(uint32_t version, const CharacterSheet& sheet) : PacketEncodedData(version), CharacterSheet(sheet),
		groupSheet(version), pve_props(version), pvp_props(version), gear_cap_props(version), hard_cap_props(version), ts_props(version), base_props(version) {
		for (uint8_t i = 0; i < 45; i++)
			spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 45; i++)
			detrimental_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 100; i++)
			passive_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 30; i++)
			maintained_effects[i].ResetVersion(version);
		RegisterElements();

		bDumpOffsets = true;
	}

	void PreWrite() override;

	Substruct_SpellEffects spell_effects[45];
	Substruct_SpellEffects detrimental_spell_effects[45];
	Substruct_PassiveEffects passive_spell_effects[100];
	Substruct_MaintainedEffects maintained_effects[30];
	Substruct_GroupSheet groupSheet;

	Substruct_SpellProps pve_props;
	//I think these are the max values you can get of blue stats from gear if set? i only ever see uncontested riposte/parry/dodge
	Substruct_SpellProps gear_cap_props;
	Substruct_SpellProps pvp_props;
	Substruct_SpellProps hard_cap_props;
	Substruct_TsSpellProps ts_props;
	Substruct_SpellProps base_props;

private:
	double advExp_do_not_set;
	double advExpNextLevel_do_not_set;
	double tsExp_do_not_set;
	double tsExpNextLevel_do_not_set;

	int16_t str_do_not_set;
	int16_t sta_do_not_set;
	int16_t agi_do_not_set;
	int16_t wis_do_not_set;
	int16_t int_do_not_set;

	int16_t str_base_do_not_set;
	int16_t sta_base_do_not_set;
	int16_t agi_base_do_not_set;
	int16_t wis_base_do_not_set;
	int16_t int_base_do_not_set;

	void RegisterElements() override;
	void RegisterElementsLegacy();
	void RegisterElements67650();

	void RegisterAttributeElements();
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