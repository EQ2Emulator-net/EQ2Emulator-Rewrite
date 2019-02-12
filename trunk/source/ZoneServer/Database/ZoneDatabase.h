#pragma once

#include "../../common/database.h"

class ZoneDatabase : public Database {
public:
	ZoneDatabase();
	~ZoneDatabase();

	bool Start();
	void Stop();

private:

};