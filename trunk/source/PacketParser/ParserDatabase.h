#include "../common/CommonDatabase.h"

class ParserDatabase : public CommonDatabase {
public:
	ParserDatabase();
	~ParserDatabase() = default;

	bool Start();
	std::unordered_map<std::string, uint16_t> LoadOpcodesForVersion(uint32_t ver);
};