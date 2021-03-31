#pragma once

#include "../../common/Parser/Parser.h"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <string>

class ParserZone : public Parser {
public:
	ParserZone(int arg, char** argv);

	void ProcessLogs();

	void ProcessItems(PacketLog& log);
	void ProcessAppearances(PacketLog& log);
};

struct ItemSetKey {
	std::string set_name;
	uint32_t id;
	bool bPvpDesc;
	int16_t item_level;
	bool operator==(const ItemSetKey& rhs) {
		return set_name == rhs.set_name && bPvpDesc == rhs.bPvpDesc && item_level == rhs.item_level;
	}
};

class LogParser {
public:
	LogParser(PacketLog& l, class ParserDatabase& db) : log(l), database(db) {}
	virtual ~LogParser() = default;

	void QueueRowInsert(class DatabaseRow& row);
	void DoInsertsForTable(const char* table, int maxPerQuery);

private:
	//map<tableName, pair<fields, vector<values> > >
	std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> queued_inserts;

protected:
	class ParserDatabase& database;
	PacketLog& log;
};

class LogItemsParser : public LogParser {
public:
	LogItemsParser(PacketLog& log, class ParserDatabase& db);

private:
	void ProcessQueuedInserts();
	std::string GetItemTypeAsString(uint8_t type);

	void LoadExistingData();
	void ProcessItemDesc(class Substruct_ExamineDescItem* item, bool bPvp);
	void ProcessTypeSpecificData(class Substruct_ExamineDescItem* item);
	void ProcessItemArmor(class Substruct_ExamineDescItem_Armor* a);
	void ProcessItemMeleeWeapon(class Substruct_ExamineDescItem_MeleeWeapon* a);
	void ProcessItemRangedWeapon(class Substruct_ExamineDescItem_RangedWeapon* a);
	void ProcessItemShield(class Substruct_ExamineDescItem_Shield* a);
	void ProcessItemBag(class Substruct_ExamineDescItem_Bag* b);
	void ProcessItemSpellScroll(class Substruct_ExamineDescItem_SpellScroll* b);
	void ProcessItemRecipeBook(class Substruct_ExamineDescItem_RecipeBook* b);
	void ProcessItemProvision(class Substruct_ExamineDescItem_Provision* p);
	void ProcessItemBauble(class Substruct_ExamineDescItem_Bauble* p);
	void ProcessItemHouse(class Substruct_ExamineDescItem_House* h);
	void ProcessItemAmmo(class Substruct_ExamineDescItem_Ammo* a);
	void ProcessItemHouseContainer(class Substruct_ExamineDescItem_HouseContainer* h);
	void ProcessItemAdornment(class Substruct_ExamineDescItem_Adornment* a);
	void ProcessItemAchievementProfile(class Substruct_ExamineDescItem_AchievementProfile* a);
	void ProcessItemRewardVoucher(class Substruct_ExamineDescItem_RewardVoucher* r);
	void ProcessItemRewardCrate(class Substruct_ExamineDescItem_RewardCrate* r);
	void ProcessItemBook(class Substruct_ExamineDescItem_Book* r);
	void ProcessItemReforgingDecoration(class Substruct_ExamineDescItem_ReforgingDecoration* r);

	void ProcessHouseData(class Substruct_HouseItem* h);
	uint32_t ProcessItemSetData(class Substruct_ExamineDescItem* item, bool bPvp);

	std::unordered_set<uint32_t> parsed_soe_items;
	std::vector<ItemSetKey> parsed_item_sets;
	uint32_t next_id;
	uint32_t item_id;
};

class LogAppearancesParser : public LogParser {
public:
	LogAppearancesParser(PacketLog& log, class ParserDatabase& db);
	~LogAppearancesParser() = default;


};