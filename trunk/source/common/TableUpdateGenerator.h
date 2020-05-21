#pragma once

#include "DBFieldTracker.h"
#include <vector>

class TableUpdateGenerator {
public:
	TableUpdateGenerator() = default;

	TableUpdateGenerator(const TableUpdateGenerator& rhs) {
		//We don't want to actually copy the raw pointers etc over
	}

	~TableUpdateGenerator() {
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

	bool CheckForUpdates(std::ostringstream& out) {
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
};