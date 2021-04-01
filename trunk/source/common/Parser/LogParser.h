#pragma once

#include "ParserDatabase.h"
#include "../DatabaseRow.h"
#include "PacketLog.h"
#include <unordered_map>
#include <string>

class LogParser {
public:
	LogParser(PacketLog& l, class ParserDatabase& db) : log(l), database(db) {}
	virtual ~LogParser() = default;

	void QueueRowInsert(class DatabaseRow& row);
	void DoInsertsForTable(const char* table, int maxPerQuery, bool bInsertIgnore = false);

private:
	//map<tableName, pair<fields, vector<values> > >
	std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> queued_inserts;

protected:
	class ParserDatabase& database;
	PacketLog& log;
};