#pragma once
#include "../CommonDatabase.h"

class ParserDatabase : public CommonDatabase {
private:
	ParserDatabase();
public:
	~ParserDatabase() = default;

	bool Start();
	std::unordered_map<std::string, uint16_t> LoadOpcodesForVersion(uint32_t ver);

	static ParserDatabase* GetGlobal() { static ParserDatabase g_database; return &g_database; }
};