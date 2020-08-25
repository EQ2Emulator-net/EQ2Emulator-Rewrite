#include "stdafx.h"

#include "ParserDatabase.h"
#include "../common/log.h"

ParserDatabase::ParserDatabase() {

}

bool ParserDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

std::unordered_map<std::string, uint16_t> ParserDatabase::LoadOpcodesForVersion(uint32_t ver) {
	DatabaseResult res;
	
	assert(Select(&res, "SELECT `name`, `opcode` FROM `opcodes` WHERE `version_range1` <= %u AND `version_range2` >= %u;", ver, ver));

	std::unordered_map<std::string, uint16_t> ret;
	ret.reserve(res.GetNumRows());

	while (res.Next()) {
		ret[res.GetString(0)] = res.GetUInt16(1);
	}

	return ret;
}