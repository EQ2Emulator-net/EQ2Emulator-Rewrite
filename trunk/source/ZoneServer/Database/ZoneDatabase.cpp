#include "stdafx.h"

#include "ZoneDatabase.h"
#include "../../common/log.h"

static void DatabaseQueryError(Database *db) {
	LogError(LOG_DATABASE, 0, "Error running MySQL query: %s", db->GetError());
}

ZoneDatabase::ZoneDatabase() {

}

ZoneDatabase::~ZoneDatabase() {
	Disconnect();
}

bool ZoneDatabase::Start() {
	DatabaseCallbacks callbacks;
	callbacks.query_error = DatabaseQueryError;
	SetCallbacks(&callbacks);

	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	LogError(LOG_DATABASE, 0, "Error connecting to MySQL: %s", GetError());
	return false;
}

void ZoneDatabase::Stop() {
	Disconnect();
}