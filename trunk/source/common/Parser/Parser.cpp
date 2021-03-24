#include "stdafx.h"

#include "Parser.h"
#include "ParserDatabase.h"
#include "../ConfigReader.h"
#include "../log.h"
#include "../util.h"

Parser::Parser(int argc, char** argv) : database(*ParserDatabase::GetGlobal()) {
	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-PacketParser");
	ConfigReader cr(nullptr, &database, nullptr);
	cr.ReadConfig("parser-config.xml");
	InitDatabase();

	//Find args and log filenames
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (StringEndsWith(arg, ".log")) {
			log_names.emplace_back(std::move(arg));
		}
		else if (arg.front() == '-' && arg[1] != '-') {
			options.emplace_back(std::move(arg));
		}
	}
}

void Parser::InitDatabase() {
	database.Start();
}

std::optional<PacketLog> Parser::PopNextLog() {
	std::optional<PacketLog> ret;

	while (!log_names.empty()) {
		ret.emplace(log_names.back());
		log_names.pop_back();

		if (ret->TransformPackets()) break;
		ret.reset();
	}

	return ret;
}