#pragma once

#include <cstdint>
#include <map>
#include "Mutex.h"
#include <string>

enum class ERuleCategory {
	R_Client,
	R_Faction,
	R_Guild,
	R_Player,
	R_PVP,
	R_Spawn,
	R_UI,
	R_World,
	R_Zone,
	R_CategoryInvalid
};

enum class ERuleType {
	/* CLIENT */
	ShowWelcomeScreen,

	/* FACTION */
	AllowFactionBasedCombat,

	/* GUILD */
	/* PLAYER */
	MaxLevel,
	MaxLevelOverrideStatus,
	MaxPlayers,
	MaxPlayersOverrideStatus,
	VitalityAmount,
	VitalityFrequency,
	MaxAA,
	MaxClassAA,
	MaxSubclassAA,
	MaxShadowsAA,
	MaxHeroicAA,
	MaxTradeskillAA,
	MaxPrestigeAA,
	MaxTradeskillPrestigeAA,
	MaxDragonAA,
	MinLastNameLevel,
	MaxLastNameLength,
	MinLastNameLength,

	/* PVP */
	AllowPVP,

	/* SPAWN */
	SpeedMultiplier,
	SpeedRatio,

	/* UI */
	MaxWhoResults,
	MaxWhoOverrideStatus,

	/* WORLD */
	DefaultStartingZoneID,
	EnablePOIDiscovery,
	GamblingTokenItemID,
	GuildAutoJoin,
	GuildAutoJoinID,
	GuildAutoJoinDefaultRankID,
	ServerLocked,
	ServerLockedOverrideStatus,
	SyncZonesWithLogin,
	SyncEquipWithLogin,
	UseBannedIPsTable,
	LinkDeadTimer,
	RemoveDisconnectedClientsTimer,
	PlayerCampTimer,
	GMCampTimer,
	AutoAdminPlayers,
	AutoAdminGMs,
	AutoAdminStatusValue,
	DuskTime,
	DawnTime,
	ThreadedLoad,
	TradeskillSuccessChance,
	TradeskillCritSuccessChance,
	TradeskillFailChance,
	TradeskillCritFailChance,
	TradeskillEventChance,
	EditorURL,
	EditorIncludeID,
	EditorOfficialServer,
	IRCEnabled,
	IRCGlobalEnabled,
	IRCAddress,
	IRCPort,
	IRCChan,
	GroupSpellsTimer,
	SavePaperdollImage,
	SaveHeadshotImage,
	SendPaperdollImagesToLogin,
	TreasureChestDisabled,

	/* ZONE */
	MinZoneLevelOverrideStatus,
	MinZoneAccessOverrideStatus,
	XPMultiplier,
	TSXPMultiplier,
	WeatherEnabled,
	WeatherType,
	MinWeatherSeverity,
	MaxWeatherSeverity,
	WeatherChangeFrequency,
	WeatherChangePerInterval,
	WeatherDynamicMaxOffset,
	WeatherChangeChance,
	SpawnUpdateTimer,
	CheckAttackPlayer,
	CheckAttackNPC,
	HOTime,
	HearChatDistance,

	/* ZONE TIMERS */
	RegenTimer,
	ClientSaveTimer,
	DefaultZoneShutdownTimer,
	WeatherTimer,
	SpawnDeleteTimer,
	RuleTypeInvalid
};

class Rule {
public:
	Rule();
	Rule(ERuleCategory category, ERuleType type, const char* value);
	Rule(Rule* rule_in);
	virtual ~Rule();

	void SetValue(const char* v) { value = v; }

	ERuleCategory GetCategory() { return category; }
	ERuleType GetType() { return type; }
	std::string GetValue() { return value; }

	uint8_t GetUInt8() { return atoi(value.c_str()); }
	uint16_t GetUInt16() { return atoi(value.c_str()); }
	uint32_t GetUInt32() { return strtoul(value.c_str(), nullptr, 10); }
	int8_t GetInt8() { return atoi(value.c_str()); }
	int16_t GetInt16() { return atoi(value.c_str()); }
	int32_t GetInt32() { return atoi(value.c_str()); }
	bool GetBool() { return atoi(value.c_str()) > 0 ? true : false; }
	float GetFloat() { return (float)atof(value.c_str()); }
	char GetChar() { return value[0]; }
	std::string GetString() { return value; }

private:
	ERuleCategory category;
	ERuleType type;
	std::string value;
};

class RuleSet {
public:
	RuleSet();
	RuleSet(RuleSet* in_rule_set);
	virtual ~RuleSet();

	void CopyRulesInto(RuleSet* in_rule_set);

	void SetID(uint32_t id) { this->id = id; }
	void SetName(const char* str) { name = str; }

	uint32_t GetID() { return id; }
	std::string GetName() { return name; }

	void AddRule(Rule* rule);
	Rule* GetRule(ERuleCategory category, ERuleType type);
	void ClearRules();

	std::map<ERuleCategory, std::map<ERuleType, Rule*> >* GetRules() { return &rules; }

private:
	uint32_t id;
	std::string name;
	Mutex m_rules;
	std::map<ERuleCategory, std::map<ERuleType, Rule*> > rules;
};

class RuleManager {
public:
	RuleManager();
	virtual ~RuleManager();

	void LoadCodedDefaultsIntoRuleSet(RuleSet* rule_set);

	bool AddRuleSet(uint32_t id, const char* name);
	uint32_t GetNumRuleSets();
	void ClearRuleSets();

	bool SetRuleValue(uint32_t ruleset_id, ERuleCategory cat, ERuleType rt, const char* value);

	bool SetGlobalRuleSet(uint32_t rule_set_id);
	Rule* GetGlobalRule(ERuleCategory category, ERuleType type);

	bool SetZoneRuleSet(uint32_t zone_id, uint32_t rule_set_id);
	Rule* GetZoneRule(uint32_t zone_id, ERuleCategory category, ERuleType type);
	void ClearZoneRuleSets();

	RuleSet* GetGlobalRuleSet() { return &global_rule_set; }
	std::map<ERuleCategory, std::map<ERuleType, Rule*> >* GetRules() { return &rules; }

	ERuleCategory GetRuleCategoryByName(const char* name);
	ERuleType GetRuleTypeByName(const char* name);

private:
	void InitEnumLookups();
	void InitRuleDefaults();
	std::map<std::string, ERuleCategory> categoryStringLookup;
	std::map<std::string, ERuleType> ruleTypeStringLookup;


	Mutex m_rule_sets;
	Mutex m_global_rule_set;
	Mutex m_chunk_rule_sets;
	std::map<ERuleCategory, std::map<ERuleType, Rule*> > rules;	/* all of the rules loaded with their defaults (FROM THE CODE). map<category, map<type, rule>> */
	std::map<uint32_t, RuleSet*> rule_sets;				/* all of the possible rule sets from the database. map<rule set id, rule set> */
	RuleSet global_rule_set;						/* the global rule set, first fill it the defaults from the code, then over ride from the database */
	std::map<uint32_t, RuleSet*> zone_rule_sets;		/* references to a zone's rule set. map<zone id, rule set> */
};