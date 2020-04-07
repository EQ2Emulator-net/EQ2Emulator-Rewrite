#pragma once

#include "../../common/database.h"

class ZoneServer;
class Entity;

class ZoneDatabase : public Database {
public:
	ZoneDatabase();
	~ZoneDatabase();

	bool Start();

	bool LoadOpcodes();
	bool LoadZoneInfo(ZoneServer* z);
	bool LoadCommands();
	bool LoadSubcommands();
	bool LoadCharacter(uint32_t char_id, uint32_t account_id, std::shared_ptr<Entity> entity);

private:

};