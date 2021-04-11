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
	std::unordered_map<uint32_t, std::vector<ItemEffect>> effects;

	auto LoadStats = [this, &stats, &stringStats] {
		DatabaseResult result;
		if (!Select(&result, "SELECT item_id, type, subtype, iValue, fValue, sValue FROM item_mod_stats WHERE item_id < 10000000;")) {
			LogError(LOG_DATABASE, 0, "Error loading item stats.");
			return;
		}

		while (result.Next()) {
			uint32_t itemID = result.GetUInt32(0);
			uint8_t type = result.GetUInt8(1);

			std::vector<ItemStatMod>& mods = stats[itemID];
			mods.emplace_back();
			ItemStatMod& mod = mods.back();
			mod.statType = type;
			mod.statSubtype = result.GetInt16(2);
			if (mod.statType == 6) {
				mod.fValue = result.GetFloat(4);
				mod.statAsFloat = mod.fValue;
			}
			else {
				mod.iValue = result.GetInt32(3);
				mod.statAsFloat = static_cast<float>(mod.iValue);
			}
			mod.stringVal = result.GetString(5);
			mod.unknown64 = 0;
			mod.unknown92 = 0;
			//This is set to something else for string stats but not sure what it does
			mod.stringStatUnknown = -1;
		}

		if (!Select(&result, "SELECT item_id, `mod`, `description` FROM item_mod_strings WHERE item_id < 10000000;")) {
			LogError(LOG_DATABASE, 0, "Error loading item mod strings.");
			return;
		}

		while (result.Next()) {
			uint32_t itemID = result.GetUInt32(0);
			std::vector<ItemStringMod>& mods = stringStats[itemID];

			mods.emplace_back();
			ItemStringMod& m = mods.back();
			m.stringVal = result.GetString(1);
			m.statDescription = result.GetString(2);
			m.unknown1 = 0;
			m.unknown2 = 0;
			m.unknown3 = 0;
			m.unknown4 = 0;
			m.unknown5 = 0;
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

	auto LoadEffects = [this, &effects] {
		DatabaseResult result;
		if (!Select(&result, "SELECT * FROM item_effects ORDER BY item_id, `index`;")) {
			LogError(LOG_DATABASE, 0, "Error loading item stats.");
			return;
		}

		effects.reserve(result.GetNumRows() / 2);

		while (result.Next()) {
			uint32_t i = 1;
			auto& vec = effects[result.GetUInt32(1)];
			vec.emplace_back();
			ItemEffect& e = vec.back();
			e.effectText = result.GetString(2);
			e.percentage = result.GetUInt8(3);
			e.tabIndex = result.GetUInt16(4);
		}
	};

	LogInfo(LOG_DATABASE, 0, "Loading items...");

	ss << "SELECT COUNT(id) FROM items WHERE bPvpDesc = 0;\n"
		<< "SELECT id, set_name FROM item_itemsets;\n"
		<< "SELECT id, set_id, num_items_needed FROM item_itemset_bonus ORDER BY set_id, `index`;\n"
		<< "SELECT sb.set_id, sbe.set_bonus_id, sbe.indent, sbe.description, sbe.percentage FROM item_itemset_bonus_effects sbe INNER JOIN item_itemset_bonus sb ON sbe.set_bonus_id = sb.id ORDER BY set_bonus_id, effect_order;\n"
		<< "SELECT sb.set_id, sbs.set_bonus_id, sbs.type, sbs.subtype, sbs.iValue, sbs.fValue, sbs.sValue FROM item_itemset_bonus_stats sbs INNER JOIN item_itemset_bonus sb ON sbs.set_bonus_id = sb.id ORDER BY set_bonus_id, stats_order;\n"
		<< "SELECT * FROM item_itemset_items ORDER BY set_id, `index`;\n"
		<< "SELECT * FROM items WHERE bPvpDesc = 0 AND item_type IN ('Normal', 'Dungeon Maker');\n"
		<< "SELECT * FROM items i INNER JOIN item_details_weapon idw ON bPvpDesc = 0 AND  i.id = idw.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_range idr ON bPvpDesc = 0 AND  i.id = idr.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_armor ida ON bPvpDesc = 0 AND  i.id = ida.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_shield ids ON bPvpDesc = 0 AND  i.id = ids.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_bag idb ON bPvpDesc = 0 AND  i.id = idb.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_food idf ON bPvpDesc = 0 AND  i.id = idf.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_recipe idr ON bPvpDesc = 0 AND  i.id = idr.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_house idh ON i.bPvpDesc = 0 AND i.item_type = 'House' AND  i.id = idh.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_bauble idb ON bPvpDesc = 0 AND  i.id = idb.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_thrown idt ON bPvpDesc = 0 AND  i.id = idt.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_house_container idhc ON bPvpDesc = 0 AND  i.id = idhc.item_id;\n"
		<< "SELECT i.*, idb.*, idh.* FROM items i INNER JOIN item_details_book idb ON i.bPvpDesc = 0 AND i.id = idb.item_id INNER JOIN item_details_house idh ON idb.item_id = idh.item_id;\n"
		<< "SELECT * FROM items WHERE bPvpDesc = 0 AND  id IN (SELECT DISTINCT voucher_item_id FROM item_details_reward_voucher);\n"
		<< "SELECT * FROM items i INNER JOIN item_details_reward_crate irc ON i.bPvpDesc = 0 AND irc.item_id = i.id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_adornments ida ON i.bPvpDesc = 0 AND i.id = ida.item_id;\n"
		<< "SELECT * FROM items i INNER JOIN item_details_decorations idd ON i.bPvpDesc = 0 AND i.id = idd.item_id;\n"
		<< "SELECT * from items i INNER JOIN item_details_achievement_profile ida ON i.bPvpDesc = 0 AND i.id = ida.item_id;\n"
		<< "SELECT * FROM item_details_reward_voucher;\n"
		<< "SELECT * FROM item_details_reward_crate_item;\n"
		<< "SELECT * FROM item_details_recipe_items;\n";

	std::thread statThread(LoadStats);
	std::thread effectThread(LoadEffects);
	std::thread appearanceThread(LoadAppearances);

	DatabaseResult result;
	if (!Select(&result, ss.str().c_str())) {
		LogError(LOG_ITEM, 0, "Error loading items list!");
		return;
	}

	std::unordered_map<uint32_t, std::shared_ptr<Item>> loadList;
	result.Next();
	//Item count
	loadList.reserve(result.GetUInt32(0));
	result.NextResultSet();

	//Item Sets (temp, will need something more robust later)
	//Names first
	std::unordered_map<uint32_t, std::string> setNames;
	setNames.reserve(result.GetNumRows());
	while (result.Next()) {
		setNames[result.GetUInt32(0)] = result.GetString(1);
	}
	result.NextResultSet();

	std::unordered_map<uint32_t, ItemSetDetails> setDetails;
	setDetails.reserve(setNames.size());
	//Item Set Bonuses
	while (result.Next()) {
		ItemSetDetails& det = setDetails[result.GetUInt32(1)];
		det.setBonuses.emplace_back();
		ItemSetBonus& b = det.setBonuses.back();
		b.bonusID = result.GetUInt32(0);
		b.itemsNeeded = result.GetUInt8(2);
	}
	result.NextResultSet();

	//Item Set Bonus Effects
	while (result.Next()) {
		ItemSetDetails& det = setDetails[result.GetUInt32(0)];
		
		uint32_t bonusID = result.GetUInt32(1);
		for (auto& itr : det.setBonuses) {
			if (itr.bonusID == bonusID) {
				itr.effects.emplace_back();
				ItemSetBonusEffect& e = itr.effects.back();
				e.tabIndex = result.GetUInt8(2);
				e.effectText = result.GetString(3);
				e.percentage = result.GetUInt8(4);
				break;
			}
		}
	}
	result.NextResultSet();

	//Item Set Bonus Stats
	while (result.Next()) {
		ItemSetDetails& det = setDetails[result.GetUInt32(0)];

		uint32_t bonusID = result.GetUInt32(1);
		for (auto& itr : det.setBonuses) {
			if (itr.bonusID == bonusID) {
				itr.stats.emplace_back();
				ItemStatMod& mod = itr.stats.back();
				
				mod.statType = result.GetUInt8(2);
				mod.statSubtype = result.GetInt16(3);
				if (mod.statType == 6) {
					mod.fValue = result.GetFloat(5);
					mod.statAsFloat = mod.fValue;
				}
				else {
					mod.iValue = result.GetInt32(4);
					mod.statAsFloat = static_cast<float>(mod.iValue);
				}
				mod.stringVal = result.GetString(6);
				mod.unknown64 = 0;
				mod.unknown92 = 0;
				break;
			}
		}
	}
	result.NextResultSet();

	//Set Items
	while (result.Next()) {
		ItemSetDetails& det = setDetails[result.GetUInt32(1)];
		det.setItems.emplace_back();
		ItemSetItem& i = det.setItems.back();
		i.itemName = result.GetString(3);
		i.unknown1 = 0;
		i.unknown2 = 0;
	}
	result.NextResultSet();

	//Finish up sets
	for (auto& itr : setDetails) {
		auto& det = itr.second;
		det.numItemsInSet = static_cast<uint8_t>(det.setItems.size());
	}
	//Finished loading set info
	
	for (int i = 0; i < 18; i++) {
		while (result.Next()) {
			EItemType type = Item::GetItemTypeFromName(result.GetString(3));

			if (type == EItemType::EINVALID) {
				type = EItemType::EGENERIC;
			}

			auto item = Item::CreateItemWithType(type);
			item->itemType = static_cast<uint8_t>(type);
			uint32_t i = ProcessItemTableResult(result, item);
			item->LoadTypeSpecificData(result, i);

			if (item->set_id) {
				item->setName = setNames[item->set_id];
				item->setDetails = setDetails[item->set_id];
			}

			loadList[item->itemID] = item;
		}
		result.NextResultSet();
	}

	//item_details_reward_voucher
	while (result.Next()) {
		uint32_t id = result.GetUInt32(1);

		auto itr = loadList.find(id);
		if (itr == loadList.end()) {
			continue;
		}

		auto item = std::dynamic_pointer_cast<ItemRewardVoucher>(itr->second);
		EmuAssert(item);

		ProcessItemRewardVoucherResult(result, item);
	}
	result.NextResultSet();

	//item_details_reward_crate_item
	while (result.Next()) {
		uint32_t id = result.GetUInt32(1);

		auto itr = loadList.find(id);
		if (itr == loadList.end()) {
			continue;
		}

		auto item = std::dynamic_pointer_cast<ItemRewardCrate>(itr->second);
		EmuAssert(item);

		ProcessItemRewardCrateResult(result, item);
	}
	result.NextResultSet();

	while (result.Next()) {
		uint32_t id = result.GetUInt32(1);

		auto itr = loadList.find(id);
		if (itr == loadList.end()) {
			continue;
		}

		auto item = std::dynamic_pointer_cast<ItemRecipeBook>(itr->second);
		EmuAssert(item);

		item->items.emplace_back();
		RecipeBookItem& i = item->items.back();
		i.recipeName = result.GetString(2);
		i.icon = result.GetUInt16(3);
		i.recipeID = result.GetUInt32(4);
	}

	statThread.join();
	effectThread.join();
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

	for (auto& itr : effects) {
		auto f = loadList.find(itr.first);
		if (f == loadList.end()) {
			continue;
		}

		f->second->effects = std::move(itr.second);
	}

	LogInfo(LOG_DATABASE, 0, "Successfully loaded %u items.", static_cast<uint32_t>(loadList.size()));;

	masterItems.AssignItems(reinterpret_cast<std::unordered_map<uint32_t, std::shared_ptr<const Item>>&>(loadList));
}

uint32_t ZoneDatabase::ProcessItemTableResult(DatabaseResult& result, const std::shared_ptr<Item>& item) {
	uint32_t i = 0;
	item->itemID = result.GetUInt32(i++);
	//bPvpDesc
	i++;
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
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_ETHEREAL;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_REFINED;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_SALVAGE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_INDESTRUCTIBLE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_NO_EXPERIMENT;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_HOUSE_LORE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_UNK_28;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_BUILDING_BLOCK;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_FREE_REFORGE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_INFUSABLE;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_MERC_ONLY;
	}
	if (result.GetBool(i++)) {
		flags |= Item::ITEM_FLAG_MOUNT_ONLY;
	}
	item->flags = flags;
	item->bUseable = result.GetBool(i++);
	item->slotBitmask = result.GetUInt32(i++);
	item->set_id = result.GetUInt32(i++);
	i++;
	item->sell_status_amount = result.GetUInt32(i++);
	item->stackSize = result.GetUInt32(i++);
	item->bCollectable = result.GetBool(i++);
	item->bQuestExamine = result.GetBool(i++);
	
	for (int x = 0; x < 6; x++) {
		item->adornSlots[x] = result.GetUInt8(i++);
	}

	//Offers quest id
	i++;

	item->offersQuest = result.GetString(i++);
	item->requiredByQuest = result.GetString(i++);

	i += 2;

	item->chargesMax = result.GetUInt16(i++);
	item->chargesRemaining = item->chargesMax;
	item->bHasCharges = result.GetBool(i++);
	item->recommendedLevel = result.GetUInt8(i++);
	i += 2;
	item->requiredLevel = result.GetUInt8(i++);
	item->requiredClasses = result.GetUInt64(i++);
	item->requiredClasses2 = item->requiredClasses;
	i++;
	//Skipping SOE fields
	i += 4;
	i++;
	item->bTransmutedMaterial = result.GetBool(i++);
	item->bHarvestedMaterial = result.GetBool(i++);
	//Skipping body_drop, no_buy_back, unk fields
	i += 10;
	return i;
}

void ItemMeleeWeapon::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
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
	itemScore = result.GetUInt32(i++);
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
	itemScore = result.GetUInt32(i++);
}

void ItemArmor::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	mitigationLow = result.GetInt32(i++);
	mitigationHigh = result.GetInt32(i++);
	absorb = result.GetInt16(i++);
	i++;
	itemScore = result.GetUInt32(i++);
}

void ItemShield::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	mitigationLow = result.GetInt32(i++);
	mitigationHigh = result.GetInt32(i++);
	itemScore = result.GetUInt32(i++);
}

void ItemBag::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	numSlots = result.GetUInt8(i++);
	weightReduction = result.GetUInt8(i++);
	bagUnknown1 = result.GetUInt8(i++);
	bBackpack = result.GetBool(i++);
	bagUnknown3 = result.GetUInt8(i++);
	bagUnknown4 = result.GetUInt32(i++);
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
	itemScore = result.GetUInt32(i++);
}

void ItemAmmo::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	i += 2;
	range = result.GetInt32(i++);
	damageModifier = result.GetInt32(i++);
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
	

	//item_details_house info
	i += 3;
	houseData.rentStatusReduction = result.GetInt32(i++);
	houseData.rentCoinReduction = result.GetFloat(i++);
	houseData.houseType = result.GetUInt8(i++);
	houseData.ItemHouseData::unknown1 = 0;
	houseData.ItemHouseData::unknown2 = 0xFF;
}

void ItemRewardVoucher::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	unknown1 = 0;
	unknown2 = 0;
}

void ItemGeneric::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {

}

void ItemRewardCrate::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {
	i += 2;
	unknown1 = res.GetUInt8(i++);
	unknown2 = res.GetUInt8(i++);
}

void ItemSpellScroll::LoadTypeSpecificData(DatabaseResult& result, uint32_t i) {
	//implement
}

uint32_t ZoneDatabase::ProcessItemRewardVoucherResult(DatabaseResult& result, const std::shared_ptr<ItemRewardVoucher>& item) {
	uint32_t i = 2;
	item->items.emplace_back();
	RewardVoucherItem& reward = item->items.back();
	reward.itemID = result.GetUInt32(i++);
	i++;
	reward.crc = result.GetUInt32(i++);
	reward.icon = result.GetUInt16(i++);
	reward.itemName = result.GetString(i++);
	reward.unknown = 0;
	return i;
}

uint32_t ZoneDatabase::ProcessItemRewardCrateResult(DatabaseResult& result, const std::shared_ptr<ItemRewardCrate>& item) {
	uint32_t i = 2;
	item->items.emplace_back();
	RewardCrateItem& reward = item->items.back();
	reward.itemID = result.GetUInt32(i++);
	i++;
	reward.crc = result.GetUInt32(i++);
	reward.icon = result.GetUInt16(i++);
	reward.stackSize = result.GetUInt32(i++);
	reward.colorID = result.GetUInt32(i++);
	reward.itemName = result.GetString(i++);
	reward.language = result.GetUInt8(i++);
	return i;
}

void ItemAdornment::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {
	i += 2;
	duration = res.GetFloat(i++);
	itemTypes = res.GetUInt64(i++);
	slotColor = res.GetUInt16(i++);
	ItemAdornmentData::description = res.GetString(i++);
	description2 = res.GetString(i++);
	unknown = res.GetUInt32(i++);
	unknown2 = res.GetUInt32(i++);
}

void ItemReforgingDecoration::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {
	i += 2;
	decoName = res.GetString(i++);
}

void ItemAchievementProfile::LoadTypeSpecificData(DatabaseResult& res, uint32_t i) {
	i += 2;
	rentStatusReduction = res.GetUInt32(i++);
	rentCoinReduction = res.GetFloat(i++);
	houseType = res.GetUInt8(i++);
	unknownString = res.GetString(i++);
	unknown = res.GetUInt8(i++);
}