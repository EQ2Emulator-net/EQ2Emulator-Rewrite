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
	void LoadExistingData();
	void ProcessItemDesc(PacketLog& log, class Substruct_ExamineDescItem* item, bool bPvp);
	class ParserDatabase& database;
	std::unordered_set<int32_t> parsed_soe_items;
	std::unordered_map<std::string, uint32_t> parsed_item_sets;
};