#include "stdafx.h"

#include "ConfigReader.h"
#include "util.h"
#include "string.h"
#include "log.h"
#include <fstream>
#include "../depends/rapid-xml/rapidxml_utils.hpp"

#define XML_SAFE_ATTR_VALUE(node, name) (node)->first_attribute(name) == NULL ? NULL : (node)->first_attribute(name)->value()


ConfigReader::ConfigReader(Server* serv, Database* db, Server* zone) {
	//assert(serv); tmp
	//assert(db);

	server = serv;
	DB = db;
	zonetalk = zone;
}

ConfigReader::~ConfigReader() {

}

bool ConfigReader::ReadConfig(std::string file) {
	xml_node<> *root, *node;
	auto doc = std::make_unique<xml_document<>>();

	rapidxml::file<> xmlFile(file.c_str());

	doc->parse<0>(xmlFile.data());
	root = doc->first_node("eq2emu");

	if (root != NULL) {
		if ((node = root->first_node("server")) != NULL)
			ReadServerConfig(node);
		if ((node = root->first_node("database")) != NULL)
			ReadDatabaseConfig(node);
		if ((node = root->first_node("logging")) != NULL)
			ReadLogConfig(node);
		if ((node = root->first_node("zonetalk")) != NULL)
			ReadZoneTalkConfig(node);
	}

	return true;
}

void ConfigReader::ReadServerConfig(xml_node<> *node) {
	const char *val;

	if ((val = XML_SAFE_ATTR_VALUE(node, "host")) != NULL)
		server->SetHost(val);
	if ((val = XML_SAFE_ATTR_VALUE(node, "port")) != NULL && IsUnsignedInt(val))
		server->SetPort(atoul(val));
	if ((val = XML_SAFE_ATTR_VALUE(node, "id")) != NULL && IsUnsignedInt(val))
		server->SetID(atoul(val));
}

void ConfigReader::ReadDatabaseConfig(xml_node<> *node) {
	const char *val;

	if ((val = XML_SAFE_ATTR_VALUE(node, "host")) != NULL)
		DB->SetHost(val);
	if ((val = XML_SAFE_ATTR_VALUE(node, "port")) != NULL && IsUnsignedInt(val))
		DB->SetPort(atoul(val));
	if ((val = XML_SAFE_ATTR_VALUE(node, "user")) != NULL)
		DB->SetUser(val);
	if ((val = XML_SAFE_ATTR_VALUE(node, "password")) != NULL)
		DB->SetPassword(val);
	if ((val = XML_SAFE_ATTR_VALUE(node, "database")) != NULL)
		DB->SetDatabase(val);
}

void ConfigReader::ReadLogConfig(xml_node<> *node) {
	xml_node<> *node_category, *node_type;
	const char *val1, *val2;
	LogCategory category;
	LogType type;
	int level, output, color;
	bool on;

	using namespace LoggingSystem;

	if ((val1 = XML_SAFE_ATTR_VALUE(node, "format")) != NULL && strcasecmp(val1, "xml") == 0)
		LogSetFormat(LOG_FORMAT_XML);
	if ((val1 = XML_SAFE_ATTR_VALUE(node, "split")) != NULL && strcasecmp(val1, "true") == 0)
		LogSetSplit(true);
	if ((val1 = XML_SAFE_ATTR_VALUE(node, "pid")) != NULL && strcasecmp(val1, "true") == 0)
		LogSetPID(true);

	for (node_category = node->first_node("category"); node_category != NULL; node_category = node_category->next_sibling()) {
		for (node_type = node_category->first_node("type"); node_type != NULL; node_type = node_type->next_sibling()) {
			val1 = XML_SAFE_ATTR_VALUE(node_category, "name");
			val2 = XML_SAFE_ATTR_VALUE(node_type, "name");

			if (val1 != NULL && val2 != NULL) {
				category = LogCategoryByName(val1);
				type = LogTypeByName(val2);

				if (category != LOG_INVALID && type != LOG_TYPE_INVALID) {
					on = false;
					level = 0;
					output = 0;
					color = LOG_WHITE;

					if ((val1 = XML_SAFE_ATTR_VALUE(node_type, "level")) != NULL)
						level = atoi(val1);
					if ((val1 = XML_SAFE_ATTR_VALUE(node_type, "enabled")) != NULL && strcasecmp(val1, "true") == 0)
						on = true;
					if ((val1 = XML_SAFE_ATTR_VALUE(node_type, "logs")) != NULL)
						output = atoi(val1);

					if ((val1 = XML_SAFE_ATTR_VALUE(node_type, "color")) != NULL) {
						if (strcasecmp(val1, "whitebold") == 0)        color = LOG_WHITE_BOLD;
						else if (strcasecmp(val1, "red") == 0)         color = LOG_RED;
						else if (strcasecmp(val1, "redbold") == 0)     color = LOG_RED_BOLD;
						else if (strcasecmp(val1, "green") == 0)       color = LOG_GREEN;
						else if (strcasecmp(val1, "greenbold") == 0)   color = LOG_GREEN_BOLD;
						else if (strcasecmp(val1, "blue") == 0)        color = LOG_BLUE;
						else if (strcasecmp(val1, "bluebold") == 0)    color = LOG_BLUE_BOLD;
						else if (strcasecmp(val1, "yellow") == 0)      color = LOG_YELLOW;
						else if (strcasecmp(val1, "yellowbold") == 0)  color = LOG_YELLOW_BOLD;
						else if (strcasecmp(val1, "cyan") == 0)        color = LOG_CYAN;
						else if (strcasecmp(val1, "cyanbold") == 0)    color = LOG_CYAN_BOLD;
						else if (strcasecmp(val1, "magenta") == 0)     color = LOG_MAGENTA;
						else if (strcasecmp(val1, "magentabold") == 0) color = LOG_MAGENTA_BOLD;
					}

					LogSet(category, type, on, level, output, color);
				}
			}
		}
	}
}

void ConfigReader::ReadZoneTalkConfig(xml_node<> *node) {
	const char *val;

	if ((val = XML_SAFE_ATTR_VALUE(node, "host")) != NULL)
		zonetalk->SetHost(val);
	if ((val = XML_SAFE_ATTR_VALUE(node, "port")) != NULL && IsUnsignedInt(val))
		zonetalk->SetPort(atoul(val));
}