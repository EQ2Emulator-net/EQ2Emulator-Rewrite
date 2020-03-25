#pragma once

#include "../../common/database.h"

class ZoneServer;

class ZoneDatabase : public Database {
public:
	ZoneDatabase();
	~ZoneDatabase();

	bool Start();
	void Stop();

	bool LoadOpcodes();
	bool LoadZoneInfo(ZoneServer* z);
	bool LoadCommands();
	bool LoadSubcommands();

private:

};