#include "stdafx.h"

#include "LogParser.h"

void LogParser::DoInsertsForTable(const char* table, int maxPerQuery, bool bInsertIgnore) {
	bool bFirst = true;
	int i = 0;

	auto itr = queued_inserts.find(table);

	if (itr == queued_inserts.end()) {
		return;
	}

	const std::string& fields = itr->second.first;
	const std::vector<std::string>& vals = itr->second.second;

	auto ResetQuery = [&bFirst, &i, table, bInsertIgnore](std::ostringstream& ss, const std::string& fields) {
		i = 0;
		bFirst = true;
		ss.str("");
		ss << "INSERT ";
		if (bInsertIgnore) ss << "IGNORE";
		ss << " INTO " << table << " " << fields << " VALUES ";
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
				database.Query(ss.str().c_str());
				ResetQuery(ss, fields);
			}
		}

		if (i) {
			database.Query(ss.str().c_str());
		}
	}

	queued_inserts.erase(itr);
};

void LogParser::QueueRowInsert(DatabaseRow& row) {
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