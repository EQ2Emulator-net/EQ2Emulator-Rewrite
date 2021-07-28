#include "stdafx.h"

#include "Parser.h"
#include "ParserDatabase.h"
#include "../ConfigReader.h"
#include "../log.h"
#include "../util.h"
#include <regex>
#include <filesystem>

extern CommonDatabase* dbFieldTrackerDB;

Parser::Parser(int argc, char** argv) : database(*ParserDatabase::GetGlobal()) {
	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-PacketParser");
	ConfigReader cr(nullptr, &database, nullptr);
	cr.ReadConfig("parser-config.xml");
	InitDatabase();
	dbFieldTrackerDB = &database;

	const std::regex regex_check("^LOGFILE_REGEX\\((.*)\\)$");

	//Find args and log filenames
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		std::smatch m;
		if (std::regex_match(arg, m, regex_check)) {
			std::string r_text(m[1].first, m[1].second);
			const std::regex file_regex(r_text);

			using rdi = std::filesystem::recursive_directory_iterator;

			for (auto& itr : rdi("./")) {
				std::string fpath = itr.path().string();
				//Strip the ./ from the path
				fpath = fpath.substr(2);
				//Change \ characters to / for consistency between OS's
				std::replace(fpath.begin(), fpath.end(), '\\', '/');
				if (std::regex_search(fpath, m, file_regex)) {
					log_names.emplace_back(fpath);
				}
			}
		}
		else if (StringEndsWith(arg, ".log")) {
			log_names.emplace_back(std::move(arg));
		}
		else if (arg.front() == '-' && arg[1] != '-') {
			options.emplace_back(std::move(arg));
		}
	}
}

void Parser::InitDatabase() {
	database.Start();
	database.LoadOpcodes();
}

std::optional<PacketLog> Parser::PopNextLog() {
	std::optional<PacketLog> ret;

	while (!log_names.empty()) {
		ret.emplace(log_names.back());
		log_names.pop_back();

		LogInfo(LOG_PARSER, 0, "Processing log %s", ret->filename.c_str());
		if (ret->TransformPackets())
			break;
		else 
			LogError(LOG_PARSER, 0, "Error processing log %s!", ret->filename.c_str());
		ret.reset();
	}

	return ret;
}