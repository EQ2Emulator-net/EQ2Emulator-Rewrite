#pragma once

#include <map>
#include <unordered_map>
#include <string>

class ChatFilterLookup {
public:
	ChatFilterLookup() = default;
	~ChatFilterLookup() = default;

	void ProcessDatabaseResult(class DatabaseResult& result);
	uint8_t GetChatFilterID(const char* name, uint32_t version);

private:
	//<version, <name, filterID>>
	std::map<uint32_t, std::unordered_map<std::string, uint8_t> > versionedFilters;
};