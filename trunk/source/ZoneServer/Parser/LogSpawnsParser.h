#pragma once

#include "../../common/Parser/LogParser.h"

class LogSpawnsParser : public LogParser {
public:
	LogSpawnsParser(PacketLog& log, class ParserDatabase& db);
	~LogSpawnsParser() = default;

private:
	void ParseNPCAppearance(class OP_CreateGhostCmd_Packet& p, uint32_t spawn_id);
};