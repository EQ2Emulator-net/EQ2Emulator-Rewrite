#pragma once

#include "DBFieldTracker.h"
#include <sstream>
#include <vector>

class DatabaseRow {
public:
	DatabaseRow() = default;

	DatabaseRow(const DatabaseRow& rhs) {
		//We don't want to actually copy the raw pointers etc over
	}

	~DatabaseRow() {
		for (auto& itr : fields) {
			delete itr;
		}
	}

	std::string m_tableName;
	std::string m_criteria;
	std::vector<DBFieldTrackerBase*> fields;

	template<typename T, typename Manip = InvalidManip<T>>
	void RegisterField(const char* field, const T& val) {
		fields.push_back(new DBFieldTracker<T, Manip>(field, val));
	}

	bool CheckForUpdates(std::ostream& out) {
		std::ostringstream tmp;
		tmp << "UPDATE `" << m_tableName << "` SET ";

		bool ret = false;
		for (auto& field : fields) {
			ret |= field->CheckAndAdd(tmp, !ret);
		}

		if (ret) {
			//We have updates!
			tmp << '\n' << m_criteria << ";\n";
			out << tmp.str();
		}

		return ret;
	}

	std::ostream& GenerateFieldList(std::ostream& out) {
		bool bFirst = true;
		for (auto& itr : fields) {
			if (bFirst) {
				out << "(`";
				bFirst = false;
			}
			else {
				out << ",`";
			}
			out << itr->field << '`';
		}

		out << ')';
		return out;
	}

	std::ostream& GenerateValuesList(std::ostream& out, bool bUseOrigValue = false, bool bIncludeField = true) {
		bool bFirst = true;
		for (auto& itr : fields) {
			if (bFirst) {
				out << "(";
			}
			else {
				out << ",";
			}
			itr->AddWithoutCheck(out, bUseOrigValue, bIncludeField);
			bFirst = false;
		}

		out << ')';
		return out;
	}

	void GenerateSingleInsert(std::ostream& out, bool bUseOrigValue = false) {
		if (fields.empty()) {
			return;
		}

		out << "INSERT INTO " << m_tableName << ' ';
		GenerateFieldList(out) << " VALUES ";
		GenerateValuesList(out, bUseOrigValue, false) << ';';
	}
};