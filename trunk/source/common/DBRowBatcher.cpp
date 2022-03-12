#include "stdafx.h"

#include "DBRowBatcher.h"
#include <sstream>

void DBRowBatcher::QueueRowInsert(DatabaseRow& row) {
	auto& ins = queued_inserts[row.m_tableName];
	if (ins.first.empty()) {
		std::ostringstream ss;
		row.GenerateFieldList(ss);
		ins.first = ss.str();
	}

	std::ostringstream ss;
	row.GenerateValuesList(ss, true, false);
	ins.second.emplace_back(ss.str());
}

std::vector<std::string> DBRowBatcher::ProduceInsertsForTable(const char* table, int maxPerQuery, bool bInsertIgnore) {
	std::vector<std::string> ret;
	bool bFirst = true;
	int i = 0;

	auto itr = queued_inserts.find(table);

	if (itr == queued_inserts.end()) {
		return ret;
	}

	const std::string& fields = itr->second.first;
	const std::vector<std::string>& vals = itr->second.second;

	auto ResetQuery = [&bFirst, &i, table, bInsertIgnore](std::ostringstream& ss, const std::string& fields) {
		i = 0;
		bFirst = true;
		ss.str("");
		ss << "INSERT ";
		if (bInsertIgnore) ss << "IGNORE ";
		ss << "INTO " << table << " " << fields << " VALUES ";
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
				ss << ';';
				ret.emplace_back(ss.str());
				ResetQuery(ss, fields);
			}
		}

		if (i) {
			ss << ';';
			ret.emplace_back(ss.str());
		}
	}

	queued_inserts.erase(itr);

	return ret;
};