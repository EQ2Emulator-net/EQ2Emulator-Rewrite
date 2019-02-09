#pragma once

#include <string>
#include "Server.h"
#include "../depends/rapid-xml/rapidxml.hpp"
#include "database.h"

using namespace rapidxml;

class ConfigReader {
public:
	ConfigReader(Server* serv, Database* db, Server* zonetalk);
	~ConfigReader();

	bool ReadConfig(std::string file);

private:
	char* GetFileContent(const char* file);
	void ReadServerConfig(xml_node<>* node);
	void ReadDatabaseConfig(xml_node<>* node);
	void ReadLogConfig(xml_node<>* node);
	void ReadZoneTalkConfig(xml_node<>* node);

	Server* server;
	Database* DB;
	Server* zonetalk;
};