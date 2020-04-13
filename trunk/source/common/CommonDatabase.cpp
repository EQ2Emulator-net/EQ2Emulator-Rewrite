#include "stdafx.h"

#include "CommonDatabase.h"
#include "log.h"
#include "Packets/EQ2Packets/OpcodeManager.h"

bool CommonDatabase::LoadOpcodes() {
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

	LogDebug(LOG_DATABASE, 0, "Loaded %u opcodes", count);
	return true;
}