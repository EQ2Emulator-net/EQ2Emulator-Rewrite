#pragma once

#include "ParserDatabase.h"
#include "../DBRowBatcher.h"
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
	DBRowBatcher batcher;

protected:
	class ParserDatabase& database;
	PacketLog& log;
};