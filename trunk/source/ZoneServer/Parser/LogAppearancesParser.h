#pragma once

#include "../../common/Parser/LogParser.h"

class LogAppearancesParser : public LogParser {
public:
	LogAppearancesParser(PacketLog& log, class ParserDatabase& db);
	~LogAppearancesParser() = default;


};