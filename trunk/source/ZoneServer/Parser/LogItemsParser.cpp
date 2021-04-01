#include "stdafx.h"

#include "LogItemsParser.h"
#include "../Packets/ItemPackets.h"

LogItemsParser::LogItemsParser(PacketLog& plog, ParserDatabase& db) : LogParser(plog, db) {
	auto packets = log.FindPackets<OP_EqExamineInfoCmd_Packet, ExamineInfoCmd_Item_Packet>();

	if (!packets.empty()) {
		LoadExistingData();
	}

	LogDebug(LOG_PARSER, 0, "Item count for log %s : %llu", log.filename.c_str(), packets.size());
	for (auto& itr : packets) {
		auto& p = itr.second;
		int32_t id = p->itemDesc->header.itemID;
		//If we haven't parsed this item yet, do so now
		if (parsed_soe_items.insert(id).second) {
			ProcessItemDesc(p->itemDesc, false);
			if (p->pvpDesc) ProcessItemDesc(p->pvpDesc, true);
		}
	}
	ProcessQueuedInserts();
	LogDebug(LOG_PARSER, 0, "Done with log.");
}

void LogItemsParser::ProcessQueuedInserts() {
	DoInsertsForTable("items", 250);
	DoInsertsForTable("item_effects", 250);
	DoInsertsForTable("item_mod_stats", 250);
	DoInsertsForTable("item_mod_strings", 250);
	DoInsertsForTable("item_itemset_items", 250);
	DoInsertsForTable("item_details_armor", 250);
	DoInsertsForTable("item_details_weapon", 250);
	DoInsertsForTable("item_details_range", 250);
	DoInsertsForTable("item_details_shield", 250);
	DoInsertsForTable("item_details_bag", 250);
	DoInsertsForTable("item_details_skill", 250);
	DoInsertsForTable("item_details_recipe", 250);
	DoInsertsForTable("item_details_recipe_items", 250);
	DoInsertsForTable("item_details_food", 250);
	DoInsertsForTable("item_details_bauble", 250);
	DoInsertsForTable("item_details_house", 250);
	DoInsertsForTable("item_details_thrown", 250);
	DoInsertsForTable("item_details_house_container", 250);
	DoInsertsForTable("item_details_adornments", 250);
	DoInsertsForTable("item_details_achievement_profile", 250);
	DoInsertsForTable("item_details_reward_voucher", 250);
	DoInsertsForTable("item_details_reward_crate", 250);
	DoInsertsForTable("item_details_reward_crate_item", 250);
	DoInsertsForTable("item_details_book", 250);
	DoInsertsForTable("item_details_decorations", 250);
}

void LogItemsParser::LoadExistingData() {
	parsed_soe_items = database.LoadExistingItems();
	parsed_item_sets = database.LoadExistingItemSets();
	next_id = database.LoadNextItemID();
}

std::string LogItemsParser::GetItemTypeAsString(uint8_t type) {
	switch (static_cast<EItemType>(type)) {
	case EItemType::EGENERIC: return "Normal"; break;
	case EItemType::EMELEE_WEAPON: return "Weapon"; break;
	case EItemType::ERANGED_WEAPON: return "Ranged"; break;
	case EItemType::EARMOR: return "Armor"; break;
	case EItemType::ESHIELD: return "Shield"; break;
	case EItemType::EBAG: return "Bag"; break;
	case EItemType::ESPELL_SCROLL: return "Scroll"; break;
	case EItemType::ERECIPE_BOOK: return "Recipe"; break;
	case EItemType::EPROVISION: return "Food"; break;
	case EItemType::EBAUBLE: return "Bauble"; break;
	case EItemType::EHOUSE: return "House"; break;
	case EItemType::EAMMO: return "Thrown"; break;
	case EItemType::EHOUSE_CONTAINER: return "House Container"; break;
	case EItemType::EADORNMENT: return "Adornment"; break;
	case EItemType::EACHIEVEMENT_PROFILE: return "Profile"; break;
	case EItemType::EREWARD_VOUCHER: return "Pattern Set"; break;
	case EItemType::EREWARD_CRATE: return "Item Set"; break;
	case EItemType::EBOOK: return "Book"; break;
	case EItemType::EREFORGING_DECORATION: return "Decoration"; break;
	case EItemType::EDUNGEON_MAKER: return "Dungeon Maker"; break;
	case EItemType::EMARKETPLACE: return "Marketplace"; break;
	case EItemType::EREWARD_CRATE_2: return "Reward Crate2"; break;
	case EItemType::EINFUSER_1: return "Infuser1"; break;
	case EItemType::EINFUSER_2: return "Infuser2"; break;
	case EItemType::EEXPERIENCE_VIAL: return "Experience Vial"; break;
	case EItemType::EOVERSEER: return "Overseer"; break;
	default:
		throw "Invalid item type!!!";
		return "";
	}
}

uint32_t LogItemsParser::ProcessItemSetData(Substruct_ExamineDescItem* i, bool bPvp) {
	auto& f = i->footer;

	ItemSetKey key;
	key.bPvpDesc = bPvp;
	key.set_name = f.setName;
	key.item_level = f.recommendedLevel;
	key.id = 0;

	{
		//Check 
		auto itr = std::find(parsed_item_sets.begin(), parsed_item_sets.end(), key);
		if (itr != parsed_item_sets.end()) {
			return itr->id;
		}
	}

	key.id = database.CreateItemSet(f.setName, key.bPvpDesc, key.item_level);
	parsed_item_sets.push_back(key);
	//Set bonuses
	{
		uint32_t setBonusIndex = 0;
		for (auto& itr : f.itemSetDetails.setBonusArray) {
			uint32_t setBonusId = database.CreateItemSetBonus(key.id, setBonusIndex, itr.itemsNeeded);

			//This will hold inserts for set bonus effects and stats
			std::ostringstream queries;

			//Bonus effects
			uint32_t effectIndex = 0;
			for (auto& sbe : itr.effectArray) {
				DatabaseRow r;
				r.m_tableName = "item_itemset_bonus_effects";

				r.RegisterField("set_bonus_id", setBonusId);
				r.RegisterField("indent", sbe.tabIndex);
				r.RegisterField("description", sbe.effectText);
				r.RegisterField("percentage", sbe.percentage);
				r.RegisterField("effect_order", effectIndex);

				r.GenerateSingleInsert(queries);

				effectIndex++;
			}

			//Bonus stats
			uint32_t statIndex = 0;
			for (auto& stat : itr.statArray) {
				DatabaseRow r;
				r.m_tableName = "item_itemset_bonus_stats";

				r.RegisterField("set_bonus_id", setBonusId);
				r.RegisterField("type", stat.statType);
				r.RegisterField("subtype", stat.statSubtype);
				if (stat.statType != 6) {
					r.RegisterField("iValue", stat.iValue);
					r.RegisterField("fValue", SQLNull());
				}
				else {
					r.RegisterField("iValue", SQLNull());
					r.RegisterField("fValue", stat.fValue);
				}
				r.RegisterField("sValue", stat.stringVal);
				r.RegisterField("level", stat.statLevel);
				r.RegisterField("stats_order", statIndex);

				r.GenerateSingleInsert(queries);

				statIndex++;
			}

			//Now insert our entries for this bonus
			std::string insertions = queries.str();
			if (!insertions.empty()) {
				database.Query(insertions.c_str());
			}

			setBonusIndex++;
		}
	}

	//Set item list
	{
		uint32_t setItemIndex = 0;
		for (auto& itr : f.itemSetDetails.setItems) {
			DatabaseRow r;
			r.m_tableName = "item_itemset_items";

			r.RegisterField("set_id", key.id);
			r.RegisterField("item_name", itr.itemName);
			r.RegisterField("index", setItemIndex);
			QueueRowInsert(r);
			setItemIndex++;
		}
	}

	return key.id;
}

void LogItemsParser::ProcessItemDesc(Substruct_ExamineDescItem* item, bool bPvp) {
	item_id = next_id++;

	ProcessTypeSpecificData(item);

	DatabaseRow row;
	auto& h = item->header;
	auto& f = item->footer;

	row.m_tableName = "items";
	row.RegisterField("id", item_id);
	row.RegisterField("bPvpDesc", bPvp);
	row.RegisterField("log_id", log.log_id);
	row.RegisterField("name", f.itemName);
	row.RegisterField("item_type", GetItemTypeAsString(h.itemType));
	row.RegisterField("icon", h.icon);
	row.RegisterField("tier", h.tier);
	row.RegisterField("skill_id_req", h.skillReq1);
	row.RegisterField("skill_id_req2", h.skillReq2);
	row.RegisterField("skill_min", h.skillMin);
	row.RegisterField("weight", h.weight);
	row.RegisterField("description", f.description);

	//item flags
	bool bAttuneable = (h.flags & (Item::ITEM_FLAG_ATTUNEABLE | Item::ITEM_FLAG_ATTUNED)) != 0;
	row.RegisterField("attuneable", bAttuneable);
	bool bArtifact = (h.flags & Item::ITEM_FLAG_ARTIFACT) != 0;
	row.RegisterField("artifact", bArtifact);
	bool bLore = (h.flags & Item::ITEM_FLAG_LORE) != 0;
	row.RegisterField("lore", bLore);
	bool bTemp = (h.flags & Item::ITEM_FLAG_TEMPORARY) != 0;
	row.RegisterField("temporary", bTemp);
	bool bNoTrade = (h.flags & Item::ITEM_FLAG_NO_TRADE) != 0;
	row.RegisterField("notrade", bNoTrade);
	bool bNoValue = (h.flags & Item::ITEM_FLAG_NO_VALUE) != 0;
	row.RegisterField("novalue", bNoValue);
	bool bNoZone = (h.flags & Item::ITEM_FLAG_NO_ZONE) != 0;
	row.RegisterField("nozone", bNoZone);
	bool bNoDestroy = (h.flags & Item::ITEM_FLAG_NO_DESTROY) != 0;
	row.RegisterField("nodestroy", bNoDestroy);
	bool bCrafted = (h.flags & Item::ITEM_FLAG_CRAFTED) != 0;
	row.RegisterField("crafted", bCrafted);
	bool bGoodOnly = (h.flags & Item::ITEM_FLAG_GOOD_ONLY) != 0;
	row.RegisterField("good_only", bGoodOnly);
	bool bEvilOnly = (h.flags & Item::ITEM_FLAG_EVIL_ONLY) != 0;
	row.RegisterField("evil_only", bEvilOnly);
	bool bStackLore = (h.flags & Item::ITEM_FLAG_STACK_LORE) != 0;
	row.RegisterField("stacklore", bStackLore);
	bool bLoreEquip = (h.flags & Item::ITEM_FLAG_LORE_EQUIP) != 0;
	row.RegisterField("lore_equip", bLoreEquip);
	bool bNoTransmute = (h.flags & Item::ITEM_FLAG_NO_TRANSMUTE) != 0;
	row.RegisterField("no_transmute", bNoTransmute);
	bool bCursed = (h.flags & Item::ITEM_FLAG_CURSED) != 0;
	row.RegisterField("CURSED_flags_32768", bCursed);
	bool bOrnate = (h.flags & Item::ITEM_FLAG_ORNATE) != 0;
	row.RegisterField("ornate", bOrnate);
	bool bHeirloom = (h.flags & Item::ITEM_FLAG_HEIRLOOM) != 0;
	row.RegisterField("heirloom", bHeirloom);
	bool bAppearanceOnly = (h.flags & Item::ITEM_FLAG_APPEARANCE_ONLY) != 0;
	row.RegisterField("appearance_only", bAppearanceOnly);
	bool bUnlocked = (h.flags & Item::ITEM_FLAG_UNLOCKED) != 0;
	row.RegisterField("unlocked", bUnlocked);
	bool bReforged = (h.flags & Item::ITEM_FLAG_REFORGED) != 0;
	row.RegisterField("reforged", bReforged);
	bool bNoRepair = (h.flags & Item::ITEM_FLAG_NO_REPAIR) != 0;
	row.RegisterField("norepair", bNoRepair);
	bool bEthereal = (h.flags & Item::ITEM_FLAG_ETHEREAL) != 0;
	row.RegisterField("etheral", bEthereal);
	bool bRefined = (h.flags & Item::ITEM_FLAG_REFINED) != 0;
	row.RegisterField("refined", bRefined);
	bool bNoSalvage = (h.flags & Item::ITEM_FLAG_NO_SALVAGE) != 0;
	row.RegisterField("no_salvage", bNoSalvage);
	bool bIndestructible = (h.flags & Item::ITEM_FLAG_INDESTRUCTIBLE) != 0;
	row.RegisterField("indestructable", bIndestructible);
	bool bNoExperiment = (h.flags & Item::ITEM_FLAG_NO_EXPERIMENT) != 0;
	row.RegisterField("no_experiment", bNoExperiment);
	bool bHouseLore = (h.flags & Item::ITEM_FLAG_HOUSE_LORE) != 0;
	row.RegisterField("house_lore", bHouseLore);
	bool bFlagUnk28 = (h.flags & Item::ITEM_FLAG_UNK_28) != 0;
	row.RegisterField("flags2_4096", bFlagUnk28);
	bool bBuildingBlock = (h.flags & Item::ITEM_FLAG_BUILDING_BLOCK) != 0;
	row.RegisterField("building_block", bBuildingBlock);
	bool bFreeReforge = (h.flags & Item::ITEM_FLAG_FREE_REFORGE) != 0;
	row.RegisterField("free_reforge", bFreeReforge);
	bool bInfusable = (h.flags & Item::ITEM_FLAG_INFUSABLE) != 0;
	row.RegisterField("infusable", bInfusable);
	bool bMercOnly = (h.flags & Item::ITEM_FLAG_MERC_ONLY) != 0;
	row.RegisterField("merc_only", bMercOnly);
	bool bMountOnly = (h.flags & Item::ITEM_FLAG_MOUNT_ONLY) != 0;
	row.RegisterField("mount_only", bMountOnly);

	row.RegisterField("slots", h.slotBitmask);

	//Handle item sets
	if (!f.setName.empty()) row.RegisterField("set_id", ProcessItemSetData(item, bPvp));
	else row.RegisterField("set_id", SQLNull());

	//item effects
	{
		uint32_t effectIndex = 0;
		for (auto& e : f.effectsArray) {
			DatabaseRow r;
			r.m_tableName = "item_effects";

			r.RegisterField("item_id", item_id);
			r.RegisterField("effect", e.effectText);
			r.RegisterField("percentage", e.percentage);
			r.RegisterField("bullet", e.tabIndex);
			r.RegisterField("index", effectIndex);
			QueueRowInsert(r);
			effectIndex++;
		}
	}

	//Item stats 
	{
		uint32_t statIndex = 0;
		for (auto& stat : h.statModArray) {
			DatabaseRow r;
			r.m_tableName = "item_mod_stats";

			r.RegisterField("item_id", item_id);
			r.RegisterField("type", stat.statType);
			r.RegisterField("subtype", stat.statSubtype);
			if (stat.statType != 6) {
				r.RegisterField("iValue", stat.iValue);
				r.RegisterField("fValue", SQLNull());
			}
			else {
				r.RegisterField("iValue", SQLNull());
				r.RegisterField("fValue", stat.fValue);
			}
			r.RegisterField("sValue", stat.stringVal);
			r.RegisterField("level", stat.statLevel);
			r.RegisterField("stats_order", statIndex);
			QueueRowInsert(r);
			statIndex++;
		}
	}

	//Item mod strings 
	{
		uint32_t modIndex = 0;
		for (auto& mod : h.stringModArray) {
			DatabaseRow r;
			r.m_tableName = "item_mod_strings";

			r.RegisterField("item_id", item_id);
			r.RegisterField("mod", mod.stringVal);
			r.RegisterField("description", mod.statDescription);
			r.RegisterField("unk1", mod.unknown1);
			r.RegisterField("unk2", mod.unknown2);
			r.RegisterField("index", modIndex);
			QueueRowInsert(r);
			modIndex++;
		}
	}

	row.RegisterField("stack_count", f.stackSize);
	row.RegisterField("collectable_unk", f.collectable_unk);
	row.RegisterField("collectable", f.bCollectable);
	row.RegisterField("adornment_slot1", f.adornSlots[0]);
	row.RegisterField("adornment_slot2", f.adornSlots[1]);
	row.RegisterField("adornment_slot3", f.adornSlots[2]);
	row.RegisterField("adornment_slot4", f.adornSlots[3]);
	row.RegisterField("adornment_slot5", f.adornSlots[4]);
	row.RegisterField("adornment_slot6", f.adornSlots[5]);
	row.RegisterField("offers_quest_name", f.offersQuest);
	row.RegisterField("required_by_quest_name", f.requiredByQuest);
	row.RegisterField("display_charges", f.bHasCharges);
	row.RegisterField("max_charges", f.chargesMax);
	row.RegisterField("recommended_level", f.recommendedLevel);
	row.RegisterField("required_level", f.requiredLevel);
	row.RegisterField("soe_item_id", reinterpret_cast<int32_t&>(h.itemID));
	row.RegisterField("soe_item_crc", reinterpret_cast<int32_t&>(h.itemCRC));
	row.RegisterField("soe_item_id_unsigned", h.itemID);
	row.RegisterField("soe_item_crc_unsigned", h.itemCRC);
	row.RegisterField("sell_status_amount", f.sell_status_amount);
	row.RegisterField("transmuted_material", f.bTransmutedMaterial);
	row.RegisterField("harvest", f.bHarvestedMaterial);
	//Header and footer unknowns
	//Of the below unknowns only header_ftr_type_unk and header_unk19 are sent in our parse
	row.RegisterField("header_ftr_type_unk", h.footerTypeUnknown);
	row.RegisterField("header_unk19", h.unknown19);

	row.RegisterField("footer_unk_61", f.unknown61);
	row.RegisterField("footer_unk_7", f.footer_unknown_7);
	row.RegisterField("header_oversized1", h.unknownOversized1);
	row.RegisterField("header_oversized2", h.unknownOversized2);
	row.RegisterField("header_bytes5", std::string(reinterpret_cast<const char*>(h.unknownBytes5), 5));
	row.RegisterField("header_bytes10", std::string(reinterpret_cast<const char*>(h.unknownBytes10), 10));


	uint64_t adv_classes = 0;
	uint64_t ts_classes = 0;
	uint32_t adv_level = 0;
	uint32_t ts_level = 0;
	for (auto& itr : h.classReqArray) {
		if (itr.adventureClass != 255) {
			adv_level = itr.level / 10;
			adv_classes |= 1ull << itr.adventureClass;
		}
		else {
			ts_level = itr.level / 10;
			ts_classes |= 1ull << itr.tradeskillClass;
		}
	}
	row.RegisterField("adventure_default_level", adv_level);
	row.RegisterField("tradeskill_default_level", ts_level);
	row.RegisterField("tradeskill_classes", ts_classes);
	row.RegisterField("adventure_classes", adv_classes);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessTypeSpecificData(Substruct_ExamineDescItem* item) {
	if (auto a = dynamic_cast<Substruct_ExamineDescItem_Armor*>(item)) ProcessItemArmor(a);
	else if (auto w = dynamic_cast<Substruct_ExamineDescItem_MeleeWeapon*>(item)) ProcessItemMeleeWeapon(w);
	else if (auto w = dynamic_cast<Substruct_ExamineDescItem_RangedWeapon*>(item)) ProcessItemRangedWeapon(w);
	else if (auto s = dynamic_cast<Substruct_ExamineDescItem_Shield*>(item)) ProcessItemShield(s);
	else if (auto b = dynamic_cast<Substruct_ExamineDescItem_Bag*>(item)) ProcessItemBag(b);
	else if (auto s = dynamic_cast<Substruct_ExamineDescItem_SpellScroll*>(item)) ProcessItemSpellScroll(s);
	else if (auto r = dynamic_cast<Substruct_ExamineDescItem_RecipeBook*>(item)) ProcessItemRecipeBook(r);
	else if (auto p = dynamic_cast<Substruct_ExamineDescItem_Provision*>(item)) ProcessItemProvision(p);
	else if (auto b = dynamic_cast<Substruct_ExamineDescItem_Bauble*>(item)) ProcessItemBauble(b);
	else if (auto h = dynamic_cast<Substruct_ExamineDescItem_House*>(item)) ProcessItemHouse(h);
	else if (auto a = dynamic_cast<Substruct_ExamineDescItem_Ammo*>(item)) ProcessItemAmmo(a);
	else if (auto h = dynamic_cast<Substruct_ExamineDescItem_HouseContainer*>(item)) ProcessItemHouseContainer(h);
	else if (auto a = dynamic_cast<Substruct_ExamineDescItem_Adornment*>(item)) ProcessItemAdornment(a);
	else if (auto a = dynamic_cast<Substruct_ExamineDescItem_AchievementProfile*>(item)) ProcessItemAchievementProfile(a);
	else if (auto r = dynamic_cast<Substruct_ExamineDescItem_RewardVoucher*>(item)) ProcessItemRewardVoucher(r);
	else if (auto r = dynamic_cast<Substruct_ExamineDescItem_RewardCrate*>(item)) ProcessItemRewardCrate(r);
	else if (auto b = dynamic_cast<Substruct_ExamineDescItem_Book*>(item)) ProcessItemBook(b);
	else if (auto r = dynamic_cast<Substruct_ExamineDescItem_ReforgingDecoration*>(item)) ProcessItemReforgingDecoration(r);
}

void LogItemsParser::ProcessItemArmor(Substruct_ExamineDescItem_Armor* a) {
	DatabaseRow row;
	row.m_tableName = "item_details_armor";

	row.RegisterField("item_id", item_id);
	row.RegisterField("mitigation_low", a->mitigationLow);
	row.RegisterField("mitigation_high", a->mitigationHigh);
	row.RegisterField("absorb", a->absorb);
	row.RegisterField("unknown", a->skillID_NEEDS_VERIFY);
	row.RegisterField("item_score", a->itemScore);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemMeleeWeapon(Substruct_ExamineDescItem_MeleeWeapon* w) {
	DatabaseRow row;
	row.m_tableName = "item_details_weapon";

	row.RegisterField("item_id", item_id);
	row.RegisterField("wield_style", w->wieldType);
	row.RegisterField("damage_type", w->damageType);
	row.RegisterField("dmg_low", w->minDmg);
	row.RegisterField("dmg_high", w->maxDmg);
	row.RegisterField("dmg_mastery_low", w->masteryMinDmg);
	row.RegisterField("dmg_mastery_high", w->masteryMaxDmg);
	row.RegisterField("dmg_base_low", w->baseMinDamage);
	row.RegisterField("dmg_base_high", w->baseMaxDamage);
	row.RegisterField("delay", w->delay);
	row.RegisterField("damage_rating", w->damageRating);
	row.RegisterField("item_score", w->itemScore);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemRangedWeapon(Substruct_ExamineDescItem_RangedWeapon* w) {
	DatabaseRow row;
	row.m_tableName = "item_details_range";

	row.RegisterField("item_id", item_id);
	row.RegisterField("damage_type", w->damageType);
	row.RegisterField("dmg_low", w->minDmg);
	row.RegisterField("dmg_high", w->maxDmg);
	row.RegisterField("dmg_mastery_low", w->masteryMinDmg);
	row.RegisterField("dmg_mastery_high", w->masteryMaxDmg);
	row.RegisterField("dmg_base_low", w->baseMinDamage);
	row.RegisterField("dmg_base_high", w->baseMaxDamage);
	row.RegisterField("delay", w->delay);
	row.RegisterField("damage_rating", w->damageRating);
	row.RegisterField("item_score", w->itemScore);
	row.RegisterField("range_low", w->minRange);
	row.RegisterField("range_high", w->maxRange);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemShield(Substruct_ExamineDescItem_Shield* s) {
	DatabaseRow row;
	row.m_tableName = "item_details_shield";

	row.RegisterField("item_id", item_id);
	row.RegisterField("mitigation_low", s->mitigationLow);
	row.RegisterField("mitigation_high", s->mitigationHigh);
	row.RegisterField("item_score", s->itemScore);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemBag(Substruct_ExamineDescItem_Bag* b) {
	DatabaseRow row;
	row.m_tableName = "item_details_bag";

	row.RegisterField("item_id", item_id);
	row.RegisterField("num_slots", b->numSlots);
	row.RegisterField("weight_reduction", b->weightReduction);
	row.RegisterField("unknown12", b->bagUnknown1);
	row.RegisterField("backpack", b->bBackpack);
	row.RegisterField("unknown81", b->bagUnknown3);
	row.RegisterField("unknown69", b->bagUnknown4);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemSpellScroll(Substruct_ExamineDescItem_SpellScroll* s) {
	DatabaseRow row;
	row.m_tableName = "item_details_skill";

	row.RegisterField("item_id", item_id);
	row.RegisterField("spell_tier", s->spell.tier);
	row.RegisterField("soe_spell_crc", s->spell.id);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemRecipeBook(Substruct_ExamineDescItem_RecipeBook* r) {
	//Recipe Book Details
	{
		DatabaseRow row;
		row.m_tableName = "item_details_recipe";

		row.RegisterField("item_id", item_id);
		row.RegisterField("max_uses", r->numUses);
		QueueRowInsert(row);
	}

	//Recipe Items
	for (auto& itr : r->recipeArray) {
		DatabaseRow row;
		row.m_tableName = "item_details_recipe_items";

		row.RegisterField("item_id", item_id);
		row.RegisterField("name", itr.recipeName);
		row.RegisterField("icon", itr.icon);
		row.RegisterField("soe_recipe_crc", itr.recipeID);
		QueueRowInsert(row);
	}
}

void LogItemsParser::ProcessItemProvision(Substruct_ExamineDescItem_Provision* p) {
	DatabaseRow row;
	row.m_tableName = "item_details_food";

	row.RegisterField("item_id", item_id);
	row.RegisterField("type", p->provisionType);
	row.RegisterField("level", p->provisionLevel);
	row.RegisterField("duration", p->duration);
	row.RegisterField("satiation", p->header.tier);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemBauble(Substruct_ExamineDescItem_Bauble* p) {
	DatabaseRow row;
	row.m_tableName = "item_details_bauble";

	row.RegisterField("item_id", item_id);
	row.RegisterField("cast", p->castTime);
	row.RegisterField("recovery", p->recovery);
	row.RegisterField("duration", p->duration);
	row.RegisterField("recast", p->recast);
	row.RegisterField("display_slot_optional", p->bDisplaySlotOptional);
	row.RegisterField("display_cast_time", p->bDisplayCastTime);
	row.RegisterField("display_bauble_type", p->bDisplayBaubleType);
	row.RegisterField("effect_radius", p->effectRadius);
	row.RegisterField("max_aoe_targets", p->maxAoeTargets);
	row.RegisterField("display_until_cancelled", p->bDisplayUntilCancelled);
	row.RegisterField("item_score", p->itemScore);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessHouseData(Substruct_HouseItem* h) {
	DatabaseRow row;
	row.m_tableName = "item_details_house";

	row.RegisterField("item_id", item_id);
	row.RegisterField("status_rent_reduction", h->rentStatusReduction);
	row.RegisterField("coin_rent_reduction", h->rentCoinReduction);
	row.RegisterField("house_only", h->houseType);
	row.RegisterField("unk1", h->unknown1);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemHouse(Substruct_ExamineDescItem_House* h) {
	ProcessHouseData(&h->houseData);
}

void LogItemsParser::ProcessItemAmmo(Substruct_ExamineDescItem_Ammo* a) {
	DatabaseRow row;
	row.m_tableName = "item_details_thrown";

	row.RegisterField("item_id", item_id);
	row.RegisterField("range_bonus", a->range);
	row.RegisterField("damage_bonus", a->damageModifier);
	row.RegisterField("hit_bonus", a->hitBonus);
	row.RegisterField("damage_type", a->damageType);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemHouseContainer(Substruct_ExamineDescItem_HouseContainer* h) {
	DatabaseRow row;
	row.m_tableName = "item_details_house_container";

	row.RegisterField("item_id", item_id);
	row.RegisterField("num_slots", h->numSlots);
	row.RegisterField("allowed_types", h->allowedTypes);
	row.RegisterField("unknown12", h->unknown1);
	row.RegisterField("unknown13", h->unknown2);
	row.RegisterField("broker_commission", h->brokerCommission);
	row.RegisterField("fence_commission", h->fenceCommission);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemAdornment(Substruct_ExamineDescItem_Adornment* a) {
	DatabaseRow row;
	row.m_tableName = "item_details_adornments";

	row.RegisterField("item_id", item_id);
	row.RegisterField("duration", a->duration);
	row.RegisterField("item_types", a->itemTypes);
	row.RegisterField("slot_type", a->slotColor);
	row.RegisterField("description", a->description);
	row.RegisterField("description2", a->description2);
	row.RegisterField("unk1", a->unknown);
	row.RegisterField("unk2", a->unknown2);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemAchievementProfile(Substruct_ExamineDescItem_AchievementProfile* a) {
	DatabaseRow row;
	row.m_tableName = "item_details_achievement_profile";

	row.RegisterField("item_id", item_id);
	row.RegisterField("status_reduction", a->rentStatusReduction);
	row.RegisterField("coin_reduction", a->rentCoinReduction);
	row.RegisterField("house_type", a->houseType);
	row.RegisterField("unk_string", a->unknownString);
	row.RegisterField("unk1", a->unknown);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemRewardVoucher(Substruct_ExamineDescItem_RewardVoucher* r) {
	for (auto& itr : r->itemsArray) {
		DatabaseRow row;
		row.m_tableName = "item_details_reward_voucher";

		row.RegisterField("voucher_item_id", item_id);
		row.RegisterField("soe_item_id", itr.itemID);
		row.RegisterField("soe_item_crc", itr.crc);
		row.RegisterField("icon", itr.icon);
		row.RegisterField("name", itr.itemName);
		QueueRowInsert(row);
	}
}

void LogItemsParser::ProcessItemRewardCrate(Substruct_ExamineDescItem_RewardCrate* r) {
	{
		DatabaseRow row;
		row.m_tableName = "item_details_reward_crate";

		row.RegisterField("item_id", item_id);
		row.RegisterField("unk1", r->unknown1);
		row.RegisterField("unk2", r->unknown2);
		QueueRowInsert(row);
	}

	for (auto& itr : r->itemsArray) {
		DatabaseRow row;
		row.m_tableName = "item_details_reward_crate_item";

		row.RegisterField("crate_item_id", item_id);
		row.RegisterField("soe_item_id", itr.itemID);
		row.RegisterField("soe_item_crc", itr.crc);
		row.RegisterField("icon", itr.icon);
		row.RegisterField("stack_size", itr.stackSize);
		row.RegisterField("name_color", itr.colorID);
		row.RegisterField("name", itr.itemName);
		row.RegisterField("language_type", itr.language);
		QueueRowInsert(row);
	}
}

void LogItemsParser::ProcessItemBook(Substruct_ExamineDescItem_Book* r) {
	ProcessHouseData(&r->houseData);

	DatabaseRow row;
	row.m_tableName = "item_details_book";

	row.RegisterField("item_id", item_id);
	row.RegisterField("language", r->language);
	row.RegisterField("author", r->author);
	row.RegisterField("title", r->title);
	QueueRowInsert(row);
}

void LogItemsParser::ProcessItemReforgingDecoration(Substruct_ExamineDescItem_ReforgingDecoration* r) {
	DatabaseRow row;
	row.m_tableName = "item_details_decorations";

	row.RegisterField("item_id", item_id);
	row.RegisterField("decoration_name", r->decoName);
	QueueRowInsert(row);
}