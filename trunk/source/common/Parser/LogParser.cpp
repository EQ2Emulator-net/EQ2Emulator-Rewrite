#include "stdafx.h"

#include "LogParser.h"

void LogParser::DoInsertsForTable(const char* table, int maxPerQuery, bool bInsertIgnore) {
	auto queries = batcher.ProduceInsertsForTable(table, maxPerQuery, bInsertIgnore);

	for (auto& itr : queries) {
		database.QuerySimple(itr.c_str());
	}
};

void LogParser::QueueRowInsert(DatabaseRow& row) {
	batcher.QueueRowInsert(row);
}