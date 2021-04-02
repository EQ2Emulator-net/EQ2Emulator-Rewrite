#pragma once

#include <string>
#include <cstdint>
#include "ParserZone.h"
#include "../../common/Parser/LogParser.h"

struct ItemSetKey {
	std::string set_name;
	uint32_t id;
	bool bPvpDesc;
	int16_t item_level;
	bool operator==(const ItemSetKey& rhs) {
		return set_name == rhs.set_name && bPvpDesc == rhs.bPvpDesc && item_level == rhs.item_level;
	}
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