#include "stdafx.h"

#include "log.h"
#include "Rules.h"
#include "CommonDatabase.h"
#include "EmuAssert.h"

Rule::Rule() {
	category = ERuleCategory::R_CategoryInvalid;
	type = ERuleType::RuleTypeInvalid;
}

Rule::Rule(ERuleCategory cat, ERuleType rule_type, const char* v) {
	category = cat;
	type = rule_type;
	value = v;
}

Rule::Rule(Rule* rule_in) {
	category = rule_in->GetCategory();
	type = rule_in->GetType();
	value = rule_in->GetValue();
}

Rule::~Rule() {
}

RuleSet::RuleSet() {
	id = 0;
	m_rules.SetName("RuleSet::rules");
}

RuleSet::RuleSet(RuleSet* in_rule_set) {
	auto in_rules = in_rule_set->GetRules();
	Rule* rule;

	EmuAssert(in_rule_set);

	m_rules.SetName("RuleSet::rules");
	id = in_rule_set->GetID();
	name = in_rule_set->GetName();
	for (auto& itr : *in_rules) {
		for (auto& itr2 : itr.second) {
			rule = itr2.second;
			rules[rule->GetCategory()][rule->GetType()] = new Rule(rule);
		}
	}
}

RuleSet::~RuleSet() {
	ClearRules();
}

void RuleSet::CopyRulesInto(RuleSet* in_rule_set) {
	if (!in_rule_set) {
		LogError(LOG_RULES, 0, "in_rule_set was invalid");
		return;
	}

	auto in_rules = in_rule_set->GetRules();
	Rule* rule;

	EmuAssert(in_rule_set);

	ClearRules();
	m_rules.WriteLock();
	for (auto& itr : *in_rules) {
		for (auto& itr2 : itr.second) {
			rule = itr2.second;
			rules[rule->GetCategory()][rule->GetType()] = new Rule(rule);
		}
	}
	m_rules.WriteUnlock();
}

void RuleSet::AddRule(Rule* rule) {
	ERuleCategory category;
	ERuleType type;

	EmuAssert(rule);

	category = rule->GetCategory();
	type = rule->GetType();
	m_rules.WriteLock();;
	if (rules[category].count(type) == 0)
		rules[category][type] = rule;
	else
		rules[category][type]->SetValue(rule->GetValue().c_str());
	m_rules.WriteUnlock();
}

Rule* RuleSet::GetRule(ERuleCategory category, ERuleType type) {
	Rule* ret = nullptr;

	ReadLocker lock(m_rules);
	auto itr = rules.find(category);
	if (itr != rules.end()) {
		auto rule = itr->second.find(type);
		if (rule != itr->second.end()) {
			ret = rule->second;
		}
	}

	return ret;
}

void RuleSet::ClearRules() {
	m_rules.WriteLock();
	for (auto& itr : rules) {
		for (auto& itr2 : itr.second)
			delete itr2.second;
	}
	rules.clear();
	m_rules.WriteUnlock();
}

void RuleManager::InitEnumLookups() {
#define CATEGORY_INIT(c) categoryStringLookup[#c] = ERuleCategory::c
	CATEGORY_INIT(R_Client);
	CATEGORY_INIT(R_Faction);
	CATEGORY_INIT(R_Guild);
	CATEGORY_INIT(R_Player);
	CATEGORY_INIT(R_PVP);
	CATEGORY_INIT(R_Spawn);
	CATEGORY_INIT(R_UI);
	CATEGORY_INIT(R_World);
	CATEGORY_INIT(R_Zone);
	CATEGORY_INIT(R_CategoryInvalid);
#undef CATEGORY_INIT

#define RULE_TYPE_INIT(rt) ruleTypeStringLookup[#rt] = ERuleType::rt
	RULE_TYPE_INIT(MaxPlayers);
	RULE_TYPE_INIT(MaxPlayersOverrideStatus);
	RULE_TYPE_INIT(SyncEquipWithLogin);
	RULE_TYPE_INIT(ShowWelcomeScreen);
	RULE_TYPE_INIT(AllowFactionBasedCombat);
	RULE_TYPE_INIT(MaxLevel);
	RULE_TYPE_INIT(MaxLevelOverrideStatus);
	RULE_TYPE_INIT(VitalityAmount);
	RULE_TYPE_INIT(VitalityFrequency);
	RULE_TYPE_INIT(MaxAA);
	RULE_TYPE_INIT(MaxClassAA);
	RULE_TYPE_INIT(MaxSubclassAA);
	RULE_TYPE_INIT(MaxShadowsAA);
	RULE_TYPE_INIT(MaxHeroicAA);
	RULE_TYPE_INIT(MaxTradeskillAA);
	RULE_TYPE_INIT(MaxPrestigeAA);
	RULE_TYPE_INIT(MaxTradeskillPrestigeAA);
	RULE_TYPE_INIT(MaxDragonAA);
	RULE_TYPE_INIT(MinLastNameLevel);
	RULE_TYPE_INIT(MaxLastNameLength);
	RULE_TYPE_INIT(MinLastNameLength);
	RULE_TYPE_INIT(AllowPVP);
	RULE_TYPE_INIT(SpeedMultiplier);
	RULE_TYPE_INIT(SpeedRatio);
	RULE_TYPE_INIT(MaxWhoResults);
	RULE_TYPE_INIT(MaxWhoOverrideStatus);
	RULE_TYPE_INIT(DefaultStartingZoneID);
	RULE_TYPE_INIT(EnablePOIDiscovery);
	RULE_TYPE_INIT(GamblingTokenItemID);
	RULE_TYPE_INIT(GuildAutoJoin);
	RULE_TYPE_INIT(GuildAutoJoinID);
	RULE_TYPE_INIT(GuildAutoJoinDefaultRankID);
	RULE_TYPE_INIT(ServerLocked);
	RULE_TYPE_INIT(ServerLockedOverrideStatus);
	RULE_TYPE_INIT(SyncZonesWithLogin);
	RULE_TYPE_INIT(UseBannedIPsTable);
	RULE_TYPE_INIT(LinkDeadTimer);
	RULE_TYPE_INIT(RemoveDisconnectedClientsTimer);
	RULE_TYPE_INIT(PlayerCampTimer);
	RULE_TYPE_INIT(GMCampTimer);
	RULE_TYPE_INIT(AutoAdminPlayers);
	RULE_TYPE_INIT(AutoAdminGMs);
	RULE_TYPE_INIT(AutoAdminStatusValue);
	RULE_TYPE_INIT(DuskTime);
	RULE_TYPE_INIT(DawnTime);
	RULE_TYPE_INIT(ThreadedLoad);
	RULE_TYPE_INIT(TradeskillSuccessChance);
	RULE_TYPE_INIT(TradeskillCritSuccessChance);
	RULE_TYPE_INIT(TradeskillFailChance);
	RULE_TYPE_INIT(TradeskillCritFailChance);
	RULE_TYPE_INIT(TradeskillEventChance);
	RULE_TYPE_INIT(EditorURL);
	RULE_TYPE_INIT(EditorIncludeID);
	RULE_TYPE_INIT(EditorOfficialServer);
	RULE_TYPE_INIT(IRCEnabled);
	RULE_TYPE_INIT(IRCGlobalEnabled);
	RULE_TYPE_INIT(IRCAddress);
	RULE_TYPE_INIT(IRCPort);
	RULE_TYPE_INIT(IRCChan);
	RULE_TYPE_INIT(GroupSpellsTimer);
	RULE_TYPE_INIT(SavePaperdollImage);
	RULE_TYPE_INIT(SaveHeadshotImage);
	RULE_TYPE_INIT(SendPaperdollImagesToLogin);
	RULE_TYPE_INIT(TreasureChestDisabled);
	RULE_TYPE_INIT(MinZoneLevelOverrideStatus);
	RULE_TYPE_INIT(MinZoneAccessOverrideStatus);
	RULE_TYPE_INIT(XPMultiplier);
	RULE_TYPE_INIT(TSXPMultiplier);
	RULE_TYPE_INIT(WeatherEnabled);
	RULE_TYPE_INIT(WeatherType);
	RULE_TYPE_INIT(MinWeatherSeverity);
	RULE_TYPE_INIT(MaxWeatherSeverity);
	RULE_TYPE_INIT(WeatherChangeFrequency);
	RULE_TYPE_INIT(WeatherChangePerInterval);
	RULE_TYPE_INIT(WeatherDynamicMaxOffset);
	RULE_TYPE_INIT(WeatherChangeChance);
	RULE_TYPE_INIT(SpawnUpdateTimer);
	RULE_TYPE_INIT(CheckAttackPlayer);
	RULE_TYPE_INIT(CheckAttackNPC);
	RULE_TYPE_INIT(HOTime);
	RULE_TYPE_INIT(RegenTimer);
	RULE_TYPE_INIT(ClientSaveTimer);
	RULE_TYPE_INIT(DefaultZoneShutdownTimer);
	RULE_TYPE_INIT(WeatherTimer);
	RULE_TYPE_INIT(SpawnDeleteTimer);
	RULE_TYPE_INIT(RuleTypeInvalid);
	RULE_TYPE_INIT(HearChatDistance);
#undef RULE_TYPE_INIT
}

RuleManager::RuleManager() {
	m_rule_sets.SetName("RuleManager::rule_sets");
	m_global_rule_set.SetName("RuleManager::global_rule_set");
	m_chunk_rule_sets.SetName("RuleManager::chunk_rule_sets");

	InitEnumLookups();
	InitRuleDefaults();

	LoadCodedDefaultsIntoRuleSet(GetGlobalRuleSet());
}

void RuleManager::InitRuleDefaults() {
	auto RuleInit = [this](ERuleCategory cat, ERuleType rt, const char* value) {
		rules[cat][rt] = new Rule(cat, rt, value);
	};

	RuleInit(ERuleCategory::R_Zone, ERuleType::HearChatDistance, "30");
	RuleInit(ERuleCategory::R_World, ERuleType::PlayerCampTimer, "20");
	RuleInit(ERuleCategory::R_World, ERuleType::LinkDeadTimer, "120000");
}

RuleManager::~RuleManager() {
	for (auto& itr : rules) {
		for (auto& itr2 : itr.second)
			delete itr2.second;
	}

	ClearRuleSets();
	ClearZoneRuleSets();
}

void RuleManager::LoadCodedDefaultsIntoRuleSet(RuleSet* rule_set) {
	EmuAssert(rule_set);

	for (auto& itr : rules) {
		for (auto& itr2 : itr.second)
			rule_set->AddRule(new Rule(itr2.second));
	}
}

bool RuleManager::AddRuleSet(uint32_t id, const char* name) {
	bool ret = false;

	m_rule_sets.WriteLock();
	if (rule_sets.count(id) == 0) {
		RuleSet* rs = new RuleSet;
		rs->SetID(id);
		rs->SetName(name);
		rule_sets[id] = rs;
		ret = true;
	}
	m_rule_sets.WriteUnlock();

	return ret;
}

uint32_t RuleManager::GetNumRuleSets() {
	uint32_t ret;

	m_rule_sets.ReadLock();
	ret = static_cast<uint32_t>(rule_sets.size());
	m_rule_sets.ReadUnlock();

	return ret;
}

void RuleManager::ClearRuleSets() {
	m_rule_sets.WriteLock();
	for (auto& itr : rule_sets)
		delete itr.second;
	rule_sets.clear();
	m_rule_sets.WriteUnlock();
}

bool RuleManager::SetGlobalRuleSet(uint32_t rule_set_id) {
	if (rule_sets.count(rule_set_id) == 0)
		return false;

	global_rule_set.CopyRulesInto(rule_sets[rule_set_id]);
	return true;
}

Rule* RuleManager::GetGlobalRule(ERuleCategory category, ERuleType type) {
	return global_rule_set.GetRule(category, type);
}

bool RuleManager::SetZoneRuleSet(uint32_t chunk_id, uint32_t rule_set_id) {
	bool ret = true;
	RuleSet* rule_set;

	m_rule_sets.ReadLock();
	if (rule_sets.count(rule_set_id) == 0)
		ret = false;

	rule_set = rule_sets[rule_set_id];
	if (ret) {
		m_chunk_rule_sets.WriteLock();;
		zone_rule_sets[chunk_id] = rule_set;
		m_chunk_rule_sets.WriteUnlock();;
	}
	m_rule_sets.ReadUnlock();

	return ret;
}

Rule* RuleManager::GetZoneRule(uint32_t chunk_id, ERuleCategory category, ERuleType type) {
	Rule* ret = 0;

	/* we never want to return null so MAKE SURE the rule exists. if this assertion fails then the server admin must fix the problem */
	EmuAssert(rules.count(category) > 0);
	EmuAssert(rules[category].count(type) > 0);

	/* first try to get the zone rule */
	m_chunk_rule_sets.ReadLock();
	auto itr = zone_rule_sets.find(chunk_id);
	if (itr != zone_rule_sets.end())
		ret = itr->second->GetRule(category, type);
	m_chunk_rule_sets.ReadUnlock();

	//If we cannot find a chunk rule, try to revert to a global rule if it's set - if not set use the default
	if (!ret)
		ret = GetGlobalRule(category, type);

	return ret ? ret : rules[category][type];
}

void RuleManager::ClearZoneRuleSets() {
	m_chunk_rule_sets.WriteLock();
	zone_rule_sets.clear();
	m_chunk_rule_sets.WriteUnlock();
}

ERuleCategory RuleManager::GetRuleCategoryByName(const char* name) {
	auto itr = categoryStringLookup.find(name);
	if (itr != categoryStringLookup.end()) {
		return itr->second;
	}

	return ERuleCategory::R_CategoryInvalid;
}

ERuleType RuleManager::GetRuleTypeByName(const char* name) {
	auto itr = ruleTypeStringLookup.find(name);
	if (itr != ruleTypeStringLookup.end()) {
		return itr->second;
	}

	return ERuleType::RuleTypeInvalid;
}

//The return value is whether this rule has been implemented (default value set in RuleManager::InitRuleDefaults() )
bool RuleManager::SetRuleValue(uint32_t ruleset_id, ERuleCategory cat, ERuleType rt, const char* value) {
	bool ret = false;
	ReadLocker lock(m_rule_sets);
	auto itr = rules.find(cat);
	if (itr != rules.end()) {
		auto rule = itr->second.find(rt);
		if (rule != itr->second.end()) {
			ret = true;
			rule->second->SetValue(value);
		}
	}
	return ret;
}

void CommonDatabase::LoadRules(RuleManager& rule_manager) {
	{
		//First load the rule sets
		DatabaseResult result;
		Select(&result, "SELECT `ruleset_id`, `ruleset_name` FROM `rulesets` WHERE `ruleset_active`");
		while (result.Next()) {
			rule_manager.AddRuleSet(result.GetUInt32(0), result.GetString(1));
		}
	}

	//Now load the ruleset values
	DatabaseResult result;
	Select(&result, "SELECT `ruleset_id`, `rule_category`, `rule_type`, `rule_value` FROM `ruleset_details`");
	while (result.Next()) {
		const char* category_str = result.GetString(1);
		const char* type_str = result.GetString(2);

		ERuleCategory cat = rule_manager.GetRuleCategoryByName(category_str);
		ERuleType type = rule_manager.GetRuleTypeByName(type_str);

		if (cat == ERuleCategory::R_CategoryInvalid) {
			LogError(LOG_RULES, 0, "Tried loading a rule with a non existing category! (%s)", category_str);
			continue;
		}

		if (type == ERuleType::RuleTypeInvalid) {
			LogError(LOG_RULES, 0, "Tried loading a rule with a non existing rule type! (%s)", type_str);
			continue;
		}

		if (!rule_manager.SetRuleValue(result.GetUInt32(0), cat, type, result.GetString(3))) {
			LogWarn(LOG_RULES, 0, "Rule %s.%s has not been implemented yet!", category_str, type_str);
		}
	}
}