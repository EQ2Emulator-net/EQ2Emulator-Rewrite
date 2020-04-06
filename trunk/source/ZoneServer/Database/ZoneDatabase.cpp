#include "stdafx.h"

#include "ZoneDatabase.h"
#include "../../common/log.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/ZoneOperator.h"

extern ZoneOperator z;

ZoneDatabase::ZoneDatabase() {

}

ZoneDatabase::~ZoneDatabase() {
}

bool ZoneDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

bool ZoneDatabase::LoadOpcodes() {
	DatabaseResult result;
	bool success;
	uint32_t count = 0;

	success = Select(&result, "SELECT `name`, `version_range1`, `version_range2`, `opcode` FROM `opcodes`");
	if (!success)
		return false;

	while (result.Next()) {
		OpcodeManager::GetGlobal()->RegisterVersionOpcode(result.GetString(0), result.GetUInt32(1), result.GetUInt32(2), result.GetUInt16(3));
		count++;
	}

	LogDebug(LOG_DATABASE, 0, "--Loaded %u opcodes", count);
	return true;
}

bool ZoneDatabase::LoadZoneInfo(ZoneServer* z) {
	DatabaseResult result;
	
	bool success = Select(&result, "SELECT * FROM zones WHERE id = %u", z->GetID());
	if (!success)
		return success;

	if (result.Next()) {
		z->SetExpansionID(result.GetUInt8(1));
		z->SetName(result.GetString(2));
		z->SetFile(result.GetString(3));
		z->SetDescription(result.GetString(4));
		z->SetSafeX(result.GetFloat(5));
		z->SetSafeY(result.GetFloat(6));
		z->SetSafeZ(result.GetFloat(7));
		z->SetSafeHeading(result.GetFloat(8));
		z->SetUnderworld(result.GetFloat(9));
		z->SetEXPModifier(result.GetFloat(10));
		z->SetMinRecommended(result.GetUInt8(11));
		z->SetMaxRecomended(result.GetUInt8(12));
		z->SetZoneType(result.GetString(13));
		z->SetIsAlwaysLoaded(result.GetBool(14));
		z->SetCityZone(result.GetBool(15));
		z->SetWeatherAllowed(result.GetBool(16));
		z->SetMinStatus(result.GetUInt32(17));
		z->SetMinLevel(result.GetUInt32(18));
		z->SetMaxLevel(result.GetUInt32(19));
		z->SetStartingZone(result.GetUInt8(20));
		z->SetInstanceType(result.GetString(21));
		z->SetDefaultReenterTime(result.GetUInt32(22));
		z->SetDefaultResetTime(result.GetUInt32(23));
		z->SetDefaultLockoutTime(result.GetUInt32(124));
		z->SetForceGroupToZone(result.GetUInt16(25));
		z->SetLuaScript(result.GetString(26));
		z->SetShutdownTimer(result.GetUInt32(27));
		z->SetZoneMOTD(result.GetString(28));
		z->SetRulesetID(result.GetUInt32(29));
	}
	else
		return false;

	return true;
}

bool ZoneDatabase::LoadCommands() {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT id, command, required_status FROM commands WHERE length(subcommand) = 0 ORDER BY id ASC");
	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		z.AddCommand(result.GetUInt32(0), result.GetString(1));
		count++;
	}

	LogDebug(LOG_COMMAND, 0, "--Loaded %u commands", count);

	ret = LoadSubcommands();
	return ret;
}

bool ZoneDatabase::LoadSubcommands() {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT id, command, subcommand, required_status FROM commands WHERE length(subcommand) > 0 ORDER BY id ASC");
	if (!ret)
		return ret;

	uint32_t count = 0;
	while (result.Next()) {
		z.AddSubCommand(result.GetString(1), result.GetUInt32(0), result.GetString(2));
		count++;
	}

	LogDebug(LOG_COMMAND, 0, "---Loaded %u subcommands", count);
	return ret;
}