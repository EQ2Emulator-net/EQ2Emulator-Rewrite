#include "stdafx.h"

#include "ParserDatabase.h"
#include "../log.h"
#include "../EmuAssert.h"
#include "../../ZoneServer/Parser/ParserZone.h"
#include "../../ZoneServer/Parser/LogItemsParser.h"

ParserDatabase::ParserDatabase() {

}

bool ParserDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

std::unordered_map<std::string, uint16_t> ParserDatabase::LoadOpcodesForVersion(uint32_t ver) {
	DatabaseResult res;
	
	Select(&res, "SELECT `name`, `opcode` FROM `opcodes` WHERE `version_range1` <= %u AND `version_range2` >= %u;", ver, ver);

	std::unordered_map<std::string, uint16_t> ret;
	ret.reserve(res.GetNumRows());
	
	if (res.GetNumRows() == 0) {
		LogError(LOG_PARSER, 0, "Could not find opcodes for version %u! Update?", ver);
	}

	while (res.Next()) {
		ret[res.GetString(0)] = res.GetUInt16(1);
	}

	return ret;
}

std::unordered_set<uint32_t> ParserDatabase::LoadExistingItems() {
	DatabaseResult res;

	Select(&res, "SELECT DISTINCT `soe_item_id_unsigned` FROM `items`;");

	std::unordered_set<uint32_t> ret;
	ret.reserve(res.GetNumRows());

	while (res.Next()) {
		ret.insert(res.GetUInt32(0));
	}

	return ret;
}

uint32_t ParserDatabase::CreateLogEntry(std::string name, uint32_t ver) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO parsed_logs (`name`, `dataVersion`) VALUES ('%s',%u);", Escape(name).c_str(), ver);

	return static_cast<uint32_t>(res.last_insert_id);
}

uint32_t ParserDatabase::CreateItemSet(std::string name, bool bPvp, int16_t level) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO item_itemsets (`set_name`,`bPvpDesc`,`itemLevel`) VALUES ('%s',%u,%i);", Escape(name).c_str(), bPvp ? 1 : 0, level);

	return static_cast<uint32_t>(res.last_insert_id);
}

uint32_t ParserDatabase::CreateItemSetBonus(uint32_t set_id, uint32_t index, uint32_t items_needed) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO item_itemset_bonus (`set_id`, `index`, `num_items_needed`) VALUES (%u,%u,%u);", set_id, index, items_needed);

	return static_cast<uint32_t>(res.last_insert_id);
}

std::vector<ItemSetKey> ParserDatabase::LoadExistingItemSets() {
	DatabaseResult res;

	Select(&res, "SELECT id, set_name, bPvpDesc, itemLevel FROM `item_itemsets`;");

	std::vector<ItemSetKey> ret;
	ret.reserve(res.GetNumRows());

	while (res.Next()) {
		ItemSetKey key;
		key.id = res.GetUInt32(0);
		key.set_name = res.GetString(1);
		key.bPvpDesc = res.GetBool(2);
		key.item_level = res.GetInt16(3);
		ret.emplace_back(std::move(key));
	}

	return ret;
}

uint32_t ParserDatabase::LoadNextItemID() {
	DatabaseResult res;

	//Starting raw item ids at 20mil
	Select(&res, "SELECT IF(COUNT(id) = 0, 20000000, MAX(id)+1) as id FROM `items`;");

	EmuAssert(res.Next());

	return res.GetUInt32(0);
}

uint32_t ParserDatabase::LoadNextSpawnID() {
	DatabaseResult res;

	Select(&res, "SELECT IF(COUNT(id) = 0, 1, MAX(id)+1) as id FROM `spawn`;");

	EmuAssert(res.Next());

	return res.GetUInt32(0);
}