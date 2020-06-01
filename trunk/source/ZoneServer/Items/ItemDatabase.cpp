#include "stdafx.h"

#include "../Database/ZoneDatabase.h"
#include "MasterItemList.h"
#include "../../common/log.h"
#include "Item.h"
#include "ItemTypes.h"
#include "../Packets/ItemPackets.h"

void ZoneDatabase::LoadMasterItems(MasterItemList& masterItems) {
	std::ostringstream ss;

	LogInfo(LOG_DATABASE, 0, "Loading items...");

	ss << "SELECT MAX(id) FROM character_items;\n"
		<< "SELECT COUNT(id) FROM items;\n"
		<< "SELECT * FROM items WHERE item_type IN ('Normal', 'Dungeon Maker');\n"
		<< "SELECT * FROM items i INNER JOIN item_details_weapon idw ON i.id = idw.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_range idr ON i.id = idr.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_armor ida ON i.id = ida.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_shield ids ON i.id = ids.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_bag idb ON i.id = idb.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_food idf ON i.id = idf.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_recipe idr ON i.id = idr.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_house idh ON i.id = idh.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_bauble idb ON i.id = idb.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_thrown idt ON i.id = idt.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_house_container idhc ON i.id = idhc.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_book idb ON i.id = idb.item_id;\n"
		<< "SELECT i.* FROM items i INNER JOIN item_details_pattern idp ON i.id = idp.item_id;\n"
		<< "SELECT * FROM item_details_pattern;\n";
		//<< "SELECT idri.item_id, i.icon, idri.name FROM items i INNER JOIN item_details_recipe_items idri ON i.id = idri.item_id;\n"
		//
		//<< "SELECT * FROM items i INNER JOIN item_details_adornments ida ON i.id = ida.item_id;\n";

	DatabaseResult result;
	if (!Select(&result, ss.str().c_str())) {
		LogError(LOG_ITEM, 0, "Error loading items list!");
		return;
	}

	std::unordered_map<uint32_t, std::shared_ptr<const Item>> loadList;
	result.Next();
	//Unique id
	uint32_t nextID = result.GetUInt32(0);
	result.NextResultSet();
	result.Next();
	//Item count
	loadList.reserve(result.GetUInt32(0));
	result.NextResultSet();
	//Generic items
	while (result.Next()) {
		auto item = std::make_shared<ItemGeneric>();
		ProcessItemTableResult(result, item);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Melee weapons
	while (result.Next()) {
		auto item = std::make_shared<ItemMeleeWeapon>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemWeaponResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Ranged weapons
	while (result.Next()) {
		auto item = std::make_shared<ItemRangedWeapon>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemRangeResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Armor
	while (result.Next()) {
		auto item = std::make_shared<ItemArmor>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemArmorResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Shield
	while (result.Next()) {
		auto item = std::make_shared<ItemShield>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemShieldResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Bag
	while (result.Next()) {
		auto item = std::make_shared<ItemBag>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemBagResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Provisions
	while (result.Next()) {
		auto item = std::make_shared<ItemProvision>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemProvisionResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Recipe
	while (result.Next()) {
		auto item = std::make_shared<ItemRecipeBook>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemRecipeResult(result, item, i);
		//TODO: Load recipe item data once we have it parsed
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//House
	while (result.Next()) {
		auto item = std::make_shared<ItemHouse>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemHouseResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Bauble
	while (result.Next()) {
		auto item = std::make_shared<ItemBauble>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemBaubleResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Ammo
	while (result.Next()) {
		auto item = std::make_shared<ItemAmmo>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemAmmoResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//House container
	while (result.Next()) {
		auto item = std::make_shared<ItemHouseContainer>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemHouseContainerResult(result, item, i);
		loadList[item->itemID] = item;
	}
	result.NextResultSet();
	//Book
	while (result.Next()) {
		auto item = std::make_shared<ItemBook>();
		uint32_t i = ProcessItemTableResult(result, item);
		ProcessItemBookResult(result, item, i);
		loadList[item->itemID] = item;
	}
	std::map<uint32_t, std::shared_ptr<ItemRewardVoucher>> rewardVouchers;
	result.NextResultSet();
	//RewardVoucher
	while (result.Next()) {		
		uint32_t id = result.GetUInt32(0);
		auto itr = rewardVouchers.find(id);
		if (itr != rewardVouchers.end()) {
			continue;
		}

		auto item = std::make_shared<ItemRewardVoucher>();
		item->ItemRewardVoucherData::unknown1 = 0;
		item->ItemRewardVoucherData::unknown2 = 0;
		rewardVouchers[id] = item;
		loadList[id] = item;
		ProcessItemTableResult(result, item);
	}
	result.NextResultSet();
	while (result.Next()) {
		uint32_t id = result.GetUInt32(1);
		auto item = rewardVouchers[id];
		ProcessItemRewardVoucherResult(result, item);
	}
	rewardVouchers.clear();

	LogInfo(LOG_DATABASE, 0, "Successfully loaded %u items.", static_cast<uint32_t>(loadList.size()));;

	masterItems.AssignItems(loadList, nextID);
}

uint32_t ZoneDatabase::ProcessItemTableResult(DatabaseResult& result, const std::shared_ptr<Item>& item) {
	uint32_t i = 0;
	item->itemID = result.GetUInt32(i++);
	item->itemName = result.GetString(i++);
	EItemType type = Item::GetItemTypeFromName(result.GetString(i++));
	if (type == EItemType::EINVALID) {
		item->itemType = static_cast<uint8_t>(EItemType::EGENERIC);
	}
	else {
		item->itemType = static_cast<uint8_t>(type);
	}
	item->icon = result.GetUInt16(i++);
	item->stackSize = 1;
	i++;
	item->tier = result.GetUInt8(i++);
	item->skillReq1 = result.GetUInt32(i++);
	item->skillReq2 = result.GetUInt32(i++);
	item->skillMin = result.GetUInt16(i++);
	item->weight = result.GetInt32(i++);
	item->description = result.GetString(i++);
	i++;
	uint64_t flags = 0;
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_ATTUNEABLE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_ARTIFACT;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_LORE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_TEMPORARY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_TRADE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_VALUE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_ZONE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_DESTROY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_CRAFTED;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_GOOD_ONLY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_EVIL_ONLY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_STACK_LORE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_LORE_EQUIP;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_TRANSMUTE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_CURSED;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_ORNATE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_HEIRLOOM;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_APPEARANCE_ONLY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_UNLOCKED;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_REFORGED;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_REPAIR;
	}
	i++;
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_REFINED;
	}
	i++;
	item->flags = flags;
	item->bUseable = result.GetBool(i++);
	item->slotBitmask = result.GetUInt32(i++);
	i++;
	//Do not set the set name because for some stupid reason set data wasn't parsed and that adds elements to the packet...
	//item->setName = result.GetString(i++);
	i += 2;
	item->stackSize = result.GetUInt32(i++);
	item->bCollectable = result.GetBool(i++);
	
	for (int x = 0; x < 6; x++) {
		item->adornSlots[x] = result.GetUInt8(i++);
	}

	//Several fields which we arent using yet..
	i += 5;

	item->chargesMax = result.GetUInt16(i++);
	item->chargesRemaining = item->chargesMax;
	i++;
	item->recommendedLevel = result.GetUInt8(i++);
	item->requiredLevel = result.GetUInt8(i++);
	i++;
	item->requiredClasses = result.GetUInt64(i++);
	item->requiredClasses2 = item->requiredClasses;
	i++;
	//Skipping SOE fields
	i += 2;
	i++;
	i++;
	return i;
}

uint32_t ZoneDatabase::ProcessItemWeaponResult(DatabaseResult& result, const std::shared_ptr<ItemMeleeWeapon>& item, uint32_t i) {
	i += 2;
	item->wieldType = result.GetUInt8(i++);
	item->damageType = result.GetUInt8(i++);
	item->minDmg = result.GetInt16(i++);
	item->maxDmg = result.GetInt16(i++);
	item->masteryMinDmg = result.GetInt16(i++);
	item->masteryMaxDmg = result.GetInt16(i++);
	item->baseMinDamage = result.GetInt16(i++);
	item->baseMaxDamage = result.GetInt16(i++);
	item->delay = result.GetUInt8(i++);
	item->damageRating = result.GetFloat(i++);
	item->itemScore = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemRangeResult(DatabaseResult& result, const std::shared_ptr<ItemRangedWeapon>& item, uint32_t i) {
	i += 2;
	item->minDmg = result.GetInt16(i++);
	item->maxDmg = result.GetInt16(i++);
	item->masteryMinDmg = result.GetInt16(i++);
	item->masteryMaxDmg = result.GetInt16(i++);
	item->baseMinDamage = result.GetInt16(i++);
	item->baseMaxDamage = result.GetInt16(i++);
	item->delay = result.GetUInt8(i++);
	item->minRange = result.GetUInt16(i++);
	item->maxRange = result.GetUInt16(i++);
	item->damageRating = result.GetFloat(i++);
	item->damageType = result.GetUInt8(i++);
	item->itemScore = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemArmorResult(DatabaseResult& result, const std::shared_ptr<ItemArmor>& item, uint32_t i) {
	i += 2;
	item->mitigationLow = result.GetInt32(i++);
	item->mitigationHigh = result.GetInt32(i++);
	item->itemScore = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemShieldResult(DatabaseResult& result, const std::shared_ptr<ItemShield>& item, uint32_t i) {
	i += 2;
	item->mitigationLow = result.GetInt32(i++);
	item->mitigationHigh = result.GetInt32(i++);
	item->itemScore = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemBagResult(DatabaseResult& result, const std::shared_ptr<ItemBag>& item, uint32_t i) {
	i += 2;
	item->numSlots = result.GetUInt8(i++);
	item->weightReduction = result.GetUInt8(i++);
	item->bagUnknown1 = 0;
	item->bagUnknown2 = 0;
	item->bagUnknown3 = 0;
	memset(item->bagUnknown4, 0, sizeof(item->bagUnknown4));
	item->numEmptySlots = item->numSlots;
	item->numItems = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemProvisionResult(DatabaseResult& result, const std::shared_ptr<ItemProvision>& item, uint32_t i) {
	i += 2;
	item->provisionType = result.GetUInt8(i++);
	item->provisionLevel = result.GetInt16(i++);
	item->duration = result.GetFloat(i++);
	return ++i;
}

uint32_t ZoneDatabase::ProcessItemRecipeResult(DatabaseResult& result, const std::shared_ptr<ItemRecipeBook>& item, uint32_t i) {
	i += 2;
	item->numUses = result.GetUInt16(i++);
	item->bScribed = false;
	return i;
}

void ZoneDatabase::ProcessItemRecipeItemsResult(DatabaseResult& result, const std::unordered_map<uint32_t, std::shared_ptr<ItemRecipeBook> >& items) {
	//uint32_t i = 1;
	//uint32_t itemID = result.GetUInt32(i++);
	//auto itr = items.find(itemID);
	//if (itr == items.end()) {
	//	LogError(LOG_DATABASE, 0, "Unable to find recipe book with id %u to add recipe items.", itemID);
	//	return;
	//}

	//const std::shared_ptr<ItemRecipeBook>& item = itr->second;
	//item->items.emplace_back();
	//RecipeBookItem& recipeItemEntry = item->items.back();

	//recipeItemEntry.recipeID = itemID;
	//recipeItemEntry.
}

uint32_t ZoneDatabase::ProcessItemHouseResult(DatabaseResult& result, const std::shared_ptr<ItemHouse>& item, uint32_t i) {
	i += 3;
	item->rentStatusReduction = result.GetInt32(i++);
	item->rentCoinReduction = result.GetFloat(i++);
	item->houseType = result.GetUInt8(i++);
	item->ItemHouseData::unknown1 = 0;
	item->ItemHouseData::unknown2 = 0xFF;
	return i;
}

uint32_t ZoneDatabase::ProcessItemBaubleResult(DatabaseResult& result, const std::shared_ptr<ItemBauble>& item, uint32_t i) {
	i += 2;
	item->castTime = result.GetInt16(i++);
	item->recovery = result.GetInt16(i++);
	item->duration = result.GetInt32(i++);
	item->recast = result.GetFloat(i++);
	item->bDisplaySlotOptional = result.GetBool(i++);
	item->bDisplayCastTime = result.GetBool(i++);
	item->bDisplayBaubleType = result.GetBool(i++);
	item->effectRadius = result.GetFloat(i++);
	item->maxAoeTargets = result.GetInt32(i++);
	item->bDisplayUntilCancelled = result.GetBool(i++);
	return i;
}

uint32_t ZoneDatabase::ProcessItemAmmoResult(DatabaseResult& result, const std::shared_ptr<ItemAmmo>& item, uint32_t i) {
	i += 2;
	item->range = result.GetInt32(i++);
	item->damageModifier = result.GetInt32(i++);
	i++;
	item->hitBonus = result.GetFloat(i++);
	item->damageType = result.GetUInt32(i++);
	return i;
}

uint32_t ZoneDatabase::ProcessItemHouseContainerResult(DatabaseResult& result, const std::shared_ptr<ItemHouseContainer>& item, uint32_t i) {
	i += 2;
	item->numSlots = result.GetUInt16(i++);
	item->allowedTypes = result.GetUInt32(i++);
	//Not sure if unknown12 in the db is this value or the other 4 bytes of allowedTypes
	item->ItemHouseContainerData::unknown1 = result.GetUInt32(i++);
	item->unknown2 = result.GetUInt8(i++);
	item->brokerCommission = result.GetUInt16(i++);
	item->fenceCommission = result.GetUInt16(i++);
	return i;
}

uint32_t ZoneDatabase::ProcessItemBookResult(DatabaseResult& result, const std::shared_ptr<ItemBook>& item, uint32_t i) {
	i += 2;
	item->language = result.GetUInt8(i++);
	item->author = result.GetString(i++);
	item->title = result.GetString(i++);
	//TODO: add in this data for books
	memset(&item->houseData, 0, sizeof(item->houseData));
	return i;
}

uint32_t ZoneDatabase::ProcessItemRewardVoucherResult(DatabaseResult& result, const std::shared_ptr<ItemRewardVoucher>& item) {
	uint32_t i = 0;
	item->items.emplace_back();
	RewardVoucherItem& reward = item->items.back();
	reward.itemID = result.GetUInt32(i++);
	reward.icon = result.GetUInt16(i++);
	reward.itemName = result.GetString(i++);
	reward.crc = 0;
	reward.unknown = 0;
	return i;
}