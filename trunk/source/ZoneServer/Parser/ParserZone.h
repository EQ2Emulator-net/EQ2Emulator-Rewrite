#pragma once

#include "../../common/Parser/Parser.h"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <string>

class ParserZone : public Parser {
public:
	ParserZone(int arg, char** argv);

	void ProcessLogs();

	void ProcessItems(PacketLog& log);
	void ProcessAppearances(PacketLog& log);
	void ProcessMerchantPrices(PacketLog& log);
	void ProcessSpawns(PacketLog& log);
	void ProcessDialogs(PacketLog& log);
};