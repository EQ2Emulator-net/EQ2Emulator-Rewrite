#include "stdafx.h"

#include "../Database/ZoneDatabase.h"
#include "MasterItemList.h"
#include "../../common/log.h"
#include "Item.h"
#include "ItemTypes.h"
#include "../Packets/ItemPackets.h"

void ZoneDatabase::LoadMasterItems(MasterItemList& masterItems) {
	std::ostringstream ss;

	std::unordered_map<uint32_t, std::vector<ItemStatMod>> stats;
	std::unordered_map<uint32_t, std::vector<ItemStringMod>> stringStats;

	auto LoadStats = [this, &stats, &stringStats] {
		DatabaseResult result;
		if (!Select(&result, "SELECT item_id, type, subtype, value, text, description FROM item_stats;")) {
			LogError(LOG_DATABASE, 0, "Error loading item stats.");
			return;
		}

		while (result.Next()) {
			uint32_t itemID = result.GetUInt32(0);
			uint8_t type = result.GetUInt8(1);
			std::string text = result.GetString(4);

			if (type == 1 || text.empty()) {
				std::vector<ItemStatMod>& mods = stats[itemID];
				mods.emplace_back();
				ItemStatMod& mod = mods.back();
				mod.statType = type;
				mod.statSubtype = result.GetInt16(2);
				if (mod.statType == 6) {
					mod.fValue = result.GetFloat(3);
				}
				else {
					mod.iValue = result.GetInt32(3);
				}
				mod.stringVal = std::move(text);
				mod.unknown64 = 0;
				mod.unknown83 = 0;
				mod.unknown89 = 0;
				mod.unknown92 = 0;
			}
			else {
				std::vector<ItemStringMod>& mods = stringStats[itemID];
				mods.emplace_back();
				ItemStringMod& mod = mods.back();
				mod.stringVal = std::move(text);
				mod.statDescription = result.GetString(5);
				mod.unknown1 = 0;
				mod.unknown2 = 0;
				mod.unknown3 = 0;
				mod.unknown4 = 0;
				mod.unknown5 = 0;
			}
		}
	};

	std::unordered_map<uint32_t, EQ2EquipmentItem> appearances;

	auto LoadAppearances = [this, &appearances] {
		DatabaseResult result;
		if (!Select(&result, "SELECT* FROM item_appearances;")) {
			LogError(LOG_DATABASE, 0, "Error loading item stats.");
			return;
		}

		appearances.reserve(result.GetNumRows());

		while (result.Next()) {
			uint32_t i = 1;
			EQ2EquipmentItem& app = appearances[result.GetUInt32(i++)];
			app.type = result.GetUInt32(i++);
			app.color.Red = result.GetUInt8(i++);
			app.color.Green = result.GetUInt8(i++);
			app.color.Blue = result.GetUInt8(i++);
			app.highlight.Red = result.GetUInt8(i++);
			app.highlight.Green = result.GetUInt8(i++);
			app.highlight.Blue = result.GetUInt8(i++);
		}
	};

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

	std::thread statThread(LoadStats);
	std::thread appearanceThread(LoadAppearances);

	DatabaseResult result;
	if (!Select(&result, ss.str().c_str())) {
		LogError(LOG_ITEM, 0, "Error loading items list!");
		return;
	}

	std::unordered_map<uint32_t, std::shared_ptr<Item>> loadList;
	result.Next();
	//Unique id
	uint32_t nextID = result.GetUInt32(0);
	result.NextResultSet();
	result.Next();
	//Item count
	loadList.reserve(result.GetUInt32(0));
	result.NextResultSet();
	
	for (int i = 0; i < 14; i++) {
		while (result.Next()) {
			EItemType type = Item::GetItemTypeFromName(result.GetString(i++));

			if (type == EItemType::EINVALID) {
				type = EItemType::EGENERIC;
			}

			auto item = Item::CreateItemWithType(type);
			uint32_t i = ProcessItemTableResult(result, item);
			item->LoadTypeSpecificData(result, i);
			loadList[item->itemID] = item;
		}
		result.NextResultSet();
	}

	statThread.join();
	appearanceThread.join();

	//Move loaded stats over
	for (auto& itr : stats) {
		auto f = loadList.find(itr.first);
		if (f == loadList.end()) {
			continue;
		}

		f->second->statMods = std::move(itr.second);
	}
	for (auto& itr : stringStats) {
		auto f = loadList.find(itr.first);
		if (f == loadList.end()) {
			continue;
		}

		f->second->stringMods = std::move(itr.second);
	}

	//Move loaded appearances
	for (auto& itr : appearances) {
		auto f = loadList.find(itr.first);
		if (f == loadList.end()) {
			continue;
		}

		f->second->appearance.emplace(itr.second);
	}

	LogInfo(LOG_DATABASE, 0, "Successfully loaded %u items.", static_cast<uint32_t>(loadList.size()));;

	masterItems.AssignItems(reinterpret_cast<std::unordered_map<uint32_t, std::shared_ptr<const Item>>&>(loadList), nextID);
}

uint32_t ZoneDatabase::ProcessItemTableResult(DatabaseResult& result, const std::shared_ptr<Item>& item) {
	uint32_t i = 0;
	item->itemID = result.GetUInt32(i++);
	item->itemName = result.GetString(i++);
	//Set the item type before this function
	i++;
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

void ItemMeleeWeapon::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	wieldType = result.GetUInt8(i++);
	damageType = result.GetUInt8(i++);
	minDmg = result.GetInt16(i++);
	maxDmg = result.GetInt16(i++);
	masteryMinDmg = result.GetInt16(i++);
	masteryMaxDmg = result.GetInt16(i++);
	baseMinDamage = result.GetInt16(i++);
	baseMaxDamage = result.GetInt16(i++);
	delay = result.GetUInt8(i++);
	damageRating = result.GetFloat(i++);
	itemScore = 0;
}

void ItemRangedWeapon::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	minDmg = result.GetInt16(i++);
	maxDmg = result.GetInt16(i++);
	masteryMinDmg = result.GetInt16(i++);
	masteryMaxDmg = result.GetInt16(i++);
	baseMinDamage = result.GetInt16(i++);
	baseMaxDamage = result.GetInt16(i++);
	delay = result.GetUInt8(i++);
	minRange = result.GetUInt16(i++);
	maxRange = result.GetUInt16(i++);
	damageRating = result.GetFloat(i++);
	damageType = result.GetUInt8(i++);
	itemScore = 0;
}

void ItemArmor::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	mitigationLow = result.GetInt32(i++);
	mitigationHigh = result.GetInt32(i++);
	itemScore = 0;
}

void ItemShield::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	mitigationLow = result.GetInt32(i++);
	mitigationHigh = result.GetInt32(i++);
	itemScore = 0;
}

void ItemBag::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	numSlots = result.GetUInt8(i++);
	weightReduction = result.GetUInt8(i++);
	bagUnknown1 = 0;
	bagUnknown2 = 0;
	bagUnknown3 = 0;
	memset(bagUnknown4, 0, sizeof(bagUnknown4));
	numEmptySlots = numSlots;
	numItems = 0;
}

void ItemProvision::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	provisionType = result.GetUInt8(i++);
	provisionLevel = result.GetInt16(i++);
	duration = result.GetFloat(i++);
}

void ItemRecipeBook::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	numUses = result.GetUInt16(i++);
	bScribed = false;
}

void ItemHouse::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 3;
	rentStatusReduction = result.GetInt32(i++);
	rentCoinReduction = result.GetFloat(i++);
	houseType = result.GetUInt8(i++);
	ItemHouseData::unknown1 = 0;
	ItemHouseData::unknown2 = 0xFF;
}

void ItemBauble::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	castTime = result.GetInt16(i++);
	recovery = result.GetInt16(i++);
	duration = result.GetInt32(i++);
	recast = result.GetFloat(i++);
	bDisplaySlotOptional = result.GetBool(i++);
	bDisplayCastTime = result.GetBool(i++);
	bDisplayBaubleType = result.GetBool(i++);
	effectRadius = result.GetFloat(i++);
	maxAoeTargets = result.GetInt32(i++);
	bDisplayUntilCancelled = result.GetBool(i++);
}

void ItemAmmo::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	range = result.GetInt32(i++);
	damageModifier = result.GetInt32(i++);
	i++;
	hitBonus = result.GetFloat(i++);
	damageType = result.GetUInt32(i++);
}

void ItemHouseContainer::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	numSlots = result.GetUInt16(i++);
	allowedTypes = result.GetUInt32(i++);
	//Not sure if unknown12 in the db is this value or the other 4 bytes of allowedTypes
	ItemHouseContainerData::unknown1 = result.GetUInt32(i++);
	unknown2 = result.GetUInt8(i++);
	brokerCommission = result.GetUInt16(i++);
	fenceCommission = result.GetUInt16(i++);
}

void ItemBook::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	language = result.GetUInt8(i++);
	author = result.GetString(i++);
	title = result.GetString(i++);
	//TODO: add in this data for books
	memset(&houseData, 0, sizeof(houseData));
}

void ItemRewardVoucher::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i = 2;
	items.emplace_back();
	RewardVoucherItem& reward = items.back();
	reward.itemID = result.GetUInt32(i++);
	reward.icon = result.GetUInt16(i++);
	reward.itemName = result.GetString(i++);
	reward.crc = 0;
	reward.unknown = 0;
}

void ItemGeneric::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}

//The below classes are still TODO
void ItemAdornment::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}

void ItemAchievementProfile::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}

void ItemReforgingDecoration::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}

void ItemRewardCrate::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}