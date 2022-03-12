#pragma once

#include "DatabaseRow.h"
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

class DBRowBatcher {
public:
	DBRowBatcher() = default;
	~DBRowBatcher() = default;

	void QueueRowInsert(DatabaseRow& row);
	std::vector<std::string> ProduceInsertsForTable(const char* table, int maxPerQuery, bool bInsertIgnore);

private:
	//map<tableName, pair<fields, vector<values> > >
	std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> queued_inserts;
};