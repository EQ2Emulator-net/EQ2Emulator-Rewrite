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
		int32_t crc = itr->itemDesc->header.itemCRC;
		//If we haven't parsed this item yet, do so now
		if (parsed_soe_items.insert(crc).second) {
			ProcessItemDesc(log, itr->itemDesc, false);
			if (itr->pvpDesc) ProcessItemDesc(log, itr->pvpDesc, true);
		}
	}
}

void LogItemsParser::LoadExistingData() {
	parsed_soe_items = database.LoadExistingItems();
	parsed_item_sets = database.LoadExistingItemSets();
}


void LogItemsParser::ProcessItemDesc(PacketLog& log, Substruct_ExamineDescItem* item, bool bPvp) {
	DatabaseRow row;
	auto& h = item->header;
	auto& f = item->footer;

	row.m_tableName = "items";
	row.RegisterField("bPvpDesc", bPvp);
	row.RegisterField("log_id", log.log_id);
	row.RegisterField("name", f.itemName);
	//row.RegisterField("item_type")
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
	row.RegisterField("lore", bNoDestroy);
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
	row.RegisterField("ethereal", bEthereal);
	bool bRefined = (h.flags & Item::ITEM_FLAG_REFINED) != 0;
	row.RegisterField("refined", bRefined);
	bool bNoSalvage = (h.flags & Item::ITEM_FLAG_NO_SALVAGE) != 0;
	row.RegisterField("no_salvage", bNoSalvage);
	bool bIndestructible = (h.flags & Item::ITEM_FLAG_INDESTRUCTIBLE) != 0;
	row.RegisterField("indestructible", bIndestructible);
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
	if (parsed_item_sets.count(f.setName) == 0) {
		parsed_item_sets[f.setName] = database.CreateItemSet(f.setName);


	}
}