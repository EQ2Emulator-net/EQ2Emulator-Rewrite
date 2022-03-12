#pragma once

#include <vector>
#include <string>
#include <optional>
#include "PacketLog.h"

class Parser {
protected:
	Parser(int arg, char** argv);

	void InitDatabase();

	class ParserDatabase& database;

	std::optional<PacketLog> PopNextLog();
	void PrintLogVersions();

	std::vector<std::string> options;
	std::vector<std::string> log_names;
};