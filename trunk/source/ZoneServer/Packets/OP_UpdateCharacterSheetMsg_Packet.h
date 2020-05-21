#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketPackedData.h"

#include "Substruct_SpellEffects.h"
#include "Substruct_PassiveEffects.h"
#include "Substruct_MaintainedEffects.h"
#include "Substruct_GroupMember.h"
#include "../Players/CharacterSheet.h"

//This is all the primitive data which can be zeroed
//Need to make this crap less ridiculous at some point
struct CharacterSheetMiscData {
	CharacterSheetMiscData() {
		memset(this, 0, sizeof(*this));
	}

	//Some elements are a different format in the packet from what we use, convert them here
	char character_name[40];
	char deity[32];
	char last_name[20];
	char house_zone[48];
	char bind_zone[32];
	int64_t hp;
	int64_t baseHp;
	int64_t maxHp;
	float advExp;
	float advExpNextLevel;
	float advExpDebt;
	float advVitality;
	float tsExp;
	float tsExpNextLevel;
	float tsExpDebt;
	float tsVitality;


	uint16_t unknown_1_1_MJ;
	uint32_t unknown_1_2_MJ;
	uint16_t account_age_base;
	uint16_t account_age_bonus;
	
	uint8_t unknown3;
	
	uint16_t character_name2_unknown;
	
	uint16_t character_name3_unknown;
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
	
	uint8_t unknown35[486];
	uint8_t trauma_count;
	uint8_t arcane_count;
	uint8_t noxious_count;
	uint8_t elemental_count;
	uint8_t curse_count;
	
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
	float aa_effects_bonus;
	float unknown131;
	float unknown131A;
	float dps;
	float unknown132;
	float unknown133;
	float unknown134;
	float base_avoidance;
	float unknown136;
	float uncontested_riposte_pve;
	float uncontested_parry_pve;
	float unknown139;
	float spell_multi_attack;
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
	float wdb;
	float spell_wdb;
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
	float unknown243;
	float unknown244;
	float unknown245;
	float unknown246;
	float unknown247;
	float uncontested_riposte_gear_cap;
	float uncontested_parry_gear_cap;
	float uncontested_dodge_gear_cap;
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
	float adventure_effects_bonus_2;
	float tradeskill_effects_bonus_2;
	float aa_effects_bonus_2;
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
	float unknown399;
	float unknown400;
	float unknown401;
	float unknown402;
	float unknown403;
	float unknown404;
	float unknown405;
	float unknown406;
	float unknown407;
	float unknown408;
	float unknown409;
	float unknown410;
	float durability_mod;
	float durability_add;
	float progress_mod;
	float progress_add;
	float success_mod;
	float crit_success_mod;
	float unknown417;
	float rare_harvest_chance;
	float max_crafting;
	float component_refund;
	float unknown421;
	float refine_quality_mod;
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
	uint16_t vision;
	uint8_t unknown551;
	uint8_t unknown552[24];
	float adventure_effects_cap;
	float tradeskill_effects_cap;
	float aa_effects_cap;
	uint8_t unknown525[1026];
	uint16_t unknown182;
	uint8_t unknown183[456];
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
	uint8_t unknown527[4];
	uint32_t status_points;
	uint32_t guild_status;
	uint8_t unknown526[141];
	uint8_t unknown187[11];
	uint8_t unknown188[55];
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

	UpdateCharacterSheetMsgData(uint32_t version, const CharacterSheet& sheet) : PacketEncodedData(version), CharacterSheet(sheet), groupSheet(version) {
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

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override;

	Substruct_SpellEffects spell_effects[45];
	Substruct_SpellEffects detrimental_spell_effects[45];
	Substruct_PassiveEffects passive_spell_effects[100];
	Substruct_MaintainedEffects maintained_effects[30];
	Substruct_GroupSheet groupSheet;

private:
	void RegisterElements() {
		static EntityAttributeSheet structDumperHackSheet;

		EntityAttributeSheet* attributes = this->attributes ? this->attributes : &structDumperHackSheet;

		char& char_name = character_name[0];
		RegisterChar(char_name)->SetCount(40);
		RegisterUInt16(unknown_1_1_MJ);
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
		RegisterUInt32(unknown_1_2_MJ);
		RegisterUInt16(account_age_base);
		RegisterUInt16(account_age_bonus);
		char& Deity = deity[0];
		RegisterChar(Deity)->SetCount(32);
		char& Last_name = last_name[0];
		RegisterChar(Last_name)->SetCount(20);
		RegisterUInt8(unknown3);
		char& char_name2 = character_name[0];
		RegisterChar(char_name2)->SetCount(40);
		RegisterUInt16(character_name2_unknown);
		char& char_name3 = character_name[0];
		RegisterChar(char_name3)->SetCount(40);
		RegisterUInt16(character_name3_unknown);
		RegisterInt64(hp);
		RegisterInt64(maxHp);
		RegisterInt64(baseHp);

		int32_t current_power = attributes->power.currentValue;
		int32_t max_power = attributes->power.maxValue;
		int32_t base_power = attributes->power.baseValue;
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

		int32_t noxious = attributes->noxious.currentValue;
		int32_t elemental = attributes->elemental.currentValue;
		int32_t arcane = attributes->arcane.currentValue;
		RegisterInt32(elemental);
		RegisterInt32(noxious);
		RegisterInt32(arcane);
		int32_t& mitigation_base = attributes->mitigation.baseValue;
		RegisterInt32(mitigation_base);
		int32_t noxious_base = attributes->noxious.baseValue;
		int32_t elemental_base = attributes->elemental.baseValue;
		int32_t arcane_base = attributes->arcane.baseValue;
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
		RegisterFloat(aa_effects_bonus);
		RegisterFloat(unknown131);
		RegisterFloat(unknown131A);
		RegisterFloat(dps);
		RegisterFloat(unknown132);
		RegisterFloat(unknown133);
		RegisterFloat(unknown134);
		RegisterFloat(base_avoidance);
		RegisterFloat(unknown136);
		RegisterFloat(uncontested_riposte_pve);
		RegisterFloat(uncontested_parry_pve);
		RegisterFloat(unknown139);
		float& melee_ae = attributes->aeAttackChance.currentValue;
		RegisterFloat(melee_ae);
		float& multi_attack = attributes->multiAttackChance.baseValue;
		RegisterFloat(multi_attack);
		RegisterFloat(spell_multi_attack);
		float& flurry = attributes->flurryChance.baseValue;
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
		RegisterFloat(wdb);
		RegisterFloat(spell_wdb);
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
		RegisterFloat(unknown243);
		RegisterFloat(unknown244);
		RegisterFloat(unknown245);
		RegisterFloat(unknown246);
		RegisterFloat(unknown247);
		RegisterFloat(uncontested_riposte_gear_cap);
		RegisterFloat(uncontested_parry_gear_cap);
		RegisterFloat(uncontested_dodge_gear_cap);
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
		RegisterFloat(adventure_effects_bonus_2);
		RegisterFloat(tradeskill_effects_bonus_2);
		RegisterFloat(aa_effects_bonus_2);
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
		RegisterFloat(unknown399);
		RegisterFloat(unknown400);
		RegisterFloat(unknown401);
		RegisterFloat(unknown402);
		RegisterFloat(unknown403);
		RegisterFloat(unknown404);
		RegisterFloat(unknown405);
		RegisterFloat(unknown406);
		RegisterFloat(unknown407);
		RegisterFloat(unknown408);
		RegisterFloat(unknown409);
		RegisterFloat(unknown410);
		RegisterFloat(durability_mod);
		RegisterFloat(durability_add);
		RegisterFloat(progress_mod);
		RegisterFloat(progress_add);
		RegisterFloat(success_mod);
		RegisterFloat(crit_success_mod);
		RegisterFloat(unknown417);
		RegisterFloat(rare_harvest_chance);
		RegisterFloat(max_crafting);
		RegisterFloat(component_refund);
		RegisterFloat(unknown421);
		RegisterFloat(refine_quality_mod);
		RegisterFloat(ex_durability_mod);
		RegisterFloat(ex_durability_add);
		RegisterFloat(ex_crit_success_mod);
		RegisterFloat(ex_crit_failure_mod);
		RegisterFloat(ex_progress_mod);
		RegisterFloat(ex_progress_add);
		RegisterFloat(ex_success_mod);;
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
		RegisterUInt16(vision);
		RegisterUInt8(unknown551);

		uint8_t& Unknown552 = unknown552[0];
		RegisterUInt8(Unknown552)->SetCount(24);

		RegisterFloat(adventure_effects_cap);
		RegisterFloat(tradeskill_effects_cap);
		RegisterFloat(aa_effects_cap);

		uint8_t& Unknown525 = unknown525[0];
		RegisterUInt8(Unknown525)->SetCount(1026);
		
		RegisterSubstruct(groupSheet);

		uint8_t& Unknown183 = unknown183[0];
		RegisterUInt8(Unknown183)->SetCount(456);
		
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
		RegisterUInt8(Unknown527)->SetCount(4);

		RegisterUInt32(status_points);
		RegisterUInt32(guild_status);
		
		char& House_zone = house_zone[0];
		RegisterChar(House_zone)->SetCount(48);
		
		uint8_t& Unknown526 = unknown526[0];
		RegisterUInt8(Unknown526)->SetCount(141);

		uint8_t& Unknown187 = unknown187[0];
		RegisterUInt8(Unknown187)->SetCount(11);
		
		char& Bind_zone = bind_zone[0];
		RegisterChar(Bind_zone)->SetCount(32);
		
		uint8_t& Unknown188 = unknown188[0];
		RegisterUInt8(Unknown188)->SetCount(55);
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
<Data ElementName="aa_effects_bonus" Type="float" Size="1" />
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