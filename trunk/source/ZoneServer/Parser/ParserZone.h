#pragma once

#include "../../common/Parser/Parser.h"
#include <unordered_set>

class ParserZone : public Parser {
public:
	ParserZone(int arg, char** argv);

	void ProcessLogs();

	void ProcessItems(PacketLog& log);
};

class LogItemsParser {
public:
	LogItemsParser(PacketLog& log, class ParserDatabase& db);

private:
	void ProcessQueuedInserts();
	std::string GetItemTypeAsString(uint8_t type);

	void LoadExistingData();
	void ProcessItemDesc(PacketLog& log, class Substruct_ExamineDescItem* item, bool bPvp);
	class ParserDatabase& database;
	std::unordered_set<uint32_t> parsed_soe_items;
	std::unordered_map<std::string, uint32_t> parsed_item_sets;
	uint32_t next_id;
	uint32_t item_id;

	std::string item_fields;
	std::vector<std::string> item_inserts;

	std::string item_effect_fields;
	std::vector<std::string> item_effects;

	std::string item_mod_stat_fields;
	std::vector<std::string> item_mod_stat;

	std::string item_mod_string_fields;
	std::vector<std::string> item_mod_strings;

	std::string item_itemset_items_fields;
	std::vector<std::string> item_itemset_items;
};