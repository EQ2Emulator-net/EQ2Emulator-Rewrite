#include "stdafx.h"

#include "Parser.h"
#include "ParserDatabase.h"
#include "../ConfigReader.h"
#include "../log.h"
#include "../util.h"
#include "../DBRowBatcher.h"

#include <regex>
#include <filesystem>
#include <map>
#include <vector>
#include <fstream>

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

void Parser::PrintLogVersions() {
	std::map<uint32_t, std::vector<std::string> > vers;

	for (auto & itr : log_names) {
		PacketLog l(itr);
		if (l.TransformPackets(true)) {
			vers[l.logVersion].emplace_back(itr);
		}
		else {
			LogDebug(LOG_PARSER, 0, "Unable to find log version: \"%s\"", itr.c_str());
		}
	}

	std::ostringstream ss;
	ss << "Versions:";

	for (auto& itr : vers) {
		ss << "\n\t" << itr.first << " : " << itr.second[0];
	}

	LogDebug(LOG_PARSER, 0, ss.str().c_str());

	//Dump all of these to a sql file
	DBRowBatcher batcher;

	for (auto& itr : vers) {
		uint32_t ver = itr.first;
		auto& logs = itr.second;

		for (auto& itr : logs) {
			DatabaseRow row;
			row.m_tableName = "eq2log_versions";
			row.RegisterField("version", ver);
			row.RegisterField("log", itr);
			batcher.QueueRowInsert(row);
		}
	}

	auto queries = batcher.ProduceInsertsForTable("eq2log_versions", 250, false);

	std::ofstream fullDump("eq2logVersions.sql", std::ios::trunc);

	for (auto& itr : queries) {
		fullDump << itr << "\n";
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