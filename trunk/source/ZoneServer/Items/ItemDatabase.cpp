#include "stdafx.h"

#include "../Database/ZoneDatabase.h"
#include "MasterItemList.h"
#include "../../common/log.h"
#include "../Items/Item.h"

void ZoneDatabase::LoadMasterItems(MasterItemList& masterItems) {
	std::ostringstream ss;

	LogInfo(LOG_DATABASE, 0, "Loading items...");

	ss << "SELECT MAX(id) FROM character_items;\n"
		<< "SELECT COUNT(id) FROM items;\n"
		<< "SELECT * FROM items WHERE item_type IN ('Normal', 'Dungeon Maker');\n";
		//<< "SELECT * FROM items i INNER JOIN item_details_weapon idw ON i.id = idw.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_range idr ON i.id = idr.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_armor ida ON i.id = ida.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_bag idb ON i.id = idb.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_recipe idr ON i.id = idr.item_id;\n"
		//<< "SELECT idri.item_id, i.icon, idri.name FROM items i INNER JOIN item_details_recipe_items idri ON i.id = idri.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_food idf ON i.id = idf.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_bauble idb ON i.id = idb.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_house idh ON i.id = idh.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_thrown idt ON i.id = idt.item_id;\n"
		//<< "SELECT * FROM items i INNER JOIN item_details_house_container idhc ON i.id = idhc.item_id;\n"
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
	item->skillMin = result.GetUInt32(i++);
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
	item->flags = flags;
	item->bUseable = result.GetBool(i++);
	item->slotBitmask = result.GetUInt32(i++);
	i++;
	//Do not set the set name because for some stupid reason set data wasn't parsed and that adds elements to the packet...
	//item->setName = result.GetString(i++);
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
	return i;
}

uint32_t ZoneDatabase::ProcessItemStatsResult(DatabaseResult& result, std::unordered_map<uint32_t, const std::shared_ptr<Item> >& outItems) {
	return 0;
}