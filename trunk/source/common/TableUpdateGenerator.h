#pragma once

#include "DatabaseRow.h"
#include <vector>

//std::vector<std::pair<KeyFieldName, KeyValue>>
typedef std::vector<std::pair<std::string, std::string>> KeyList_t;

class TableUpdateGenerator {
public:
	TableUpdateGenerator() = default;
	~TableUpdateGenerator() = default;
	TableUpdateGenerator(const TableUpdateGenerator& rhs) = delete;

	bool GenerateUpdate(std::ostream& o) {
		bool ret = GenerateExistingRowUpdates(o);
		ret |= GenerateDeleteFromStatements(o);
		ret |= GenerateInsertStatements(o);
		return ret;
	}
	
	void AddNewRow(DatabaseRow* row) {
		rowsToAdd.emplace_back(row);
	}

	void AddExistingRow(DatabaseRow* row) {
		existingRows.emplace_back(row);
	}

	//Find an existing row where ALL keys match and toggle it for removal on the next sync
	bool RemoveRow(const KeyList_t& keys) {
		bool bFound = false;

		auto DoesRowMatch = [keys](DatabaseRow* row) -> bool {
			KeyList_t checkCopy = keys;

			for (auto itr = row->fields.begin(); itr != row->fields.end(); itr++) {
				DBFieldTrackerBase* ftb = *itr;

				for (auto itr = checkCopy.begin(); itr != checkCopy.end(); itr++) {
					auto& kv = *itr;
					if (ftb->field == kv.first && ftb->ValueToString() == kv.second) {
						checkCopy.erase(itr);
						break;
					}
				}

				if (checkCopy.empty()) {
					return true;
				}
			}

			return false;
		};

		//Check existing rows
		for (auto itr = existingRows.begin(); itr != existingRows.end();) {
			std::unique_ptr<DatabaseRow>& row = *itr;
			if (DoesRowMatch(row.get())) {
				rowsToRemove.emplace_back(std::move(row));
				itr = existingRows.erase(itr);
			}
			else {
				++itr;
			}
		}

		//Check if any pending rows match, do not send them to the database if so
		for (auto itr = rowsToAdd.begin(); itr != rowsToAdd.end();) {
			const std::unique_ptr<DatabaseRow>& row = *itr;
			if (DoesRowMatch(row.get())) {
				itr = rowsToAdd.erase(itr);
			}
			else {
				++itr;
			}
		}
	}

private:
	std::vector<std::unique_ptr<DatabaseRow>> existingRows;
	std::vector<std::unique_ptr<DatabaseRow>> rowsToAdd;
	std::vector<std::unique_ptr<DatabaseRow>> rowsToRemove;

	bool GenerateExistingRowUpdates(std::ostream& o) {
		bool ret = false;
		for (auto& row : existingRows) {
			ret |= row->CheckForUpdates(o);
		}
		return ret;
	}

	bool GenerateInsertStatements(std::ostream& o) {
		bool ret = !rowsToAdd.empty();
		for (auto& row : rowsToAdd) {
			row->GenerateSingleInsert(o);
			existingRows.emplace_back(std::move(row));
		}
		rowsToAdd.clear();
		return ret;
	}

	bool GenerateDeleteFromStatements(std::ostream& o) {
		bool ret = !rowsToAdd.empty();
		for (auto& row : rowsToRemove) {
			o << "DELETE FROM " << row->m_tableName << " WHERE " << row->m_criteria << ';';
		}
		rowsToRemove.clear();
		return ret;
	}
};