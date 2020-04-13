#include "database.h"

//This class has functions that are used on both the worldserver and zoneserver
class CommonDatabase : public Database {
protected:
	CommonDatabase() = default;
	~CommonDatabase() = default;

public:
	bool LoadOpcodes();
	void LoadRules(class RuleManager& rule_manager);
};