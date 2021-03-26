#include "stdafx.h"

#include "ParserZone.h"
#include "../../common/log.h"
#include "../../common/Parser/PacketLog.h"
#include "../Packets/ItemPackets.h"
#include "../../common/Parser/ParserDatabase.h"
#include "../../common/DatabaseRow.h"

#include <set>

ParserZone::ParserZone(int argc, char** argv) : Parser(argc, argv) {
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	ProcessLogs();

	logging = false;
	logging_thread.join();
}

void ParserZone::ProcessLogs() {
	while (std::optional<PacketLog> log = PopNextLog()) {
		log->log_id = database.CreateLogEntry(log->filename, log->logVersion);
		for (const std::string& opt : options) {
			if (opt == "-items") {
				ProcessItems(*log);
			}
		}
	}
}

void ParserZone::ProcessItems(PacketLog& log) {
	LogItemsParser processor(log, database);
}

LogItemsParser::LogItemsParser(PacketLog& log, ParserDatabase& db) : database(db) {
	auto packets = log.FindPackets<OP_EqExamineInfoCmd_Packet, ExamineInfoCmd_Item_Packet>();

	if (!packets.empty()) {
		LoadExistingData();
	}

	LogDebug(LOG_PARSER, 0, "Item count for log %s : %llu", log.filename.c_str(), packets.size());
	parsed_soe_items.reserve(parsed_soe_items.size() + packets.size());
	for (auto& itr : packets) {
		int32_t id = itr->itemDesc->header.itemID;
		//If we haven't parsed this item yet, do so now
		if (parsed_soe_items.insert(id).second) {
			ProcessItemDesc(log, itr->itemDesc, false);
			if (itr->pvpDesc) ProcessItemDesc(log, itr->pvpDesc, true);
		}
	}
	ProcessQueuedInserts();
	LogDebug(LOG_PARSER, 0, "Done with log.");
}

void LogItemsParser::ProcessQueuedInserts() {
	auto DoInsertsForTable = [this](const char* table, std::string& fields, std::vector<std::string>& vals, int maxPerQuery) {
		bool bFirst = true;
		int i = 0;

		auto ResetQuery = [&bFirst, &i, table](std::ostringstream& ss, const std::string& fields) {
			i = 0;
			bFirst = true;
			ss.str("");
			ss << "INSERT INTO " << table << " " << fields << " VALUES ";
		};

		if (!vals.empty()) {
			std::ostringstream ss;
			ResetQuery(ss, fields);
			for (auto& itr : vals) {
				if (bFirst) bFirst = false;
				else ss << ",\n";

				ss << itr;

				if (++i == maxPerQuery) {
					//Start a new query;
					database.Query(ss.str().c_str());
					ResetQuery(ss, fields);
				}
			}

			if (i) {
				database.Query(ss.str().c_str());
			}
		}
	};

	DoInsertsForTable("items", item_fields, item_inserts, 250);
	DoInsertsForTable("item_effects", item_effect_fields, item_effects, 250);
	DoInsertsForTable("item_mod_stats", item_mod_stat_fields, item_mod_stat, 250);
	DoInsertsForTable("item_mod_strings", item_mod_string_fields, item_mod_strings, 250);
	DoInsertsForTable("item_itemset_items", item_itemset_items_fields, item_itemset_items, 250);
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

void LogItemsParser::ProcessItemDesc(PacketLog& log, Substruct_ExamineDescItem* item, bool bPvp) {
	item_id = next_id++;

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

	uint32_t set_id = 0;

	//Handle item sets
	if (!f.setName.empty()) {
		auto itr = parsed_item_sets.find(f.setName);
		if (itr != parsed_item_sets.end()) set_id = itr->second;
	}

	if (set_id == 0 && !f.setName.empty()) {
		set_id = database.CreateItemSet(f.setName);
		parsed_item_sets[f.setName] = set_id;
		//Set bonuses
		{
			uint32_t setBonusIndex = 0;
			for (auto& itr : f.itemSetDetails.setBonusArray) {
				uint32_t setBonusId = database.CreateItemSetBonus(set_id, setBonusIndex, itr.itemsNeeded);

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
						r.RegisterField("fValue", stat.fValue);
						r.RegisterField("iValue", SQLNull());
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

				r.RegisterField("set_id", set_id);
				r.RegisterField("item_name", itr.itemName);
				r.RegisterField("unk1", itr.unknown1);
				r.RegisterField("unk2", itr.unknown2);
				r.RegisterField("index", setItemIndex);
				if (item_itemset_items_fields.empty()) {
					std::ostringstream ss;
					r.GenerateFieldList(ss);
					item_itemset_items_fields = ss.str();
				}
				
				std::ostringstream ss;
				r.GenerateValuesList(ss, true, false);
				item_itemset_items.emplace_back(ss.str());
				setItemIndex++;
			}
		}
	}
	if (set_id) {
		row.RegisterField("set_id", set_id);
	}
	else {
		row.RegisterField("set_id", SQLNull());
	}
	//Finished with item sets (make a sub function for that)

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

			std::ostringstream ss;
			if (item_effect_fields.empty()) {
				r.GenerateFieldList(ss);
				item_effect_fields = ss.str();
				ss.str("");
			}
			r.GenerateValuesList(ss, true, false);
			item_effects.emplace_back(ss.str());
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
				r.RegisterField("fValue", stat.fValue);
				r.RegisterField("iValue", SQLNull());
			}
			r.RegisterField("sValue", stat.stringVal);
			r.RegisterField("level", stat.statLevel);
			r.RegisterField("stats_order", statIndex);

			std::ostringstream ss;
			if (item_mod_stat_fields.empty()) {
				r.GenerateFieldList(ss);
				item_mod_stat_fields = ss.str();
				ss.str("");
			}
			r.GenerateValuesList(ss, true, false);
			item_mod_stat.emplace_back(ss.str());
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

			std::ostringstream ss;
			if (item_mod_string_fields.empty()) {
				r.GenerateFieldList(ss);
				item_mod_string_fields = ss.str();
				ss.str("");
			}
			r.GenerateValuesList(ss, true, false);
			item_mod_strings.emplace_back(ss.str());
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
	row.RegisterField("soe_item_id", reinterpret_cast<int32_t&>(h.itemCRC));
	row.RegisterField("soe_item_crc", reinterpret_cast<int32_t&>(h.itemID));
	row.RegisterField("soe_item_id_unsigned", h.itemCRC);
	row.RegisterField("soe_item_crc_unsigned", h.itemID);
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

	std::ostringstream values;
	row.GenerateValuesList(values, true, false);
	item_inserts.emplace_back(values.str());

	if (item_fields.empty()) {
		std::ostringstream ss;
		row.GenerateFieldList(ss);
		item_fields = ss.str();
	}
}