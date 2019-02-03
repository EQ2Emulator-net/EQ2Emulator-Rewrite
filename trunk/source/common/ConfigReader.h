#pragma once

#include <string>
#include "../depends/rapid-xml/rapidxml.hpp"
#include "Server.h"
#include "database.h"

using namespace rapidxml;

class ConfigReader {
public:
	ConfigReader(Server* serv, Database* db);
	~ConfigReader();

	bool ReadConfig(std::string file);

private:
	char* GetFileContent(const char* file);
	void ReadServerConfig(xml_node<>* node);
	void ReadDatabaseConfig(xml_node<>* node);
	void ReadLogConfig(xml_node<>* node);

	Server* server;
	Database* DB;
};