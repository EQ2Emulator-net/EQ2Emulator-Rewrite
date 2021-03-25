#include "stdafx.h"

#include "ParserDatabase.h"
#include "../log.h"
#include "../EmuAssert.h"

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
	EmuAssert(res.GetNumRows());

	while (res.Next()) {
		ret[res.GetString(0)] = res.GetUInt16(1);
	}

	return ret;
}

std::unordered_set<int32_t> ParserDatabase::LoadExistingItems() {
	DatabaseResult res;

	Select(&res, "SELECT DISTINCT `soe_item_crc` FROM `items`;");

	std::unordered_set<int32_t> ret;
	ret.reserve(res.GetNumRows());

	while (res.Next()) {
		ret.insert(res.GetInt32(0));
	}

	return ret;
}

uint32_t ParserDatabase::CreateLogEntry(std::string name, uint32_t ver) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO parsed_logs (name, version) VALUES ('%s',%u);", Escape(name).c_str(), ver);

	return static_cast<uint32_t>(res.last_insert_id);
}

uint32_t ParserDatabase::CreateItemSet(std::string name) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO item_itemsets (set_name) VALUES ('%s');", Escape(name).c_str());

	return static_cast<uint32_t>(res.last_insert_id);
}

uint32_t ParserDatabase::CreateItemSetBonus(uint32_t set_id, uint32_t index, uint32_t items_needed) {
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID,
		"INSERT INTO item_itemset_bonus (set_id, index) VALUES (%u,%u);", set_id, index);

	return static_cast<uint32_t>(res.last_insert_id);
}

std::unordered_map<std::string, uint32_t> ParserDatabase::LoadExistingItemSets() {
	DatabaseResult res;

	Select(&res, "SELECT id, set_name FROM `item_itemsets`;");

	std::unordered_map<std::string, uint32_t> ret;
	ret.reserve(res.GetNumRows());

	while (res.Next()) {
		ret[res.GetString(1)] = res.GetUInt32(0);
	}

	return ret;
}