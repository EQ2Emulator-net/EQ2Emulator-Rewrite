#pragma once

#include "../../common/Parser/Parser.h"

class ParserZone : public Parser {
public:
	ParserZone(int arg, char** argv);

	void ProcessLogs();

	void ProcessItems(PacketLog& log);
};

class LogItemsParser {
public:
	LogItemsParser(PacketLog& log, class ParserDatabase& db);
};