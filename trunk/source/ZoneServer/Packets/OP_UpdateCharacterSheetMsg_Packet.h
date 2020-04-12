#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

#include "Substruct_SpellEffects.h"
#include "Substruct_PassiveEffects.h"
#include "Substruct_MaintainedEffects.h"
#include "Substruct_GroupMember.h"

class UpdateCharacterSheetMsgData : public PacketEncodedData {
public:
	UpdateCharacterSheetMsgData(uint32_t version) : PacketEncodedData(version), groupSheet(version) {
		RegisterElements();

		memset(character_name, 0, sizeof(character_name));
		unknown_1_1_MJ = 0;
		race = 0;
		gender = 0;
		exiled = 0;
		class1 = 0;
		class2 = 0;
		class3 = 0;
		tradeskill_class1 = 0;
		tradeskill_class2 = 0;
		tradeskill_class3 = 0;
		level = 0;
		effective_level = 0;
		tradeskill_level = 0;
		unknown_1_2_MJ = 0;
		account_age_base = 0;
		account_age_bonus = 0;
		memset(deity, 0, sizeof(deity));
		memset(last_name, 0, sizeof(last_name));
		unknown3 = 0;
		memset(character_name2, 0, sizeof(character_name2));
		character_name2_unknown = 0;
		memset(character_name3, 0, sizeof(character_name3));
		character_name3_unknown = 0;
		current_hp = 0;
		max_hp = 0;
		base_hp = 0;
		base_hp2 = 0;
		current_power = 0;
		max_power = 0;
		base_power = 0;
		conc_used = 0;
		conc_max = 0;
		savagery = 0;
		max_savagery = 0;
		unknown4b = 0;
		savagery_level = 0;
		max_savagery_level = 0;
		memset(unknown4c, 0, sizeof(unknown4c));
		dissonance = 0;
		max_dissonance = 0;
		memset(unknown5c, 0, sizeof(unknown5c));
		hp_regen = 0;
		power_regen = 0;
		memset(unknown6, 0, sizeof(unknown6));
		memset(unknown7, 0, sizeof(unknown7));
		stat_bonus_health = 0;
		stat_bonus_power = 0;
		bonus_health = 0;
		unknown8 = 0;
		bonus_power = 0;
		stat_bonus_damage = 0;
		mitigation_pct_pve = 0;
		mitigation_pct_pvp = 0;
		toughness = 0;
		toughness_resist_dmg_pvp = 0;
		lethality = 0;
		lethality_pct = 0;
		avoidance_pct = 0;
		avoidance_reduction = 0;
		avoidance = 0;
		unknown10 = 0;
		avoidance_base = 0;
		unknown10a = 0;
		parry = 0;
		unknown11 = 0;
		block = 0;
		unknown12 = 0;
		uncontested_block = 0;
		unknown13 = 0;
		uncontested_riposte = 0;
		uncontested_dodge = 0;
		uncontested_parry = 0;
		str = 0;
		sta = 0;
		agi = 0;
		wis = 0;
		intel = 0;
		str_base = 0;
		sta_base = 0;
		agi_base = 0;
		wis_base = 0;
		int_base = 0;
		mitigation_cur = 0;
		elemental = 0;
		noxious = 0;
		arcane = 0;
		mitigation_base = 0;
		elemental_base = 0;
		noxious_base = 0;
		arcane_base = 0;
		unknown14 = 0;
		elemental_absorb_pve = 0;
		noxious_absorb_pve = 0;
		arcane_absorb_pve = 0;
		unknown15 = 0;
		elemental_absorb_pvp = 0;
		noxious_absorb_pvp = 0;
		arcane_absorb_pvp = 0;
		unknown16 = 0;
		elemental_dmg_reduction = 0;
		noxious_dmg_reduction = 0;
		arcane_dmg_reduction = 0;
		unknown17 = 0;
		elemental_dmg_reduction_pct = 0;
		noxious_dmg_reduction_pct = 0;
		arcane_dmg_reduction_pct = 0;
		current_adv_xp = 0;
		needed_adv_xp = 0;
		debt_adv_xp = 0;
		current_trade_xp = 0;
		needed_trade_xp = 0;
		debt_trade_xp = 0;
		memset(unknown18, 0, sizeof(unknown18));
		server_bonus = 0;
		adventure_vet_bonus = 0;
		tradeskill_vet_bonus = 0;
		dungeon_finder_bonus = 0;
		recruit_friend_bonus = 0;
		unknown19 = 0;
		adventure_vitality = 0;
		adventure_vitality_yellow_arrow = 0;
		adventure_vitality_blue_arrow = 0;
		tradeskill_vitality = 0;
		tradeskill_vitality_purple_arrow = 0;
		tradeskill_vitality_blue_arrow = 0;
		mentor_bonus = 0;
		unknown20 = 0;
		assigned_aa = 0;
		max_aa = 0;
		unassigned_aa = 0;
		aa_green_bar = 0;
		adv_xp_to_aa_xp_slider = 0;
		unknown21 = 0;
		aa_blue_bar = 0;
		bonus_achievement_xp = 0;
		memset(unknown22, 0, sizeof(unknown22));
		memset(unknown23, 0, sizeof(unknown23));
		items_found = 0;
		named_npcs_killed = 0;
		quests_completed = 0;
		exploration_events = 0;
		completed_collections = 0;
		memset(unknown24, 0, sizeof(unknown24));
		unknown25 = 0;
		total_prestige_points = 0;
		unassigned_prestige_points = 0;
		unknown26 = 0;
		unknown27 = 0;
		total_tradeskill_points = 0;
		unassigned_tradeskill_points = 0;
		unknown28 = 0;
		unknown29 = 0;
		total_tradeskill_prestige_points = 0;
		unassigned_tradeskill_prestige_points = 0;
		unknown30 = 0;
		unknown31 = 0;
		unknown32 = 0;
		unknown33 = 0;
		coins_copper = 0;
		coins_silver = 0;
		coins_gold = 0;
		coins_plat = 0;
		memset(unknown34, 0, sizeof(unknown34));
		for (uint8_t i = 0; i < 45; i++)
			spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 45; i++)
			detrimental_spell_effects[i].ResetVersion(version);
		for (uint8_t i = 0; i < 100; i++)
			passive_spell_effects[i].ResetVersion(version);
		memset(unknown35, 0, sizeof(unknown35));
		trauma_count = 0;
		arcane_count = 0;
		noxious_count = 0;
		elemental_count = 0;
		curse_count = 0;
		for (uint8_t i = 0; i < 30; i++)
			maintained_effects[i].ResetVersion(version);
		breath = 0;
		breathe_underwater = 0;
		unknown36 = 0;
		melee_pri_dmg_min = 0;
		melee_pri_dmg_max = 0;
		melee_pri_delay = 0;
		melee_sec_dmg_min = 0;
		melee_sec_dmg_max = 0;
		melee_sec_delay = 0;
		ranged_dmg_min = 0;
		ranged_dmg_max = 0;
		ranged_delay = 0;
		unknown37 = 0;
		unknown38 = 0;
		unknown39 = 0;
		unknown40 = 0;
		ability_mod_pve = 0;
		unknown41 = 0;
		unknown42 = 0;
		unknown43 = 0;
		unknown44 = 0;
		ability_mod_pvp = 0;
		base_melee_crit = 0;
		base_spell_crit = 0;
		base_taunt_crit = 0;
		base_heal_crit = 0;
		flags = 0;
		flags2 = 0;
		unknown45 = 0;
		unknown46 = 0;
		unknown47 = 0;
		unknown48 = 0;
		unknown49 = 0;
		unknown50 = 0;
		unknown51 = 0;
		unknown52 = 0;
		unknown53 = 0;
		unknown54 = 0;
		unknown55 = 0;
		unknown56 = 0;
		unknown57 = 0;
		unknown58 = 0;
		unknown59 = 0;
		unknown60 = 0;
		unknown61 = 0;
		unknown62 = 0;
		unknown63 = 0;
		unknown64 = 0;
		unknown65 = 0;
		unknown66 = 0;
		unknown67 = 0;
		unknown68 = 0;
		unknown69 = 0;
		unknown70 = 0;
		unknown71 = 0;
		unknown72 = 0;
		unknown73 = 0;
		unknown74 = 0;
		unknown75 = 0;
		unknown76 = 0;
		unknown77 = 0;
		unknown78 = 0;
		unknown79 = 0;
		unknown80 = 0;
		unknown81 = 0;
		unknown82 = 0;
		unknown83 = 0;
		unknown84 = 0;
		unknown85 = 0;
		unknown86 = 0;
		unknown87 = 0;
		unknown88 = 0;
		unknown89 = 0;
		unknown90 = 0;
		unknown91 = 0;
		unknown92 = 0;
		unknown93 = 0;
		unknown94 = 0;
		unknown95 = 0;
		unknown96 = 0;
		unknown97 = 0;
		unknown98 = 0;
		unknown99 = 0;
		unknown100 = 0;
		unknown101 = 0;
		unknown102 = 0;
		unknown103 = 0;
		unknown104 = 0;
		unknown105 = 0;
		unknown106 = 0;
		unknown107 = 0;
		unknown108 = 0;
		unknown109 = 0;
		unknown110 = 0;
		out_of_combat_health_regen = 0;
		out_of_combat_power_regen = 0;
		in_combat_regen = 0;
		deflect = 0;
		increase_max_health = 0;
		increase_max_health2 = 0;
		unknown117 = 0;
		increase_speed = 0;
		unknown119 = 0;
		increase_ground_speed = 0;
		increase_air_speed = 0;
		unknown122 = 0;
		unknown123 = 0;
		unknown124 = 0;
		in_combat_movement_speed = 0;
		haste = 0;
		increase_max_power = 0;
		increase_max_power2 = 0;
		unknown128 = 0;
		drunk = 0;
		unknown129 = 0;
		hate_mod = 0;
		adventure_effects_bonus = 0;
		tradeskill_effects_bonus = 0;
		unknown130 = 0;
		unknown131 = 0;
		dps = 0;
		unknown132 = 0;
		unknown133 = 0;
		unknown134 = 0;
		base_avoidance = 0;
		unknown136 = 0;
		uncontested_riposte_pve = 0;
		uncontested_parry_pve = 0;
		unknown139 = 0;
		melee_ae = 0;
		multi_attack = 0;
		spell_multi_attack = 0;
		flurry = 0;
		unknown140 = 0;
		bountiful_harvest = 0;
		block_chance = 0;
		unknown141 = 0;
		unknown142 = 0;
		crit_chance = 0;
		unknown143 = 0;
		unknown144 = 0;
		crit_bonus = 0;
		potency = 0;
		unknown145 = 0;
		reuse_speed = 0;
		recovery_speed = 0;
		casting_speed = 0;
		spell_reuse_speed = 0;
		unknown146 = 0;
		unknown147 = 0;
		decrease_falling_dmg = 0;
		unknown149 = 0;
		unknown150 = 0;
		unknown151 = 0;
		unknown152 = 0;
		unknown153 = 0;
		unknown154 = 0;
		unknown155 = 0;
		unknown156 = 0;
		loot_coin = 0;
		mitigation_increase = 0;
		unknown159 = 0;
		strikethrough = 0;
		unknown160 = 0;
		accuracy = 0;
		unknown161 = 0;
		unknown162 = 0;
		unknown163 = 0;
		unknown164 = 0;
		unknown165 = 0;
		unknown166 = 0;
		unknown167 = 0;
		unknown168 = 0;
		lethality_pve = 0;
		unknown170 = 0;
		unknown171 = 0;
		unknown172 = 0;
		unknown173 = 0;
		pvp_crit_mit = 0;
		unknown174 = 0;
		unknown175 = 0;
		unknown200 = 0;
		unknown201 = 0;
		unknown202 = 0;
		unknown203 = 0;
		unknown204 = 0;
		unknown205 = 0;
		unknown206 = 0;
		unknown207 = 0;
		unknown208 = 0;
		unknown209 = 0;
		unknown210 = 0;
		unknown211 = 0;
		unknown212 = 0;
		unknown213 = 0;
		unknown214 = 0;
		unknown215 = 0;
		unknown216 = 0;
		unknown217 = 0;
		unknown218 = 0;
		unknown219 = 0;
		unknown220 = 0;
		unknown221 = 0;
		unknown222 = 0;
		unknown223 = 0;
		unknown224 = 0;
		unknown225 = 0;
		unknown226 = 0;
		unknown227 = 0;
		unknown228 = 0;
		unknown229 = 0;
		unknown230 = 0;
		unknown231 = 0;
		unknown232 = 0;
		unknown233 = 0;
		unknown234 = 0;
		unknown235 = 0;
		unknown236 = 0;
		unknown237 = 0;
		unknown238 = 0;
		unknown239 = 0;
		unknown240 = 0;
		unknown241 = 0;
		unknown242 = 0;
		uncontested_riposte_gear_cap = 0;
		uncontested_dodge_gear_cap = 0;
		uncontested_parry_gear_cap = 0;
		unknown246 = 0;
		unknown247 = 0;
		unknown248 = 0;
		unknown249 = 0;
		unknown250 = 0;
		unknown251 = 0;
		unknown252 = 0;
		unknown253 = 0;
		unknown254 = 0;
		unknown255 = 0;
		unknown256 = 0;
		unknown257 = 0;
		unknown258 = 0;
		unknown259 = 0;
		unknown260 = 0;
		unknown261 = 0;
		unknown262 = 0;
		unknown263 = 0;
		unknown264 = 0;
		unknown265 = 0;
		unknown266 = 0;
		unknown267 = 0;
		unknown268 = 0;
		unknown269 = 0;
		unknown270 = 0;
		unknown271 = 0;
		unknown272 = 0;
		unknown273 = 0;
		unknown274 = 0;
		unknown275 = 0;
		unknown276 = 0;
		unknown277 = 0;
		unknown278 = 0;
		unknown279 = 0;
		unknown280 = 0;
		unknown281 = 0;
		unknown282 = 0;
		unknown283 = 0;
		unknown284 = 0;
		unknown285 = 0;
		unknown286 = 0;
		unknown287 = 0;
		unknown288 = 0;
		unknown289 = 0;
		unknown290 = 0;
		unknown291 = 0;
		unknown292 = 0;
		unknown293 = 0;
		unknown294 = 0;
		unknown295 = 0;
		unknown296 = 0;
		unknown297 = 0;
		unknown298 = 0;
		unknown299 = 0;
		unknown300 = 0;
		unknown301 = 0;
		unknown302 = 0;
		unknown303 = 0;
		unknown304 = 0;
		unknown305 = 0;
		unknown306 = 0;
		unknown307 = 0;
		unknown308 = 0;
		out_of_combat_health_regen_pvp = 0;
		out_of_combat_power_regen_pvp = 0;
		in_combat_regen_pvp = 0;
		deflect_pvp = 0;
		increase_max_heath_pvp = 0;
		increase_max_health2_pvp = 0;
		unknown312 = 0;
		increase_speed_pvp = 0;
		unknown313 = 0;
		increase_ground_speed_pvp = 0;
		increase_air_speed_pvp = 0;
		unknown314 = 0;
		unknown315 = 0;
		unknown316 = 0;
		in_combat_movement_speed_pvp = 0;
		Haste_pvp = 0;
		increase_max_power_pvp = 0;
		increase_max_power2_pvp = 0;
		unknown321 = 0;
		unknown322 = 0;
		unknown323 = 0;
		hate_mod_pvp = 0;
		unknown325 = 0;
		unknown326 = 0;
		unknown327 = 0;
		unknown328 = 0;
		dps_pvp = 0;
		unknown330 = 0;
		unknown331 = 0;
		unknown332 = 0;
		base_avoidance_pvp = 0;
		unknown334 = 0;
		uncontested_riposte_pvp = 0;
		uncontested_parry_pvp = 0;
		unknown337 = 0;
		unknown338 = 0;
		multi_attack_pvp = 0;
		spell_multi_attack_pvp = 0;
		unknown341 = 0;
		unknown342 = 0;
		unknown343 = 0;
		block_chance_pvp = 0;
		unknown345 = 0;
		unknown346 = 0;
		crit_chance_pvp = 0;
		unknown348 = 0;
		unknown349 = 0;
		crit_bonus_pvp = 0;
		potency_pvp = 0;
		unknown352 = 0;
		reuse_speed_pvp = 0;
		recovery_speed_pvp = 0;
		casting_speed_pvp = 0;
		unknown356 = 0;
		unknown357 = 0;
		unknown358 = 0;
		decrease_falling_dmg_pvp = 0;
		unknown360 = 0;
		unknown361 = 0;
		unknown362 = 0;
		unknown363 = 0;
		unknown364 = 0;
		unknown365 = 0;
		unknown366 = 0;
		unknown367 = 0;
		loot_coin_pvp = 0;
		mitigation_increase_pvp = 0;
		unknown370 = 0;
		strikethrough_pvp = 0;
		unknown372 = 0;
		unknown373 = 0;
		unknown374 = 0;
		unknown375 = 0;
		unknown376 = 0;
		unknown377 = 0;
		unknown378 = 0;
		unknown379 = 0;
		unknown380 = 0;
		unknown381 = 0;
		lethality_pvp = 0;
		unknown383 = 0;
		unknown384 = 0;
		unknown385 = 0;
		unknown386 = 0;
		unknown387 = 0;
		unknown388 = 0;
		unknown389 = 0;
		unknown390 = 0;
		unknown391 = 0;
		unknown392 = 0;
		unknown393 = 0;
		unknown394 = 0;
		unknown395 = 0;
		unknown396 = 0;
		unknown397 = 0;
		unknown398 = 0;
		durability_mod = 0;
		durability_add = 0;
		progress_mod = 0;
		progress_add = 0;
		success_mod = 0;
		crit_success_mod = 0;
		unknown177 = 0;
		rare_harvest_chance = 0;
		max_crafting = 0;
		component_refund = 0;
		unknown178 = 0;
		ex_durability_mod = 0;
		ex_durability_add = 0;
		ex_crit_success_mod = 0;
		ex_crit_failure_mod = 0;
		ex_progress_mod = 0;
		ex_progress_add = 0;
		ex_success_mod = 0;
		unknown417 = 0;
		unknown418 = 0;
		unknown419 = 0;
		unknown420 = 0;
		unknown421 = 0;
		unknown422 = 0;
		unknown423 = 0;
		unknown424 = 0;
		unknown425 = 0;
		unknown426 = 0;
		unknown427 = 0;
		unknown428 = 0;
		unknown429 = 0;
		unknown430 = 0;
		unknown431 = 0;
		unknown432 = 0;
		unknown433 = 0;
		unknown434 = 0;
		unknown435 = 0;
		unknown436 = 0;
		unknown437 = 0;
		unknown438 = 0;
		unknown439 = 0;
		unknown440 = 0;
		unknown441 = 0;
		unknown180 = 0;
		unknown524 = 0;
		unknown181a = 0;
		unknown181b = 0;
		unknown442 = 0;
		unknown443 = 0;
		unknown444 = 0;
		unknown445 = 0;
		unknown446 = 0;
		unknown447 = 0;
		unknown448 = 0;
		unknown449 = 0;
		unknown450 = 0;
		unknown451 = 0;
		unknown452 = 0;
		unknown453 = 0;
		unknown454 = 0;
		unknown455 = 0;
		unknown456 = 0;
		unknown457 = 0;
		unknown458 = 0;
		unknown459 = 0;
		unknown460 = 0;
		unknown461 = 0;
		unknown462 = 0;
		unknown463 = 0;
		unknown464 = 0;
		unknown465 = 0;
		unknown466 = 0;
		unknown467 = 0;
		unknown468 = 0;
		unknown469 = 0;
		unknown470 = 0;
		unknown471 = 0;
		unknown472 = 0;
		unknown473 = 0;
		unknown474 = 0;
		unknown475 = 0;
		unknown476 = 0;
		unknown477 = 0;
		unknown478 = 0;
		unknown479 = 0;
		unknown480 = 0;
		unknown481 = 0;
		unknown482 = 0;
		unknown483 = 0;
		unknown484 = 0;
		unknown485 = 0;
		unknown486 = 0;
		unknown487 = 0;
		unknown488 = 0;
		unknown489 = 0;
		unknown490 = 0;
		unknown491 = 0;
		unknown492 = 0;
		unknown493 = 0;
		unknown494 = 0;
		unknown495 = 0;
		unknown496 = 0;
		unknown497 = 0;
		unknown498 = 0;
		unknown499 = 0;
		unknown500 = 0;
		unknown501 = 0;
		unknown502 = 0;
		unknown503 = 0;
		unknown504 = 0;
		unknown505 = 0;
		unknown506 = 0;
		unknown507 = 0;
		unknown508 = 0;
		unknown509 = 0;
		unknown510 = 0;
		unknown511 = 0;
		unknown512 = 0;
		unknown513 = 0;
		unknown514 = 0;
		haste_tt = 0;
		dps_pve_tt = 0;
		dps_pvp_tt = 0;
		multi_attack_pve_tt = 0;
		multi_attack_pvp_tt = 0;
		unknown520 = 0;
		unknown521 = 0;
		unknown522 = 0;
		unknown523 = 0;
		unknown550 = 0;
		vision = 0;
		unknown551 = 0;
		memset(unknown525, 0, sizeof(unknown525));
		unknown182 = 0;
		memset(unknown183, 0, sizeof(unknown183));
		pet_id = 0;
		memset(pet_name, 0, sizeof(pet_name));
		memset(unknown184, 0, sizeof(unknown184));
		pet_health_pct = 0;
		pet_power_pct = 0;
		unknown185 = 0;
		pet_movement = 0;
		pet_behavior = 0;
		memset(unknown186, 0, sizeof(unknown186));
		merc_id = 0;
		memset(merc_name, 0, sizeof(merc_name));
		memset(merc_unknown21, 0, sizeof(merc_unknown21));
		merc_health_pct = 0;
		merc_power_pct = 0;
		merc_unknown21b = 0;
		merc_movement = 0;
		merc_behavior = 0;
		memset(merc_unknown21c, 0, sizeof(merc_unknown21c));
		group_leader_id = 0;
		memset(merc_unknown21cc, 0, sizeof(merc_unknown21cc));
		rain = 0;
		rain2 = 0;
		memset(unknown527, 0, sizeof(unknown527));
		memset(house_zone, 0, sizeof(house_zone));
		memset(unknown526, 0, sizeof(unknown526));
		status_points = 0;
		guild_status = 0;
		memset(unknown187, 0, sizeof(unknown187));
		// Error parsing the struct, XMLStructConverter.Element is an unknown type
		memset(bind_zone, 0, sizeof(bind_zone));
		memset(unknown188, 0, sizeof(unknown188));
	}

	char character_name[40];
	uint16_t unknown_1_1_MJ;
	uint8_t race;
	uint8_t gender;
	uint8_t exiled;
	uint32_t class1;
	uint32_t class2;
	uint32_t class3;
	uint32_t tradeskill_class1;
	uint32_t tradeskill_class2;
	uint32_t tradeskill_class3;
	uint16_t level;
	uint16_t effective_level;
	uint16_t tradeskill_level;
	uint32_t unknown_1_2_MJ;
	uint16_t account_age_base;
	uint16_t account_age_bonus;
	char deity[32];
	char last_name[20];
	uint8_t unknown3;
	char character_name2[40];
	uint16_t character_name2_unknown;
	char character_name3[40];
	uint16_t character_name3_unknown;
	int64_t current_hp;
	uint64_t max_hp;
	uint32_t base_hp;
	uint32_t base_hp2;
	int32_t current_power;
	int32_t max_power;
	uint32_t base_power;
	uint8_t conc_used;
	uint8_t conc_max;
	int32_t savagery;
	int32_t max_savagery;
	uint32_t unknown4b;
	uint32_t savagery_level;
	uint32_t max_savagery_level;
	uint8_t unknown4c[4];
	int32_t dissonance;
	int32_t max_dissonance;
	uint8_t unknown5c[4];
	uint32_t hp_regen;
	uint32_t power_regen;
	uint32_t unknown6[2];
	float unknown7[2];
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
	uint16_t unknown10;
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
	uint32_t str;
	uint32_t sta;
	uint32_t agi;
	uint32_t wis;
	uint32_t intel;
	uint32_t str_base;
	uint32_t sta_base;
	uint32_t agi_base;
	uint32_t wis_base;
	uint32_t int_base;
	uint32_t mitigation_cur;
	uint32_t elemental;
	uint32_t noxious;
	uint32_t arcane;
	uint32_t mitigation_base;
	uint32_t elemental_base;
	uint32_t noxious_base;
	uint32_t arcane_base;
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
	float current_adv_xp;
	float needed_adv_xp;
	float debt_adv_xp;
	float current_trade_xp;
	float needed_trade_xp;
	float debt_trade_xp;
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
	uint32_t coins_copper;
	uint32_t coins_silver;
	uint32_t coins_gold;
	uint32_t coins_plat;
	uint32_t unknown34[5];
	Substruct_SpellEffects spell_effects[45];
	Substruct_SpellEffects detrimental_spell_effects[45];
	Substruct_PassiveEffects passive_spell_effects[100];
	uint8_t unknown35[486];
	uint8_t trauma_count;
	uint8_t arcane_count;
	uint8_t noxious_count;
	uint8_t elemental_count;
	uint8_t curse_count;
	Substruct_MaintainedEffects maintained_effects[30];
	float breath;
	uint8_t breathe_underwater;
	float unknown36;
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
	uint32_t flags;
	uint32_t flags2;
	float unknown45;
	float unknown46;
	float unknown47;
	float unknown48;
	float unknown49;
	float unknown50;
	float unknown51;
	float unknown52;
	float unknown53;
	float unknown54;
	float unknown55;
	float unknown56;
	float unknown57;
	float unknown58;
	float unknown59;
	float unknown60;
	float unknown61;
	float unknown62;
	float unknown63;
	float unknown64;
	float unknown65;
	float unknown66;
	float unknown67;
	float unknown68;
	float unknown69;
	float unknown70;
	float unknown71;
	float unknown72;
	float unknown73;
	float unknown74;
	float unknown75;
	float unknown76;
	float unknown77;
	float unknown78;
	float unknown79;
	float unknown80;
	float unknown81;
	float unknown82;
	float unknown83;
	float unknown84;
	float unknown85;
	float unknown86;
	float unknown87;
	float unknown88;
	float unknown89;
	float unknown90;
	float unknown91;
	float unknown92;
	float unknown93;
	float unknown94;
	float unknown95;
	float unknown96;
	float unknown97;
	float unknown98;
	float unknown99;
	float unknown100;
	float unknown101;
	float unknown102;
	float unknown103;
	float unknown104;
	float unknown105;
	float unknown106;
	float unknown107;
	float unknown108;
	float unknown109;
	float unknown110;
	float out_of_combat_health_regen;
	float out_of_combat_power_regen;
	float in_combat_regen;
	float deflect;
	float increase_max_health;
	float increase_max_health2;
	float unknown117;
	float increase_speed;
	float unknown119;
	float increase_ground_speed;
	float increase_air_speed;
	float unknown122;
	float unknown123;
	float unknown124;
	float in_combat_movement_speed;
	float haste;
	float increase_max_power;
	float increase_max_power2;
	float unknown128;
	float drunk;
	float unknown129;
	float hate_mod;
	float adventure_effects_bonus;
	float tradeskill_effects_bonus;
	float unknown130;
	float unknown131;
	float dps;
	float unknown132;
	float unknown133;
	float unknown134;
	float base_avoidance;
	float unknown136;
	float uncontested_riposte_pve;
	float uncontested_parry_pve;
	float unknown139;
	float melee_ae;
	float multi_attack;
	float spell_multi_attack;
	float flurry;
	float unknown140;
	float bountiful_harvest;
	float block_chance;
	float unknown141;
	float unknown142;
	float crit_chance;
	float unknown143;
	float unknown144;
	float crit_bonus;
	float potency;
	float unknown145;
	float reuse_speed;
	float recovery_speed;
	float casting_speed;
	float spell_reuse_speed;
	float unknown146;
	float unknown147;
	float decrease_falling_dmg;
	float unknown149;
	float unknown150;
	float unknown151;
	float unknown152;
	float unknown153;
	float unknown154;
	float unknown155;
	float unknown156;
	float loot_coin;
	float mitigation_increase;
	float unknown159;
	float strikethrough;
	float unknown160;
	float accuracy;
	float unknown161;
	float unknown162;
	float unknown163;
	float unknown164;
	float unknown165;
	float unknown166;
	float unknown167;
	float unknown168;
	float lethality_pve;
	float unknown170;
	float unknown171;
	float unknown172;
	float unknown173;
	float pvp_crit_mit;
	float unknown174;
	float unknown175;
	float unknown200;
	float unknown201;
	float unknown202;
	float unknown203;
	float unknown204;
	float unknown205;
	float unknown206;
	float unknown207;
	float unknown208;
	float unknown209;
	float unknown210;
	float unknown211;
	float unknown212;
	float unknown213;
	float unknown214;
	float unknown215;
	float unknown216;
	float unknown217;
	float unknown218;
	float unknown219;
	float unknown220;
	float unknown221;
	float unknown222;
	float unknown223;
	float unknown224;
	float unknown225;
	float unknown226;
	float unknown227;
	float unknown228;
	float unknown229;
	float unknown230;
	float unknown231;
	float unknown232;
	float unknown233;
	float unknown234;
	float unknown235;
	float unknown236;
	float unknown237;
	float unknown238;
	float unknown239;
	float unknown240;
	float unknown241;
	float unknown242;
	float uncontested_riposte_gear_cap;
	float uncontested_dodge_gear_cap;
	float uncontested_parry_gear_cap;
	float unknown246;
	float unknown247;
	float unknown248;
	float unknown249;
	float unknown250;
	float unknown251;
	float unknown252;
	float unknown253;
	float unknown254;
	float unknown255;
	float unknown256;
	float unknown257;
	float unknown258;
	float unknown259;
	float unknown260;
	float unknown261;
	float unknown262;
	float unknown263;
	float unknown264;
	float unknown265;
	float unknown266;
	float unknown267;
	float unknown268;
	float unknown269;
	float unknown270;
	float unknown271;
	float unknown272;
	float unknown273;
	float unknown274;
	float unknown275;
	float unknown276;
	float unknown277;
	float unknown278;
	float unknown279;
	float unknown280;
	float unknown281;
	float unknown282;
	float unknown283;
	float unknown284;
	float unknown285;
	float unknown286;
	float unknown287;
	float unknown288;
	float unknown289;
	float unknown290;
	float unknown291;
	float unknown292;
	float unknown293;
	float unknown294;
	float unknown295;
	float unknown296;
	float unknown297;
	float unknown298;
	float unknown299;
	float unknown300;
	float unknown301;
	float unknown302;
	float unknown303;
	float unknown304;
	float unknown305;
	float unknown306;
	float unknown307;
	float unknown308;
	float out_of_combat_health_regen_pvp;
	float out_of_combat_power_regen_pvp;
	float in_combat_regen_pvp;
	float deflect_pvp;
	float increase_max_heath_pvp;
	float increase_max_health2_pvp;
	float unknown312;
	float increase_speed_pvp;
	float unknown313;
	float increase_ground_speed_pvp;
	float increase_air_speed_pvp;
	float unknown314;
	float unknown315;
	float unknown316;
	float in_combat_movement_speed_pvp;
	float Haste_pvp;
	float increase_max_power_pvp;
	float increase_max_power2_pvp;
	float unknown321;
	float unknown322;
	float unknown323;
	float hate_mod_pvp;
	float unknown325;
	float unknown326;
	float unknown327;
	float unknown328;
	float dps_pvp;
	float unknown330;
	float unknown331;
	float unknown332;
	float base_avoidance_pvp;
	float unknown334;
	float uncontested_riposte_pvp;
	float uncontested_parry_pvp;
	float unknown337;
	float unknown338;
	float multi_attack_pvp;
	float spell_multi_attack_pvp;
	float unknown341;
	float unknown342;
	float unknown343;
	float block_chance_pvp;
	float unknown345;
	float unknown346;
	float crit_chance_pvp;
	float unknown348;
	float unknown349;
	float crit_bonus_pvp;
	float potency_pvp;
	float unknown352;
	float reuse_speed_pvp;
	float recovery_speed_pvp;
	float casting_speed_pvp;
	float unknown356;
	float unknown357;
	float unknown358;
	float decrease_falling_dmg_pvp;
	float unknown360;
	float unknown361;
	float unknown362;
	float unknown363;
	float unknown364;
	float unknown365;
	float unknown366;
	float unknown367;
	float loot_coin_pvp;
	float mitigation_increase_pvp;
	float unknown370;
	float strikethrough_pvp;
	float unknown372;
	float unknown373;
	float unknown374;
	float unknown375;
	float unknown376;
	float unknown377;
	float unknown378;
	float unknown379;
	float unknown380;
	float unknown381;
	float lethality_pvp;
	float unknown383;
	float unknown384;
	float unknown385;
	float unknown386;
	float unknown387;
	float unknown388;
	float unknown389;
	float unknown390;
	float unknown391;
	float unknown392;
	float unknown393;
	float unknown394;
	float unknown395;
	float unknown396;
	float unknown397;
	float unknown398;
	float durability_mod;
	float durability_add;
	float progress_mod;
	float progress_add;
	float success_mod;
	float crit_success_mod;
	float unknown177;
	float rare_harvest_chance;
	float max_crafting;
	float component_refund;
	float unknown178;
	float ex_durability_mod;
	float ex_durability_add;
	float ex_crit_success_mod;
	float ex_crit_failure_mod;
	float ex_progress_mod;
	float ex_progress_add;
	float ex_success_mod;
	float unknown417;
	float unknown418;
	float unknown419;
	float unknown420;
	float unknown421;
	float unknown422;
	float unknown423;
	float unknown424;
	float unknown425;
	float unknown426;
	float unknown427;
	float unknown428;
	float unknown429;
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
	float unknown446;
	float unknown447;
	float unknown448;
	float unknown449;
	float unknown450;
	float unknown451;
	float unknown452;
	float unknown453;
	float unknown454;
	float unknown455;
	float unknown456;
	float unknown457;
	float unknown458;
	float unknown459;
	float unknown460;
	float unknown461;
	float unknown462;
	float unknown463;
	float unknown464;
	float unknown465;
	float unknown466;
	float unknown467;
	float unknown468;
	float unknown469;
	float unknown470;
	float unknown471;
	float unknown472;
	float unknown473;
	float unknown474;
	float unknown475;
	float unknown476;
	float unknown477;
	float unknown478;
	float unknown479;
	float unknown480;
	float unknown481;
	float unknown482;
	float unknown483;
	float unknown484;
	float unknown485;
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
	float unknown500;
	float unknown501;
	float unknown502;
	float unknown503;
	float unknown504;
	float unknown505;
	float unknown506;
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
	uint16_t vision;
	uint8_t unknown551;
	uint8_t unknown525[1062];
	Substruct_GroupSheet groupSheet;
	uint16_t unknown182;
	uint8_t unknown183[462];
	uint32_t pet_id;
	char pet_name[32];
	uint8_t unknown184[9];
	float pet_health_pct;
	float pet_power_pct;
	uint8_t unknown185;
	uint8_t pet_movement;
	uint8_t pet_behavior;
	uint8_t unknown186[8];
	uint32_t merc_id;
	char merc_name[32];
	uint8_t merc_unknown21[9];
	float merc_health_pct;
	float merc_power_pct;
	uint8_t merc_unknown21b;
	uint8_t merc_movement;
	uint8_t merc_behavior;
	uint8_t merc_unknown21c[73];
	uint32_t group_leader_id;
	uint8_t merc_unknown21cc[4];
	float rain;
	float rain2;
	uint8_t unknown527[9];
	char house_zone[48];
	uint8_t unknown526[132];
	uint32_t status_points;
	uint32_t guild_status;
	uint8_t unknown187[11];
	char bind_zone[32];
	uint8_t unknown188[52];

private:
	void RegisterElements() {
		char& char_name = character_name[0];
		RegisterChar(char_name)->SetCount(40);
		RegisterUInt16(unknown_1_1_MJ);
		RegisterUInt8(race);
		RegisterUInt8(gender);
		RegisterUInt8(exiled);
		RegisterUInt32(class1);
		RegisterUInt32(class2);
		RegisterUInt32(class3);
		RegisterUInt32(tradeskill_class1);
		RegisterUInt32(tradeskill_class2);
		RegisterUInt32(tradeskill_class3);
		RegisterUInt16(level);
		RegisterUInt16(effective_level);
		RegisterUInt16(tradeskill_level);
		RegisterUInt32(unknown_1_2_MJ);
		RegisterUInt16(account_age_base);
		RegisterUInt16(account_age_bonus);
		char& Deity = deity[0];
		RegisterChar(Deity)->SetCount(32);
		char& Last_name = last_name[0];
		RegisterChar(Last_name)->SetCount(20);
		RegisterUInt8(unknown3);
		char& char_name2 = character_name2[0];
		RegisterChar(char_name2)->SetCount(40);
		RegisterUInt16(character_name2_unknown);
		char& char_name3 = character_name3[0];
		RegisterChar(char_name3)->SetCount(40);
		RegisterUInt16(character_name3_unknown);
		RegisterInt64(current_hp);
		RegisterUInt64(max_hp);
		RegisterUInt32(base_hp);
		RegisterUInt32(base_hp2);
		RegisterInt32(current_power);
		RegisterInt32(max_power);
		RegisterUInt32(base_power);
		RegisterUInt8(conc_used);
		RegisterUInt8(conc_max);
		RegisterInt32(savagery);
		RegisterInt32(max_savagery);
		RegisterUInt32(unknown4b);
		RegisterUInt32(savagery_level);
		RegisterUInt32(max_savagery_level);

		
		RescopeArrayElement(unknown4c);
		RegisterUInt8(unknown4c)->SetCount(4);
		

		RegisterInt32(dissonance);
		RegisterInt32(max_dissonance);

		uint8_t& Unknown5c = unknown5c[0];
		RegisterUInt8(Unknown5c)->SetCount(4);
		
		RegisterUInt32(hp_regen);
		RegisterUInt32(power_regen);

		uint32_t& Unknown6 = unknown6[0];
		RegisterUInt32(Unknown6)->SetCount(2);
		
		float& Unknown7 = unknown7[0];
		RegisterFloat(Unknown7)->SetCount(2);
		
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
		RegisterUInt16(unknown10);
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
		RegisterUInt32(str);
		RegisterUInt32(sta);
		RegisterUInt32(agi);
		RegisterUInt32(wis);
		RegisterUInt32(intel);
		RegisterUInt32(str_base);
		RegisterUInt32(sta_base);
		RegisterUInt32(agi_base);
		RegisterUInt32(wis_base);
		RegisterUInt32(int_base);
		RegisterUInt32(mitigation_cur);
		RegisterUInt32(elemental);
		RegisterUInt32(noxious);
		RegisterUInt32(arcane);
		RegisterUInt32(mitigation_base);
		RegisterUInt32(elemental_base);
		RegisterUInt32(noxious_base);
		RegisterUInt32(arcane_base);
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
		RegisterFloat(current_adv_xp);
		RegisterFloat(needed_adv_xp);
		RegisterFloat(debt_adv_xp);
		RegisterFloat(current_trade_xp);
		RegisterFloat(needed_trade_xp);
		RegisterFloat(debt_trade_xp);

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

		// Error parsing the struct, unknown is an unknown type(passive_spell_effects);
		RescopeArrayElement(passive_spell_effects);
		RegisterSubstruct(passive_spell_effects)->SetCount(100);

		uint8_t& Unknown35 = unknown35[0];
		RegisterUInt8(Unknown35)->SetCount(486);

		RegisterUInt8(trauma_count);
		RegisterUInt8(arcane_count);
		RegisterUInt8(noxious_count);
		RegisterUInt8(elemental_count);
		RegisterUInt8(curse_count);

		// Error parsing the struct, unknown is an unknown type(maintained_effects);
		RescopeArrayElement(maintained_effects);
		RegisterSubstruct(maintained_effects)->SetCount(30);

		RegisterFloat(breath);
		RegisterUInt8(breathe_underwater);
		RegisterFloat(unknown36);
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
		RegisterFloat(unknown45);
		RegisterFloat(unknown46);
		RegisterFloat(unknown47);
		RegisterFloat(unknown48);
		RegisterFloat(unknown49);
		RegisterFloat(unknown50);
		RegisterFloat(unknown51);
		RegisterFloat(unknown52);
		RegisterFloat(unknown53);
		RegisterFloat(unknown54);
		RegisterFloat(unknown55);
		RegisterFloat(unknown56);
		RegisterFloat(unknown57);
		RegisterFloat(unknown58);
		RegisterFloat(unknown59);
		RegisterFloat(unknown60);
		RegisterFloat(unknown61);
		RegisterFloat(unknown62);
		RegisterFloat(unknown63);
		RegisterFloat(unknown64);
		RegisterFloat(unknown65);
		RegisterFloat(unknown66);
		RegisterFloat(unknown67);
		RegisterFloat(unknown68);
		RegisterFloat(unknown69);
		RegisterFloat(unknown70);
		RegisterFloat(unknown71);
		RegisterFloat(unknown72);
		RegisterFloat(unknown73);
		RegisterFloat(unknown74);
		RegisterFloat(unknown75);
		RegisterFloat(unknown76);
		RegisterFloat(unknown77);
		RegisterFloat(unknown78);
		RegisterFloat(unknown79);
		RegisterFloat(unknown80);
		RegisterFloat(unknown81);
		RegisterFloat(unknown82);
		RegisterFloat(unknown83);
		RegisterFloat(unknown84);
		RegisterFloat(unknown85);
		RegisterFloat(unknown86);
		RegisterFloat(unknown87);
		RegisterFloat(unknown88);
		RegisterFloat(unknown89);
		RegisterFloat(unknown90);
		RegisterFloat(unknown91);
		RegisterFloat(unknown92);
		RegisterFloat(unknown93);
		RegisterFloat(unknown94);
		RegisterFloat(unknown95);
		RegisterFloat(unknown96);
		RegisterFloat(unknown97);
		RegisterFloat(unknown98);
		RegisterFloat(unknown99);
		RegisterFloat(unknown100);
		RegisterFloat(unknown101);
		RegisterFloat(unknown102);
		RegisterFloat(unknown103);
		RegisterFloat(unknown104);
		RegisterFloat(unknown105);
		RegisterFloat(unknown106);
		RegisterFloat(unknown107);
		RegisterFloat(unknown108);
		RegisterFloat(unknown109);
		RegisterFloat(unknown110);
		RegisterFloat(out_of_combat_health_regen);
		RegisterFloat(out_of_combat_power_regen);
		RegisterFloat(in_combat_regen);
		RegisterFloat(deflect);
		RegisterFloat(increase_max_health);
		RegisterFloat(increase_max_health2);
		RegisterFloat(unknown117);
		RegisterFloat(increase_speed);
		RegisterFloat(unknown119);
		RegisterFloat(increase_ground_speed);
		RegisterFloat(increase_air_speed);
		RegisterFloat(unknown122);
		RegisterFloat(unknown123);
		RegisterFloat(unknown124);
		RegisterFloat(in_combat_movement_speed);
		RegisterFloat(haste);
		RegisterFloat(increase_max_power);
		RegisterFloat(increase_max_power2);
		RegisterFloat(unknown128);
		RegisterFloat(drunk);
		RegisterFloat(unknown129);
		RegisterFloat(hate_mod);
		RegisterFloat(adventure_effects_bonus);
		RegisterFloat(tradeskill_effects_bonus);
		RegisterFloat(unknown130);
		RegisterFloat(unknown131);
		RegisterFloat(dps);
		RegisterFloat(unknown132);
		RegisterFloat(unknown133);
		RegisterFloat(unknown134);
		RegisterFloat(base_avoidance);
		RegisterFloat(unknown136);
		RegisterFloat(uncontested_riposte_pve);
		RegisterFloat(uncontested_parry_pve);
		RegisterFloat(unknown139);
		RegisterFloat(melee_ae);
		RegisterFloat(multi_attack);
		RegisterFloat(spell_multi_attack);
		RegisterFloat(flurry);
		RegisterFloat(unknown140);
		RegisterFloat(bountiful_harvest);
		RegisterFloat(block_chance);
		RegisterFloat(unknown141);
		RegisterFloat(unknown142);
		RegisterFloat(crit_chance);
		RegisterFloat(unknown143);
		RegisterFloat(unknown144);
		RegisterFloat(crit_bonus);
		RegisterFloat(potency);
		RegisterFloat(unknown145);
		RegisterFloat(reuse_speed);
		RegisterFloat(recovery_speed);
		RegisterFloat(casting_speed);
		RegisterFloat(spell_reuse_speed);
		RegisterFloat(unknown146);
		RegisterFloat(unknown147);
		RegisterFloat(decrease_falling_dmg);
		RegisterFloat(unknown149);
		RegisterFloat(unknown150);
		RegisterFloat(unknown151);
		RegisterFloat(unknown152);
		RegisterFloat(unknown153);
		RegisterFloat(unknown154);
		RegisterFloat(unknown155);
		RegisterFloat(unknown156);
		RegisterFloat(loot_coin);
		RegisterFloat(mitigation_increase);
		RegisterFloat(unknown159);
		RegisterFloat(strikethrough);
		RegisterFloat(unknown160);
		RegisterFloat(accuracy);
		RegisterFloat(unknown161);
		RegisterFloat(unknown162);
		RegisterFloat(unknown163);
		RegisterFloat(unknown164);
		RegisterFloat(unknown165);
		RegisterFloat(unknown166);
		RegisterFloat(unknown167);
		RegisterFloat(unknown168);
		RegisterFloat(lethality_pve);
		RegisterFloat(unknown170);
		RegisterFloat(unknown171);
		RegisterFloat(unknown172);
		RegisterFloat(unknown173);
		RegisterFloat(pvp_crit_mit);
		RegisterFloat(unknown174);
		RegisterFloat(unknown175);
		RegisterFloat(unknown200);
		RegisterFloat(unknown201);
		RegisterFloat(unknown202);
		RegisterFloat(unknown203);
		RegisterFloat(unknown204);
		RegisterFloat(unknown205);
		RegisterFloat(unknown206);
		RegisterFloat(unknown207);
		RegisterFloat(unknown208);
		RegisterFloat(unknown209);
		RegisterFloat(unknown210);
		RegisterFloat(unknown211);
		RegisterFloat(unknown212);
		RegisterFloat(unknown213);
		RegisterFloat(unknown214);
		RegisterFloat(unknown215);
		RegisterFloat(unknown216);
		RegisterFloat(unknown217);
		RegisterFloat(unknown218);
		RegisterFloat(unknown219);
		RegisterFloat(unknown220);
		RegisterFloat(unknown221);
		RegisterFloat(unknown222);
		RegisterFloat(unknown223);
		RegisterFloat(unknown224);
		RegisterFloat(unknown225);
		RegisterFloat(unknown226);
		RegisterFloat(unknown227);
		RegisterFloat(unknown228);
		RegisterFloat(unknown229);
		RegisterFloat(unknown230);
		RegisterFloat(unknown231);
		RegisterFloat(unknown232);
		RegisterFloat(unknown233);
		RegisterFloat(unknown234);
		RegisterFloat(unknown235);
		RegisterFloat(unknown236);
		RegisterFloat(unknown237);
		RegisterFloat(unknown238);
		RegisterFloat(unknown239);
		RegisterFloat(unknown240);
		RegisterFloat(unknown241);
		RegisterFloat(unknown242);
		RegisterFloat(uncontested_riposte_gear_cap);
		RegisterFloat(uncontested_dodge_gear_cap);
		RegisterFloat(uncontested_parry_gear_cap);
		RegisterFloat(unknown246);
		RegisterFloat(unknown247);
		RegisterFloat(unknown248);
		RegisterFloat(unknown249);
		RegisterFloat(unknown250);
		RegisterFloat(unknown251);
		RegisterFloat(unknown252);
		RegisterFloat(unknown253);
		RegisterFloat(unknown254);
		RegisterFloat(unknown255);
		RegisterFloat(unknown256);
		RegisterFloat(unknown257);
		RegisterFloat(unknown258);
		RegisterFloat(unknown259);
		RegisterFloat(unknown260);
		RegisterFloat(unknown261);
		RegisterFloat(unknown262);
		RegisterFloat(unknown263);
		RegisterFloat(unknown264);
		RegisterFloat(unknown265);
		RegisterFloat(unknown266);
		RegisterFloat(unknown267);
		RegisterFloat(unknown268);
		RegisterFloat(unknown269);
		RegisterFloat(unknown270);
		RegisterFloat(unknown271);
		RegisterFloat(unknown272);
		RegisterFloat(unknown273);
		RegisterFloat(unknown274);
		RegisterFloat(unknown275);
		RegisterFloat(unknown276);
		RegisterFloat(unknown277);
		RegisterFloat(unknown278);
		RegisterFloat(unknown279);
		RegisterFloat(unknown280);
		RegisterFloat(unknown281);
		RegisterFloat(unknown282);
		RegisterFloat(unknown283);
		RegisterFloat(unknown284);
		RegisterFloat(unknown285);
		RegisterFloat(unknown286);
		RegisterFloat(unknown287);
		RegisterFloat(unknown288);
		RegisterFloat(unknown289);
		RegisterFloat(unknown290);
		RegisterFloat(unknown291);
		RegisterFloat(unknown292);
		RegisterFloat(unknown293);
		RegisterFloat(unknown294);
		RegisterFloat(unknown295);
		RegisterFloat(unknown296);
		RegisterFloat(unknown297);
		RegisterFloat(unknown298);
		RegisterFloat(unknown299);
		RegisterFloat(unknown300);
		RegisterFloat(unknown301);
		RegisterFloat(unknown302);
		RegisterFloat(unknown303);
		RegisterFloat(unknown304);
		RegisterFloat(unknown305);
		RegisterFloat(unknown306);
		RegisterFloat(unknown307);
		RegisterFloat(unknown308);
		RegisterFloat(out_of_combat_health_regen_pvp);
		RegisterFloat(out_of_combat_power_regen_pvp);
		RegisterFloat(in_combat_regen_pvp);
		RegisterFloat(deflect_pvp);
		RegisterFloat(increase_max_heath_pvp);
		RegisterFloat(increase_max_health2_pvp);
		RegisterFloat(unknown312);
		RegisterFloat(increase_speed_pvp);
		RegisterFloat(unknown313);
		RegisterFloat(increase_ground_speed_pvp);
		RegisterFloat(increase_air_speed_pvp);
		RegisterFloat(unknown314);
		RegisterFloat(unknown315);
		RegisterFloat(unknown316);
		RegisterFloat(in_combat_movement_speed_pvp);
		RegisterFloat(Haste_pvp);
		RegisterFloat(increase_max_power_pvp);
		RegisterFloat(increase_max_power2_pvp);
		RegisterFloat(unknown321);
		RegisterFloat(unknown322);
		RegisterFloat(unknown323);
		RegisterFloat(hate_mod_pvp);
		RegisterFloat(unknown325);
		RegisterFloat(unknown326);
		RegisterFloat(unknown327);
		RegisterFloat(unknown328);
		RegisterFloat(dps_pvp);
		RegisterFloat(unknown330);
		RegisterFloat(unknown331);
		RegisterFloat(unknown332);
		RegisterFloat(base_avoidance_pvp);
		RegisterFloat(unknown334);
		RegisterFloat(uncontested_riposte_pvp);
		RegisterFloat(uncontested_parry_pvp);
		RegisterFloat(unknown337);
		RegisterFloat(unknown338);
		RegisterFloat(multi_attack_pvp);
		RegisterFloat(spell_multi_attack_pvp);
		RegisterFloat(unknown341);
		RegisterFloat(unknown342);
		RegisterFloat(unknown343);
		RegisterFloat(block_chance_pvp);
		RegisterFloat(unknown345);
		RegisterFloat(unknown346);
		RegisterFloat(crit_chance_pvp);
		RegisterFloat(unknown348);
		RegisterFloat(unknown349);
		RegisterFloat(crit_bonus_pvp);
		RegisterFloat(potency_pvp);
		RegisterFloat(unknown352);
		RegisterFloat(reuse_speed_pvp);
		RegisterFloat(recovery_speed_pvp);
		RegisterFloat(casting_speed_pvp);
		RegisterFloat(unknown356);
		RegisterFloat(unknown357);
		RegisterFloat(unknown358);
		RegisterFloat(decrease_falling_dmg_pvp);
		RegisterFloat(unknown360);
		RegisterFloat(unknown361);
		RegisterFloat(unknown362);
		RegisterFloat(unknown363);
		RegisterFloat(unknown364);
		RegisterFloat(unknown365);
		RegisterFloat(unknown366);
		RegisterFloat(unknown367);
		RegisterFloat(loot_coin_pvp);
		RegisterFloat(mitigation_increase_pvp);
		RegisterFloat(unknown370);
		RegisterFloat(strikethrough_pvp);
		RegisterFloat(unknown372);
		RegisterFloat(unknown373);
		RegisterFloat(unknown374);
		RegisterFloat(unknown375);
		RegisterFloat(unknown376);
		RegisterFloat(unknown377);
		RegisterFloat(unknown378);
		RegisterFloat(unknown379);
		RegisterFloat(unknown380);
		RegisterFloat(unknown381);
		RegisterFloat(lethality_pvp);
		RegisterFloat(unknown383);
		RegisterFloat(unknown384);
		RegisterFloat(unknown385);
		RegisterFloat(unknown386);
		RegisterFloat(unknown387);
		RegisterFloat(unknown388);
		RegisterFloat(unknown389);
		RegisterFloat(unknown390);
		RegisterFloat(unknown391);
		RegisterFloat(unknown392);
		RegisterFloat(unknown393);
		RegisterFloat(unknown394);
		RegisterFloat(unknown395);
		RegisterFloat(unknown396);
		RegisterFloat(unknown397);
		RegisterFloat(unknown398);
		RegisterFloat(durability_mod);
		RegisterFloat(durability_add);
		RegisterFloat(progress_mod);
		RegisterFloat(progress_add);
		RegisterFloat(success_mod);
		RegisterFloat(crit_success_mod);
		RegisterFloat(unknown177);
		RegisterFloat(rare_harvest_chance);
		RegisterFloat(max_crafting);
		RegisterFloat(component_refund);
		RegisterFloat(unknown178);
		RegisterFloat(ex_durability_mod);
		RegisterFloat(ex_durability_add);
		RegisterFloat(ex_crit_success_mod);
		RegisterFloat(ex_crit_failure_mod);
		RegisterFloat(ex_progress_mod);
		RegisterFloat(ex_progress_add);
		RegisterFloat(ex_success_mod);
		RegisterFloat(unknown417);
		RegisterFloat(unknown418);
		RegisterFloat(unknown419);
		RegisterFloat(unknown420);
		RegisterFloat(unknown421);
		RegisterFloat(unknown422);
		RegisterFloat(unknown423);
		RegisterFloat(unknown424);
		RegisterFloat(unknown425);
		RegisterFloat(unknown426);
		RegisterFloat(unknown427);
		RegisterFloat(unknown428);
		RegisterFloat(unknown429);
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
		RegisterFloat(unknown446);
		RegisterFloat(unknown447);
		RegisterFloat(unknown448);
		RegisterFloat(unknown449);
		RegisterFloat(unknown450);
		RegisterFloat(unknown451);
		RegisterFloat(unknown452);
		RegisterFloat(unknown453);
		RegisterFloat(unknown454);
		RegisterFloat(unknown455);
		RegisterFloat(unknown456);
		RegisterFloat(unknown457);
		RegisterFloat(unknown458);
		RegisterFloat(unknown459);
		RegisterFloat(unknown460);
		RegisterFloat(unknown461);
		RegisterFloat(unknown462);
		RegisterFloat(unknown463);
		RegisterFloat(unknown464);
		RegisterFloat(unknown465);
		RegisterFloat(unknown466);
		RegisterFloat(unknown467);
		RegisterFloat(unknown468);
		RegisterFloat(unknown469);
		RegisterFloat(unknown470);
		RegisterFloat(unknown471);
		RegisterFloat(unknown472);
		RegisterFloat(unknown473);
		RegisterFloat(unknown474);
		RegisterFloat(unknown475);
		RegisterFloat(unknown476);
		RegisterFloat(unknown477);
		RegisterFloat(unknown478);
		RegisterFloat(unknown479);
		RegisterFloat(unknown480);
		RegisterFloat(unknown481);
		RegisterFloat(unknown482);
		RegisterFloat(unknown483);
		RegisterFloat(unknown484);
		RegisterFloat(unknown485);
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
		RegisterFloat(unknown500);
		RegisterFloat(unknown501);
		RegisterFloat(unknown502);
		RegisterFloat(unknown503);
		RegisterFloat(unknown504);
		RegisterFloat(unknown505);
		RegisterFloat(unknown506);
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
		RegisterUInt16(vision);
		RegisterUInt8(unknown551);

		uint8_t& Unknown525 = unknown525[0];
		RegisterUInt8(Unknown525)->SetCount(1062);
		
		RegisterSubstruct(groupSheet);

		uint8_t& Unknown183 = unknown183[0];
		RegisterUInt8(Unknown183)->SetCount(460);
		
		RegisterUInt32(pet_id);
		char& Pet_name = pet_name[0];
		RegisterChar(Pet_name)->SetCount(32);

		uint8_t& Unknown184 = unknown184[0];
		RegisterUInt8(Unknown184)->SetCount(9);
		
		RegisterFloat(pet_health_pct);
		RegisterFloat(pet_power_pct);
		RegisterUInt8(unknown185);
		RegisterUInt8(pet_movement);
		RegisterUInt8(pet_behavior);

		uint8_t& Unknown186 = unknown186[0];
		RegisterUInt8(Unknown186)->SetCount(8);
		
		RegisterUInt32(merc_id);
		char& Merc_name = merc_name[0];
		RegisterChar(Merc_name)->SetCount(32);
		
		uint8_t& merc_Unknown21 = merc_unknown21[0];
		RegisterUInt8(merc_Unknown21)->SetCount(9);
		
		RegisterFloat(merc_health_pct);
		RegisterFloat(merc_power_pct);
		RegisterUInt8(merc_unknown21b);
		RegisterUInt8(merc_movement);
		RegisterUInt8(merc_behavior);

		uint8_t& merc_Unknown21c = merc_unknown21c[0];
		RegisterUInt8(merc_Unknown21c)->SetCount(73);
		
		RegisterUInt32(group_leader_id);

		uint8_t& merc_Unknown21cc = merc_unknown21cc[0];
		RegisterUInt8(merc_Unknown21cc)->SetCount(4);
		
		RegisterFloat(rain);
		RegisterFloat(rain2);

		uint8_t& Unknown527 = unknown527[0];
		RegisterUInt8(Unknown527)->SetCount(9);
		
		char& House_zone = house_zone[0];
		RegisterChar(House_zone)->SetCount(48);
		
		uint8_t& Unknown526 = unknown526[0];
		RegisterUInt8(Unknown526)->SetCount(132);
		
		RegisterUInt32(status_points);
		RegisterUInt32(guild_status);

		uint8_t& Unknown187 = unknown187[0];
		RegisterUInt8(Unknown187)->SetCount(11);
		
		char& Bind_zone = bind_zone[0];
		RegisterChar(Bind_zone)->SetCount(32);
		
		uint8_t& Unknown188 = unknown188[0];
		RegisterUInt8(Unknown188)->SetCount(52);
	}

};

class OP_UpdateCharacterSheetMsg_Packet : public UpdateCharacterSheetMsgData, public EQ2Packet {
public:
	OP_UpdateCharacterSheetMsg_Packet(uint32_t version) : UpdateCharacterSheetMsgData(version), EQ2Packet(version), packedData(version <= 283) {
		packedData.LinkSubstruct(*static_cast<UpdateCharacterSheetMsgData*>(this), "packed_data");
		//RegisterSubstruct(packedData);
		//Can't use the macro while inheriting from both PacketSubstruct and EQ2Packet
		auto e = new PacketSubstructParent<PacketPackedData>(packedData);
		e->SetName("packedData");
		EQ2Packet::elements.push_back(e);
	}

	PacketPackedData packedData;
};

/*
<Struct Name="WS_CharacterSheet" ClientVersion="60085" OpcodeName="OP_UpdateCharacterSheetMsg">
<Data ElementName="character_name" Type="char" Size="40" />
<Data ElementName="unknown_1_1_MJ" Type="int16" Size="1" />
<Data ElementName="race" Type="int8" Size="1" />
<Data ElementName="gender" Type="int8" Size="1" />
<Data ElementName="exiled" Type="int8" Size="1" />
<Data ElementName="class1" Type="int32" Size="1" />
<Data ElementName="class2" Type="int32" Size="1" />
<Data ElementName="class3" Type="int32" Size="1" />
<Data ElementName="tradeskill_class1" Type="int32" Size="1" />
<Data ElementName="tradeskill_class2" Type="int32" Size="1" />
<Data ElementName="tradeskill_class3" Type="int32" Size="1" />
<Data ElementName="level" Type="int16" Size="1" />
<Data ElementName="effective_level" Type="int16" Size="1" />
<Data ElementName="tradeskill_level" Type="int16" Size="1" />
<Data ElementName="unknown_1_2_MJ" Type="int32" Size="1" />
<Data ElementName="account_age_base" Type="int16" Size="1" />
<Data ElementName="account_age_bonus" Type="int16" Size="1" />
<Data ElementName="deity" Type="char" Size="32" />
<Data ElementName="last_name" Type="char" Size="20" />
<Data ElementName="unknown3" Type="int8" Size="1" />
<Data ElementName="character_name2" Type="char" Size="40" />
<Data ElementName="character_name2_unknown" Type="int16" Size="1" />
<Data ElementName="character_name3" Type="char" Size="40" />
<Data ElementName="character_name3_unknown" Type="int16" Size="1" />
<Data ElementName="current_hp" Type="sint64" Size="1" />
<Data ElementName="max_hp" Type="int64" Size="1" />
<Data ElementName="base_hp" Type="int32" Size="1" />
<Data ElementName="base_hp2" Type="int32" Size="1" />
<Data ElementName="current_power" Type="sint32" Size="1" />
<Data ElementName="max_power" Type="sint32" Size="1" />
<Data ElementName="base_power" Type="int32" Size="1" />
<Data ElementName="conc_used" Type="int8" Size="1" />
<Data ElementName="conc_max" Type="int8" Size="1" />
<Data ElementName="savagery" Type="sint32" Size="1" />
<Data ElementName="max_savagery" Type="sint32" Size="1" />
<Data ElementName="unknown4b" Type="int32" Size="1" />
<Data ElementName="savagery_level" Type="int32" Size="1" />
<Data ElementName="max_savagery_level" Type="int32" Size="1" />
<Data ElementName="unknown4c" Type="int8" Size="4" />
<Data ElementName="dissonance" Type="sint32" Size="1" />
<!-- index 283 -->
<Data ElementName="max_dissonance" Type="sint32" Size="1" />
<Data ElementName="unknown5c" Type="int8" Size="4" />
<Data ElementName="hp_regen" Type="int32" Size="1" />
<Data ElementName="power_regen" Type="int32" Size="1" />
<Data ElementName="unknown6" Type="int32" Size="2" />
<Data ElementName="unknown7" Type="float" Size="2" />
<Data ElementName="stat_bonus_health" Type="float" Size="1" />
<Data ElementName="stat_bonus_power" Type="float" Size="1" />
<Data ElementName="bonus_health" Type="int32" Size="1" />
<Data ElementName="unknown8" Type="int32" Size="1" />
<Data ElementName="bonus_power" Type="int32" Size="1" />
<Data ElementName="stat_bonus_damage" Type="float" Size="1" />
<Data ElementName="mitigation_pct_pve" Type="int16" Size="1" />
<Data ElementName="mitigation_pct_pvp" Type="int16" Size="1" />
<Data ElementName="toughness" Type="int16" Size="1" />
<Data ElementName="toughness_resist_dmg_pvp" Type="float" Size="1" />
<Data ElementName="lethality" Type="int16" Size="1" />
<Data ElementName="lethality_pct" Type="float" Size="1" />
<Data ElementName="avoidance_pct" Type="int16" Size="1" />
<Data ElementName="avoidance_reduction" Type="int16" Size="1" />
<Data ElementName="avoidance" Type="int16" Size="1" />
<Data ElementName="unknown10" Type="int16" Size="1" />
<Data ElementName="avoidance_base" Type="int16" Size="1" />
<Data ElementName="unknown10a" Type="int16" Size="1" />
<Data ElementName="parry" Type="int16" Size="1" />
<Data ElementName="unknown11" Type="int16" Size="1" />
<Data ElementName="block" Type="int16" Size="1" />
<Data ElementName="unknown12" Type="int16" Size="1" />
<Data ElementName="uncontested_block" Type="int16" Size="1" />
<Data ElementName="unknown13" Type="int16" Size="1" />
<Data ElementName="uncontested_riposte" Type="int16" Size="1" />
<Data ElementName="uncontested_dodge" Type="int16" Size="1" />
<Data ElementName="uncontested_parry" Type="int16" Size="1" />
<Data ElementName="str" Type="int32" Size="1" />
<Data ElementName="sta" Type="int32" Size="1" />
<Data ElementName="agi" Type="int32" Size="1" />
<Data ElementName="wis" Type="int32" Size="1" />
<Data ElementName="int" Type="int32" Size="1" />
<Data ElementName="str_base" Type="int32" Size="1" />
<Data ElementName="sta_base" Type="int32" Size="1" />
<Data ElementName="agi_base" Type="int32" Size="1" />
<Data ElementName="wis_base" Type="int32" Size="1" />
<Data ElementName="int_base" Type="int32" Size="1" />
<Data ElementName="mitigation_cur" Type="int32" Size="1" />
<Data ElementName="elemental" Type="int32" Size="1" />
<Data ElementName="noxious" Type="int32" Size="1" />
<Data ElementName="arcane" Type="int32" Size="1" />
<Data ElementName="mitigation_base" Type="int32" Size="1" />
<Data ElementName="elemental_base" Type="int32" Size="1" />
<Data ElementName="noxious_base" Type="int32" Size="1" />
<Data ElementName="arcane_base" Type="int32" Size="1" />
<Data ElementName="unknown14" Type="int16" Size="1" />
<Data ElementName="elemental_absorb_pve" Type="int16" Size="1" />
<Data ElementName="noxious_absorb_pve" Type="int16" Size="1" />
<Data ElementName="arcane_absorb_pve" Type="int16" Size="1" />
<Data ElementName="unknown15" Type="int16" Size="1" />
<Data ElementName="elemental_absorb_pvp" Type="int16" Size="1" />
<Data ElementName="noxious_absorb_pvp" Type="int16" Size="1" />
<Data ElementName="arcane_absorb_pvp" Type="int16" Size="1" />
<Data ElementName="unknown16" Type="int16" Size="1" />
<Data ElementName="elemental_dmg_reduction" Type="int16" Size="1" />
<Data ElementName="noxious_dmg_reduction" Type="int16" Size="1" />
<Data ElementName="arcane_dmg_reduction" Type="int16" Size="1" />
<Data ElementName="unknown17" Type="int16" Size="1" />
<Data ElementName="elemental_dmg_reduction_pct" Type="int16" Size="1" />
<Data ElementName="noxious_dmg_reduction_pct" Type="int16" Size="1" />
<Data ElementName="arcane_dmg_reduction_pct" Type="int16" Size="1" />
<Data ElementName="current_adv_xp" Type="float" Size="1" />
<Data ElementName="needed_adv_xp" Type="float" Size="1" />
<Data ElementName="debt_adv_xp" Type="float" Size="1" />
<Data ElementName="current_trade_xp" Type="float" Size="1" />
<Data ElementName="needed_trade_xp" Type="float" Size="1" />
<Data ElementName="debt_trade_xp" Type="float" Size="1" />
<Data ElementName="unknown18" Type="int16" Size="6" />
<Data ElementName="server_bonus" Type="int16" Size="1" />
<Data ElementName="adventure_vet_bonus" Type="int16" Size="1" />
<Data ElementName="tradeskill_vet_bonus" Type="int16" Size="1" />
<Data ElementName="dungeon_finder_bonus" Type="int16" Size="1" />
<Data ElementName="recruit_friend_bonus" Type="int32" Size="1" />
<Data ElementName="unknown19" Type="int16" Size="1" />
<Data ElementName="adventure_vitality" Type="int16" Size="1" />
<Data ElementName="adventure_vitality_yellow_arrow" Type="int16" Size="1" />
<Data ElementName="adventure_vitality_blue_arrow" Type="int16" Size="1" />
<Data ElementName="tradeskill_vitality" Type="int16" Size="1" />
<Data ElementName="tradeskill_vitality_purple_arrow" Type="int16" Size="1" />
<Data ElementName="tradeskill_vitality_blue_arrow" Type="int16" Size="1" />
<Data ElementName="mentor_bonus" Type="int16" Size="1" />
<Data ElementName="unknown20" Type="int8" Size="1" />
<Data ElementName="assigned_aa" Type="int16" Size="1" />
<Data ElementName="max_aa" Type="int16" Size="1" />
<Data ElementName="unassigned_aa" Type="int16" Size="1" />
<Data ElementName="aa_green_bar" Type="int16" Size="1" />
<Data ElementName="adv_xp_to_aa_xp_slider" Type="int16" Size="1" />
<Data ElementName="unknown21" Type="int16" Size="1" />
<Data ElementName="aa_blue_bar" Type="int16" Size="1" />
<Data ElementName="bonus_achievement_xp" Type="int16" Size="1" />
<Data ElementName="unknown22" Type="int8" Size="2" />
<Data ElementName="unknown23" Type="int8" Size="2" />
<Data ElementName="items_found" Type="int32" Size="1" />
<Data ElementName="named_npcs_killed" Type="int32" Size="1" />
<Data ElementName="quests_completed" Type="int32" Size="1" />
<Data ElementName="exploration_events" Type="int32" Size="1" />
<Data ElementName="completed_collections" Type="int32" Size="1" />
<Data ElementName="unknown24" Type="int16" Size="10" />
<Data ElementName="unknown25" Type="int8" Size="1" />
<Data ElementName="total_prestige_points" Type="int16" Size="1" />
<Data ElementName="unassigned_prestige_points" Type="int16" Size="1" />
<Data ElementName="unknown26" Type="int16" Size="1" />
<Data ElementName="unknown27" Type="int16" Size="1" />
<Data ElementName="total_tradeskill_points" Type="int16" Size="1" />
<Data ElementName="unassigned_tradeskill_points" Type="int16" Size="1" />
<Data ElementName="unknown28" Type="int16" Size="1" />
<Data ElementName="unknown29" Type="int16" Size="1" />
<Data ElementName="total_tradeskill_prestige_points" Type="int16" Size="1" />
<Data ElementName="unassigned_tradeskill_prestige_points" Type="int16" Size="1" />
<Data ElementName="unknown30" Type="int16" Size="1" />
<Data ElementName="unknown31" Type="int16" Size="1" />
<Data ElementName="unknown32" Type="int16" Size="1" />
<Data ElementName="unknown33" Type="int16" Size="1" />
<Data ElementName="coins_copper" Type="int32" Size="1" />
<Data ElementName="coins_silver" Type="int32" Size="1" />
<Data ElementName="coins_gold" Type="int32" Size="1" />
<Data ElementName="coins_plat" Type="int32" Size="1" />
<Data ElementName="unknown34" Type="int32" Size="5" />
<Data ElementName="spell_effects" Substruct="Substruct_SpellEffects" Size="45" />
<Data ElementName="detrimental_spell_effects" Substruct="Substruct_SpellEffects" Size="45" />
<Data ElementName="passive_spell_effects" Substruct="Substruct_PassiveEffects" Size="100" />
<Data ElementName="unknown35" Type="int8" Size="486" />
<Data ElementName="trauma_count" Type="int8" Size="1" />
<Data ElementName="arcane_count" Type="int8" Size="1" />
<Data ElementName="noxious_count" Type="int8" Size="1" />
<Data ElementName="elemental_count" Type="int8" Size="1" />
<Data ElementName="curse_count" Type="int8" Size="1" />
<Data ElementName="maintained_effects" Substruct="Substruct_MaintainedEffects" Size="30" />
<Data ElementName="breath" Type="float" Size="1" />
<Data ElementName="breathe_underwater" Type="int8" Size="1" />
<Data ElementName="unknown36" Type="float" Size="1" />
<Data ElementName="melee_pri_dmg_min" Type="int32" Size="1" />
<Data ElementName="melee_pri_dmg_max" Type="int32" Size="1" />
<Data ElementName="melee_pri_delay" Type="float" Size="1" />
<Data ElementName="melee_sec_dmg_min" Type="int32" Size="1" />
<Data ElementName="melee_sec_dmg_max" Type="int32" Size="1" />
<Data ElementName="melee_sec_delay" Type="float" Size="1" />
<Data ElementName="ranged_dmg_min" Type="int32" Size="1" />
<Data ElementName="ranged_dmg_max" Type="int32" Size="1" />
<Data ElementName="ranged_delay" Type="float" Size="1" />
<Data ElementName="unknown37" Type="int32" Size="1" />
<Data ElementName="unknown38" Type="int32" Size="1" />
<Data ElementName="unknown39" Type="int32" Size="1" />
<Data ElementName="unknown40" Type="int32" Size="1" />
<Data ElementName="ability_mod_pve" Type="float" Size="1" />
<Data ElementName="unknown41" Type="float" Size="1" />
<Data ElementName="unknown42" Type="float" Size="1" />
<Data ElementName="unknown43" Type="float" Size="1" />
<Data ElementName="unknown44" Type="float" Size="1" />
<Data ElementName="ability_mod_pvp" Type="float" Size="1" />
<Data ElementName="base_melee_crit" Type="float" Size="1" />
<Data ElementName="base_spell_crit" Type="float" Size="1" />
<Data ElementName="base_taunt_crit" Type="float" Size="1" />
<Data ElementName="base_heal_crit" Type="float" Size="1" />
<!--<Data ElementName="unknown44a" Type="int16" Size="1" />
-->
<!-- 58588 - Required to make it in the game -->
<Data ElementName="flags" Type="int32" Size="1" />
<Data ElementName="flags2" Type="int32" Size="1" />
<Data ElementName="unknown45" Type="float" Size="1" />
<Data ElementName="unknown46" Type="float" Size="1" />
<Data ElementName="unknown47" Type="float" Size="1" />
<Data ElementName="unknown48" Type="float" Size="1" />
<Data ElementName="unknown49" Type="float" Size="1" />
<Data ElementName="unknown50" Type="float" Size="1" />
<Data ElementName="unknown51" Type="float" Size="1" />
<Data ElementName="unknown52" Type="float" Size="1" />
<Data ElementName="unknown53" Type="float" Size="1" />
<Data ElementName="unknown54" Type="float" Size="1" />
<Data ElementName="unknown55" Type="float" Size="1" />
<Data ElementName="unknown56" Type="float" Size="1" />
<Data ElementName="unknown57" Type="float" Size="1" />
<Data ElementName="unknown58" Type="float" Size="1" />
<Data ElementName="unknown59" Type="float" Size="1" />
<Data ElementName="unknown60" Type="float" Size="1" />
<Data ElementName="unknown61" Type="float" Size="1" />
<Data ElementName="unknown62" Type="float" Size="1" />
<Data ElementName="unknown63" Type="float" Size="1" />
<Data ElementName="unknown64" Type="float" Size="1" />
<Data ElementName="unknown65" Type="float" Size="1" />
<Data ElementName="unknown66" Type="float" Size="1" />
<Data ElementName="unknown67" Type="float" Size="1" />
<Data ElementName="unknown68" Type="float" Size="1" />
<Data ElementName="unknown69" Type="float" Size="1" />
<Data ElementName="unknown70" Type="float" Size="1" />
<Data ElementName="unknown71" Type="float" Size="1" />
<Data ElementName="unknown72" Type="float" Size="1" />
<Data ElementName="unknown73" Type="float" Size="1" />
<Data ElementName="unknown74" Type="float" Size="1" />
<Data ElementName="unknown75" Type="float" Size="1" />
<Data ElementName="unknown76" Type="float" Size="1" />
<Data ElementName="unknown77" Type="float" Size="1" />
<Data ElementName="unknown78" Type="float" Size="1" />
<Data ElementName="unknown79" Type="float" Size="1" />
<Data ElementName="unknown80" Type="float" Size="1" />
<Data ElementName="unknown81" Type="float" Size="1" />
<Data ElementName="unknown82" Type="float" Size="1" />
<Data ElementName="unknown83" Type="float" Size="1" />
<Data ElementName="unknown84" Type="float" Size="1" />
<Data ElementName="unknown85" Type="float" Size="1" />
<Data ElementName="unknown86" Type="float" Size="1" />
<Data ElementName="unknown87" Type="float" Size="1" />
<Data ElementName="unknown88" Type="float" Size="1" />
<Data ElementName="unknown89" Type="float" Size="1" />
<Data ElementName="unknown90" Type="float" Size="1" />
<Data ElementName="unknown91" Type="float" Size="1" />
<Data ElementName="unknown92" Type="float" Size="1" />
<Data ElementName="unknown93" Type="float" Size="1" />
<Data ElementName="unknown94" Type="float" Size="1" />
<Data ElementName="unknown95" Type="float" Size="1" />
<Data ElementName="unknown96" Type="float" Size="1" />
<Data ElementName="unknown97" Type="float" Size="1" />
<Data ElementName="unknown98" Type="float" Size="1" />
<Data ElementName="unknown99" Type="float" Size="1" />
<Data ElementName="unknown100" Type="float" Size="1" />
<Data ElementName="unknown101" Type="float" Size="1" />
<Data ElementName="unknown102" Type="float" Size="1" />
<Data ElementName="unknown103" Type="float" Size="1" />
<Data ElementName="unknown104" Type="float" Size="1" />
<Data ElementName="unknown105" Type="float" Size="1" />
<Data ElementName="unknown106" Type="float" Size="1" />
<Data ElementName="unknown107" Type="float" Size="1" />
<Data ElementName="unknown108" Type="float" Size="1" />
<Data ElementName="unknown109" Type="float" Size="1" />
<Data ElementName="unknown110" Type="float" Size="1" />
<Data ElementName="out_of_combat_health_regen" Type="float" Size="1" />
<Data ElementName="out_of_combat_power_regen" Type="float" Size="1" />
<Data ElementName="in_combat_regen" Type="float" Size="1" />
<Data ElementName="deflect" Type="float" Size="1" />
<Data ElementName="increase_max_health" Type="float" Size="1" />
<Data ElementName="increase_max_health2" Type="float" Size="1" />
<!-- used when bonus is applied to both health & power -->
<Data ElementName="unknown117" Type="float" Size="1" />
<Data ElementName="increase_speed" Type="float" Size="1" />
<Data ElementName="unknown119" Type="float" Size="1" />
<Data ElementName="increase_ground_speed" Type="float" Size="1" />
<Data ElementName="increase_air_speed" Type="float" Size="1" />
<Data ElementName="unknown122" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 8 which involves skill buff -->
<Data ElementName="unknown123" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 3 which involves skill buff -->
<Data ElementName="unknown124" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 3 which involves skill buff -->
<Data ElementName="in_combat_movement_speed" Type="float" Size="1" />
<Data ElementName="haste" Type="float" Size="1" />
<Data ElementName="increase_max_power" Type="float" Size="1" />
<Data ElementName="increase_max_power2" Type="float" Size="1" />
<!-- used when bonus is applied to both health & power -->
<Data ElementName="unknown128" Type="float" Size="1" />
<Data ElementName="drunk" Type="float" Size="1" />
<Data ElementName="unknown129" Type="float" Size="1" />
<Data ElementName="hate_mod" Type="float" Size="1" />
<Data ElementName="adventure_effects_bonus" Type="float" Size="1" />
<Data ElementName="tradeskill_effects_bonus" Type="float" Size="1" />
<Data ElementName="unknown130" Type="float" Size="1" />
<Data ElementName="unknown131" Type="float" Size="1" />
<Data ElementName="dps" Type="float" Size="1" />
<Data ElementName="unknown132" Type="float" Size="1" />
<Data ElementName="unknown133" Type="float" Size="1" />
<Data ElementName="unknown134" Type="float" Size="1" />
<Data ElementName="base_avoidance" Type="float" Size="1" />
<Data ElementName="unknown136" Type="float" Size="1" />
<Data ElementName="uncontested_riposte_pve" Type="float" Size="1" />
<Data ElementName="uncontested_parry_pve" Type="float" Size="1" />
<Data ElementName="unknown139" Type="float" Size="1" />
<Data ElementName="melee_ae" Type="float" Size="1" />
<Data ElementName="multi_attack" Type="float" Size="1" />
<Data ElementName="spell_multi_attack" Type="float" Size="1" />
<!-- spell_doublecast -->
<Data ElementName="flurry" Type="float" Size="1" />
<Data ElementName="unknown140" Type="float" Size="1" />
<Data ElementName="bountiful_harvest" Type="float" Size="1" />
<Data ElementName="block_chance" Type="float" Size="1" />
<Data ElementName="unknown141" Type="float" Size="1" />
<Data ElementName="unknown142" Type="float" Size="1" />
<Data ElementName="crit_chance" Type="float" Size="1" />
<Data ElementName="unknown143" Type="float" Size="1" />
<Data ElementName="unknown144" Type="float" Size="1" />
<Data ElementName="crit_bonus" Type="float" Size="1" />
<Data ElementName="potency" Type="float" Size="1" />
<Data ElementName="unknown145" Type="float" Size="1" />
<Data ElementName="reuse_speed" Type="float" Size="1" />
<Data ElementName="recovery_speed" Type="float" Size="1" />
<Data ElementName="casting_speed" Type="float" Size="1" />
<Data ElementName="spell_reuse_speed" Type="float" Size="1" />
<Data ElementName="unknown146" Type="float" Size="1" />
<Data ElementName="unknown147" Type="float" Size="1" />
<Data ElementName="decrease_falling_dmg" Type="float" Size="1" />
<Data ElementName="unknown149" Type="float" Size="1" />
<Data ElementName="unknown150" Type="float" Size="1" />
<Data ElementName="unknown151" Type="float" Size="1" />
<Data ElementName="unknown152" Type="float" Size="1" />
<Data ElementName="unknown153" Type="float" Size="1" />
<Data ElementName="unknown154" Type="float" Size="1" />
<Data ElementName="unknown155" Type="float" Size="1" />
<Data ElementName="unknown156" Type="float" Size="1" />
<Data ElementName="loot_coin" Type="float" Size="1" />
<Data ElementName="mitigation_increase" Type="float" Size="1" />
<Data ElementName="unknown159" Type="float" Size="1" />
<Data ElementName="strikethrough" Type="float" Size="1" />
<Data ElementName="unknown160" Type="float" Size="1" />
<Data ElementName="accuracy" Type="float" Size="1" />
<Data ElementName="unknown161" Type="float" Size="1" />
<Data ElementName="unknown162" Type="float" Size="1" />
<Data ElementName="unknown163" Type="float" Size="1" />
<Data ElementName="unknown164" Type="float" Size="1" />
<Data ElementName="unknown165" Type="float" Size="1" />
<Data ElementName="unknown166" Type="float" Size="1" />
<Data ElementName="unknown167" Type="float" Size="1" />
<Data ElementName="unknown168" Type="float" Size="1" />
<Data ElementName="lethality_pve" Type="float" Size="1" />
<Data ElementName="unknown170" Type="float" Size="1" />
<Data ElementName="unknown171" Type="float" Size="1" />
<Data ElementName="unknown172" Type="float" Size="1" />
<Data ElementName="unknown173" Type="float" Size="1" />
<Data ElementName="pvp_crit_mit" Type="float" Size="1" />
<Data ElementName="unknown174" Type="float" Size="1" />
<Data ElementName="unknown175" Type="float" Size="1" />
<Data ElementName="unknown200" Type="float" Size="1" />
<Data ElementName="unknown201" Type="float" Size="1" />
<Data ElementName="unknown202" Type="float" Size="1" />
<Data ElementName="unknown203" Type="float" Size="1" />
<Data ElementName="unknown204" Type="float" Size="1" />
<Data ElementName="unknown205" Type="float" Size="1" />
<Data ElementName="unknown206" Type="float" Size="1" />
<Data ElementName="unknown207" Type="float" Size="1" />
<Data ElementName="unknown208" Type="float" Size="1" />
<Data ElementName="unknown209" Type="float" Size="1" />
<Data ElementName="unknown210" Type="float" Size="1" />
<Data ElementName="unknown211" Type="float" Size="1" />
<Data ElementName="unknown212" Type="float" Size="1" />
<Data ElementName="unknown213" Type="float" Size="1" />
<Data ElementName="unknown214" Type="float" Size="1" />
<Data ElementName="unknown215" Type="float" Size="1" />
<Data ElementName="unknown216" Type="float" Size="1" />
<Data ElementName="unknown217" Type="float" Size="1" />
<Data ElementName="unknown218" Type="float" Size="1" />
<Data ElementName="unknown219" Type="float" Size="1" />
<Data ElementName="unknown220" Type="float" Size="1" />
<Data ElementName="unknown221" Type="float" Size="1" />
<Data ElementName="unknown222" Type="float" Size="1" />
<Data ElementName="unknown223" Type="float" Size="1" />
<Data ElementName="unknown224" Type="float" Size="1" />
<Data ElementName="unknown225" Type="float" Size="1" />
<Data ElementName="unknown226" Type="float" Size="1" />
<Data ElementName="unknown227" Type="float" Size="1" />
<Data ElementName="unknown228" Type="float" Size="1" />
<Data ElementName="unknown229" Type="float" Size="1" />
<Data ElementName="unknown230" Type="float" Size="1" />
<Data ElementName="unknown231" Type="float" Size="1" />
<Data ElementName="unknown232" Type="float" Size="1" />
<Data ElementName="unknown233" Type="float" Size="1" />
<Data ElementName="unknown234" Type="float" Size="1" />
<Data ElementName="unknown235" Type="float" Size="1" />
<Data ElementName="unknown236" Type="float" Size="1" />
<Data ElementName="unknown237" Type="float" Size="1" />
<Data ElementName="unknown238" Type="float" Size="1" />
<Data ElementName="unknown239" Type="float" Size="1" />
<Data ElementName="unknown240" Type="float" Size="1" />
<Data ElementName="unknown241" Type="float" Size="1" />
<Data ElementName="unknown242" Type="float" Size="1" />
<Data ElementName="uncontested_riposte_gear_cap" Type="float" Size="1" />
<Data ElementName="uncontested_dodge_gear_cap" Type="float" Size="1" />
<Data ElementName="uncontested_parry_gear_cap" Type="float" Size="1" />
<Data ElementName="unknown246" Type="float" Size="1" />
<Data ElementName="unknown247" Type="float" Size="1" />
<Data ElementName="unknown248" Type="float" Size="1" />
<Data ElementName="unknown249" Type="float" Size="1" />
<Data ElementName="unknown250" Type="float" Size="1" />
<Data ElementName="unknown251" Type="float" Size="1" />
<Data ElementName="unknown252" Type="float" Size="1" />
<Data ElementName="unknown253" Type="float" Size="1" />
<Data ElementName="unknown254" Type="float" Size="1" />
<Data ElementName="unknown255" Type="float" Size="1" />
<Data ElementName="unknown256" Type="float" Size="1" />
<Data ElementName="unknown257" Type="float" Size="1" />
<Data ElementName="unknown258" Type="float" Size="1" />
<Data ElementName="unknown259" Type="float" Size="1" />
<Data ElementName="unknown260" Type="float" Size="1" />
<Data ElementName="unknown261" Type="float" Size="1" />
<Data ElementName="unknown262" Type="float" Size="1" />
<Data ElementName="unknown263" Type="float" Size="1" />
<Data ElementName="unknown264" Type="float" Size="1" />
<Data ElementName="unknown265" Type="float" Size="1" />
<Data ElementName="unknown266" Type="float" Size="1" />
<Data ElementName="unknown267" Type="float" Size="1" />
<Data ElementName="unknown268" Type="float" Size="1" />
<Data ElementName="unknown269" Type="float" Size="1" />
<Data ElementName="unknown270" Type="float" Size="1" />
<Data ElementName="unknown271" Type="float" Size="1" />
<Data ElementName="unknown272" Type="float" Size="1" />
<Data ElementName="unknown273" Type="float" Size="1" />
<Data ElementName="unknown274" Type="float" Size="1" />
<Data ElementName="unknown275" Type="float" Size="1" />
<Data ElementName="unknown276" Type="float" Size="1" />
<Data ElementName="unknown277" Type="float" Size="1" />
<Data ElementName="unknown278" Type="float" Size="1" />
<Data ElementName="unknown279" Type="float" Size="1" />
<Data ElementName="unknown280" Type="float" Size="1" />
<Data ElementName="unknown281" Type="float" Size="1" />
<Data ElementName="unknown282" Type="float" Size="1" />
<Data ElementName="unknown283" Type="float" Size="1" />
<Data ElementName="unknown284" Type="float" Size="1" />
<Data ElementName="unknown285" Type="float" Size="1" />
<Data ElementName="unknown286" Type="float" Size="1" />
<Data ElementName="unknown287" Type="float" Size="1" />
<Data ElementName="unknown288" Type="float" Size="1" />
<Data ElementName="unknown289" Type="float" Size="1" />
<Data ElementName="unknown290" Type="float" Size="1" />
<Data ElementName="unknown291" Type="float" Size="1" />
<Data ElementName="unknown292" Type="float" Size="1" />
<Data ElementName="unknown293" Type="float" Size="1" />
<Data ElementName="unknown294" Type="float" Size="1" />
<Data ElementName="unknown295" Type="float" Size="1" />
<Data ElementName="unknown296" Type="float" Size="1" />
<Data ElementName="unknown297" Type="float" Size="1" />
<Data ElementName="unknown298" Type="float" Size="1" />
<Data ElementName="unknown299" Type="float" Size="1" />
<Data ElementName="unknown300" Type="float" Size="1" />
<Data ElementName="unknown301" Type="float" Size="1" />
<Data ElementName="unknown302" Type="float" Size="1" />
<Data ElementName="unknown303" Type="float" Size="1" />
<Data ElementName="unknown304" Type="float" Size="1" />
<Data ElementName="unknown305" Type="float" Size="1" />
<Data ElementName="unknown306" Type="float" Size="1" />
<Data ElementName="unknown307" Type="float" Size="1" />
<Data ElementName="unknown308" Type="float" Size="1" />
<Data ElementName="out_of_combat_health_regen_pvp" Type="float" Size="1" />
<Data ElementName="out_of_combat_power_regen_pvp" Type="float" Size="1" />
<Data ElementName="in_combat_regen_pvp" Type="float" Size="1" />
<Data ElementName="deflect_pvp" Type="float" Size="1" />
<Data ElementName="increase_max_heath_pvp" Type="float" Size="1" />
<Data ElementName="increase_max_health2_pvp" Type="float" Size="1" />
<!-- used when bonus is applied to both health & power -->
<Data ElementName="unknown312" Type="float" Size="1" />
<Data ElementName="increase_speed_pvp" Type="float" Size="1" />
<Data ElementName="unknown313" Type="float" Size="1" />
<Data ElementName="increase_ground_speed_pvp" Type="float" Size="1" />
<Data ElementName="increase_air_speed_pvp" Type="float" Size="1" />
<Data ElementName="unknown314" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 8 which involves skill buff -->
<Data ElementName="unknown315" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 3 which involves skill buff -->
<Data ElementName="unknown316" Type="float" Size="1" />
<!-- see coldain pegasus buff this has a value of 3 which involves skill buff -->
<Data ElementName="in_combat_movement_speed_pvp" Type="float" Size="1" />
<Data ElementName="Haste_pvp" Type="float" Size="1" />
<Data ElementName="increase_max_power_pvp" Type="float" Size="1" />
<Data ElementName="increase_max_power2_pvp" Type="float" Size="1" />
<!-- used when bonus is applied to both health & power -->
<Data ElementName="unknown321" Type="float" Size="1" />
<Data ElementName="unknown322" Type="float" Size="1" />
<Data ElementName="unknown323" Type="float" Size="1" />
<Data ElementName="hate_mod_pvp" Type="float" Size="1" />
<Data ElementName="unknown325" Type="float" Size="1" />
<Data ElementName="unknown326" Type="float" Size="1" />
<Data ElementName="unknown327" Type="float" Size="1" />
<Data ElementName="unknown328" Type="float" Size="1" />
<Data ElementName="dps_pvp" Type="float" Size="1" />
<Data ElementName="unknown330" Type="float" Size="1" />
<Data ElementName="unknown331" Type="float" Size="1" />
<Data ElementName="unknown332" Type="float" Size="1" />
<Data ElementName="base_avoidance_pvp" Type="float" Size="1" />
<Data ElementName="unknown334" Type="float" Size="1" />
<Data ElementName="uncontested_riposte_pvp" Type="float" Size="1" />
<Data ElementName="uncontested_parry_pvp" Type="float" Size="1" />
<Data ElementName="unknown337" Type="float" Size="1" />
<Data ElementName="unknown338" Type="float" Size="1" />
<Data ElementName="multi_attack_pvp" Type="float" Size="1" />
<Data ElementName="spell_multi_attack_pvp" Type="float" Size="1" />
<!-- spell_doublecast -->
<Data ElementName="unknown341" Type="float" Size="1" />
<Data ElementName="unknown342" Type="float" Size="1" />
<Data ElementName="unknown343" Type="float" Size="1" />
<Data ElementName="block_chance_pvp" Type="float" Size="1" />
<Data ElementName="unknown345" Type="float" Size="1" />
<Data ElementName="unknown346" Type="float" Size="1" />
<Data ElementName="crit_chance_pvp" Type="float" Size="1" />
<Data ElementName="unknown348" Type="float" Size="1" />
<Data ElementName="unknown349" Type="float" Size="1" />
<Data ElementName="crit_bonus_pvp" Type="float" Size="1" />
<Data ElementName="potency_pvp" Type="float" Size="1" />
<Data ElementName="unknown352" Type="float" Size="1" />
<Data ElementName="reuse_speed_pvp" Type="float" Size="1" />
<Data ElementName="recovery_speed_pvp" Type="float" Size="1" />
<Data ElementName="casting_speed_pvp" Type="float" Size="1" />
<Data ElementName="unknown356" Type="float" Size="1" />
<Data ElementName="unknown357" Type="float" Size="1" />
<Data ElementName="unknown358" Type="float" Size="1" />
<Data ElementName="decrease_falling_dmg_pvp" Type="float" Size="1" />
<Data ElementName="unknown360" Type="float" Size="1" />
<Data ElementName="unknown361" Type="float" Size="1" />
<Data ElementName="unknown362" Type="float" Size="1" />
<Data ElementName="unknown363" Type="float" Size="1" />
<Data ElementName="unknown364" Type="float" Size="1" />
<Data ElementName="unknown365" Type="float" Size="1" />
<Data ElementName="unknown366" Type="float" Size="1" />
<Data ElementName="unknown367" Type="float" Size="1" />
<Data ElementName="loot_coin_pvp" Type="float" Size="1" />
<Data ElementName="mitigation_increase_pvp" Type="float" Size="1" />
<Data ElementName="unknown370" Type="float" Size="1" />
<Data ElementName="strikethrough_pvp" Type="float" Size="1" />
<Data ElementName="unknown372" Type="float" Size="1" />
<Data ElementName="unknown373" Type="float" Size="1" />
<Data ElementName="unknown374" Type="float" Size="1" />
<Data ElementName="unknown375" Type="float" Size="1" />
<Data ElementName="unknown376" Type="float" Size="1" />
<Data ElementName="unknown377" Type="float" Size="1" />
<Data ElementName="unknown378" Type="float" Size="1" />
<Data ElementName="unknown379" Type="float" Size="1" />
<Data ElementName="unknown380" Type="float" Size="1" />
<Data ElementName="unknown381" Type="float" Size="1" />
<Data ElementName="lethality_pvp" Type="float" Size="1" />
<Data ElementName="unknown383" Type="float" Size="1" />
<Data ElementName="unknown384" Type="float" Size="1" />
<Data ElementName="unknown385" Type="float" Size="1" />
<Data ElementName="unknown386" Type="float" Size="1" />
<Data ElementName="unknown387" Type="float" Size="1" />
<Data ElementName="unknown388" Type="float" Size="1" />
<Data ElementName="unknown389" Type="float" Size="1" />
<Data ElementName="unknown390" Type="float" Size="1" />
<Data ElementName="unknown391" Type="float" Size="1" />
<Data ElementName="unknown392" Type="float" Size="1" />
<Data ElementName="unknown393" Type="float" Size="1" />
<Data ElementName="unknown394" Type="float" Size="1" />
<Data ElementName="unknown395" Type="float" Size="1" />
<Data ElementName="unknown396" Type="float" Size="1" />
<Data ElementName="unknown397" Type="float" Size="1" />
<Data ElementName="unknown398" Type="float" Size="1" />
<Data ElementName="durability_mod" Type="float" Size="1" />
<Data ElementName="durability_add" Type="float" Size="1" />
<Data ElementName="progress_mod" Type="float" Size="1" />
<Data ElementName="progress_add" Type="float" Size="1" />
<Data ElementName="success_mod" Type="float" Size="1" />
<Data ElementName="crit_success_mod" Type="float" Size="1" />
<Data ElementName="unknown177" Type="float" Size="1" />
<Data ElementName="rare_harvest_chance" Type="float" Size="1" />
<Data ElementName="max_crafting" Type="float" Size="1" />
<Data ElementName="component_refund" Type="float" Size="1" />
<Data ElementName="unknown178" Type="float" Size="1" />
<Data ElementName="ex_durability_mod" Type="float" Size="1" />
<Data ElementName="ex_durability_add" Type="float" Size="1" />
<Data ElementName="ex_crit_success_mod" Type="float" Size="1" />
<Data ElementName="ex_crit_failure_mod" Type="float" Size="1" />
<Data ElementName="ex_progress_mod" Type="float" Size="1" />
<Data ElementName="ex_progress_add" Type="float" Size="1" />
<Data ElementName="ex_success_mod" Type="float" Size="1" />
<Data ElementName="unknown417" Type="float" Size="1" />
<Data ElementName="unknown418" Type="float" Size="1" />
<Data ElementName="unknown419" Type="float" Size="1" />
<Data ElementName="unknown420" Type="float" Size="1" />
<Data ElementName="unknown421" Type="float" Size="1" />
<Data ElementName="unknown422" Type="float" Size="1" />
<Data ElementName="unknown423" Type="float" Size="1" />
<Data ElementName="unknown424" Type="float" Size="1" />
<Data ElementName="unknown425" Type="float" Size="1" />
<Data ElementName="unknown426" Type="float" Size="1" />
<Data ElementName="unknown427" Type="float" Size="1" />
<Data ElementName="unknown428" Type="float" Size="1" />
<Data ElementName="unknown429" Type="float" Size="1" />
<Data ElementName="unknown430" Type="float" Size="1" />
<Data ElementName="unknown431" Type="float" Size="1" />
<Data ElementName="unknown432" Type="float" Size="1" />
<Data ElementName="unknown433" Type="float" Size="1" />
<Data ElementName="unknown434" Type="float" Size="1" />
<Data ElementName="unknown435" Type="float" Size="1" />
<Data ElementName="unknown436" Type="float" Size="1" />
<Data ElementName="unknown437" Type="float" Size="1" />
<Data ElementName="unknown438" Type="float" Size="1" />
<Data ElementName="unknown439" Type="float" Size="1" />
<Data ElementName="unknown440" Type="float" Size="1" />
<Data ElementName="unknown441" Type="float" Size="1" />
<Data ElementName="unknown180" Type="int8" Size="1" />
<Data ElementName="unknown524" Type="int8" Size="1" />
<Data ElementName="unknown181a" Type="int8" Size="1" />
<Data ElementName="unknown181b" Type="int8" Size="1" />
<Data ElementName="unknown442" Type="float" Size="1" />
<Data ElementName="unknown443" Type="float" Size="1" />
<Data ElementName="unknown444" Type="float" Size="1" />
<Data ElementName="unknown445" Type="float" Size="1" />
<Data ElementName="unknown446" Type="float" Size="1" />
<Data ElementName="unknown447" Type="float" Size="1" />
<Data ElementName="unknown448" Type="float" Size="1" />
<Data ElementName="unknown449" Type="float" Size="1" />
<Data ElementName="unknown450" Type="float" Size="1" />
<Data ElementName="unknown451" Type="float" Size="1" />
<Data ElementName="unknown452" Type="float" Size="1" />
<Data ElementName="unknown453" Type="float" Size="1" />
<Data ElementName="unknown454" Type="float" Size="1" />
<Data ElementName="unknown455" Type="float" Size="1" />
<Data ElementName="unknown456" Type="float" Size="1" />
<Data ElementName="unknown457" Type="float" Size="1" />
<Data ElementName="unknown458" Type="float" Size="1" />
<Data ElementName="unknown459" Type="float" Size="1" />
<Data ElementName="unknown460" Type="float" Size="1" />
<Data ElementName="unknown461" Type="float" Size="1" />
<Data ElementName="unknown462" Type="float" Size="1" />
<Data ElementName="unknown463" Type="float" Size="1" />
<Data ElementName="unknown464" Type="float" Size="1" />
<Data ElementName="unknown465" Type="float" Size="1" />
<Data ElementName="unknown466" Type="float" Size="1" />
<Data ElementName="unknown467" Type="float" Size="1" />
<Data ElementName="unknown468" Type="float" Size="1" />
<Data ElementName="unknown469" Type="float" Size="1" />
<Data ElementName="unknown470" Type="float" Size="1" />
<Data ElementName="unknown471" Type="float" Size="1" />
<Data ElementName="unknown472" Type="float" Size="1" />
<Data ElementName="unknown473" Type="float" Size="1" />
<Data ElementName="unknown474" Type="float" Size="1" />
<Data ElementName="unknown475" Type="float" Size="1" />
<Data ElementName="unknown476" Type="float" Size="1" />
<Data ElementName="unknown477" Type="float" Size="1" />
<Data ElementName="unknown478" Type="float" Size="1" />
<Data ElementName="unknown479" Type="float" Size="1" />
<Data ElementName="unknown480" Type="float" Size="1" />
<Data ElementName="unknown481" Type="float" Size="1" />
<Data ElementName="unknown482" Type="float" Size="1" />
<Data ElementName="unknown483" Type="float" Size="1" />
<Data ElementName="unknown484" Type="float" Size="1" />
<Data ElementName="unknown485" Type="float" Size="1" />
<Data ElementName="unknown486" Type="float" Size="1" />
<Data ElementName="unknown487" Type="float" Size="1" />
<Data ElementName="unknown488" Type="float" Size="1" />
<Data ElementName="unknown489" Type="float" Size="1" />
<Data ElementName="unknown490" Type="float" Size="1" />
<Data ElementName="unknown491" Type="float" Size="1" />
<Data ElementName="unknown492" Type="float" Size="1" />
<Data ElementName="unknown493" Type="float" Size="1" />
<Data ElementName="unknown494" Type="float" Size="1" />
<Data ElementName="unknown495" Type="float" Size="1" />
<Data ElementName="unknown496" Type="float" Size="1" />
<Data ElementName="unknown497" Type="float" Size="1" />
<Data ElementName="unknown498" Type="float" Size="1" />
<Data ElementName="unknown499" Type="float" Size="1" />
<Data ElementName="unknown500" Type="float" Size="1" />
<Data ElementName="unknown501" Type="float" Size="1" />
<Data ElementName="unknown502" Type="float" Size="1" />
<Data ElementName="unknown503" Type="float" Size="1" />
<Data ElementName="unknown504" Type="float" Size="1" />
<Data ElementName="unknown505" Type="float" Size="1" />
<Data ElementName="unknown506" Type="float" Size="1" />
<Data ElementName="unknown507" Type="float" Size="1" />
<Data ElementName="unknown508" Type="float" Size="1" />
<Data ElementName="unknown509" Type="float" Size="1" />
<Data ElementName="unknown510" Type="float" Size="1" />
<Data ElementName="unknown511" Type="float" Size="1" />
<Data ElementName="unknown512" Type="float" Size="1" />
<Data ElementName="unknown513" Type="float" Size="1" />
<Data ElementName="unknown514" Type="float" Size="1" />
<Data ElementName="haste_tt" Type="int32" Size="1" />
<Data ElementName="dps_pve_tt" Type="int32" Size="1" />
<Data ElementName="dps_pvp_tt" Type="int32" Size="1" />
<Data ElementName="multi_attack_pve_tt" Type="float" Size="1" />
<Data ElementName="multi_attack_pvp_tt" Type="float" Size="1" />
<Data ElementName="unknown520" Type="float" Size="1" />
<Data ElementName="unknown521" Type="float" Size="1" />
<Data ElementName="unknown522" Type="float" Size="1" />
<Data ElementName="unknown523" Type="float" Size="1" />
<Data ElementName="unknown550" Type="int8" Size="1" />
<Data ElementName="vision" Type="int16" Size="1" />
<Data ElementName="unknown551" Type="int8" Size="1" />
<Data ElementName="unknown525" Type="int8" Size="1062" />
<Data ElementName="group_members" Substruct="Substruct_GroupMember" Size="5" />
<Data ElementName="unknown182" Type="int16" Size="1" />
<Data ElementName="unknown183" Type="int8" Size="462" />
<Data ElementName="pet_id" Type="int32" Size="1" />
<Data ElementName="pet_name" Type="char" Size="32" />
<Data ElementName="unknown184" Type="int8" Size="9" />
<Data ElementName="pet_health_pct" Type="float" Size="1" />
<Data ElementName="pet_power_pct" Type="float" Size="1" />
<Data ElementName="unknown185" Type="int8" Size="1" />
<Data ElementName="pet_movement" Type="int8" Size="1" />
<Data ElementName="pet_behavior" Type="int8" Size="1" />
<Data ElementName="unknown186" Type="int8" Size="8" />
<Data ElementName="merc_id" Type="int32" Size="1" />
<Data ElementName="merc_name" Type="char" Size="32" />
<Data ElementName="merc_unknown21" Type="int8" Size="9" />
<Data ElementName="merc_health_pct" Type="float" Size="1" />
<Data ElementName="merc_power_pct" Type="float" Size="1" />
<Data ElementName="merc_unknown21b" Type="int8" Size="1" />
<Data ElementName="merc_movement" Type="int8" Size="1" />
<Data ElementName="merc_behavior" Type="int8" Size="1" />
<Data ElementName="merc_unknown21c" Type="int8" Size="73" />
<Data ElementName="group_leader_id" Type="int32" Size="1" />
<Data ElementName="merc_unknown21cc" Type="int8" Size="4" />
<Data ElementName="rain" Type="float" Size="1" />
<Data ElementName="rain2" Type="float" Size="1" />
<Data ElementName="unknown527" Type="int8" Size="9" />
<Data ElementName="house_zone" Type="char" Size="48" />
<Data ElementName="unknown526" Type="int8" Size="132" />
<Data ElementName="status_points" Type="int32" Size="1" />
<Data ElementName="guild_status" Type="int32" Size="1" />
<Data ElementName="unknown187" Type="int8" Size="11" />
<Data ElementName="bind_zone" Type="char" Size="32" />
<Data ElementName="unknown188" Type="int8" Size="52" />
</Struct>
*/