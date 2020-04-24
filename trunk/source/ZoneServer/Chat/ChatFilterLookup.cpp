#include "stdafx.h"

#include "ChatFilterLookup.h"
#include "../Database/ZoneDatabase.h"
#include "../../common/log.h"

bool ZoneDatabase::LoadChatFilters(ChatFilterLookup& filters) {
	DatabaseResult res;
	if (!Select(&res, "SELECT version, id, name FROM chat_filters")) {
		LogDebug(LOG_CHAT, 0, "Could not load chat filters!");
		return false;
	}

	filters.ProcessDatabaseResult(res);

	return true;
}

void ChatFilterLookup::ProcessDatabaseResult(DatabaseResult& result) {
	uint32_t count = 0;
	while (result.Next()) {
		uint32_t ver = result.GetUInt32(0);
		uint8_t filterID = result.GetUInt8(1);
		std::string name = result.GetString(2);

		versionedFilters[ver][name] = filterID;
		count++;
	}

	LogDebug(LOG_CHAT, 0, "Successfully loaded %u chat filters.", count);
}

uint8_t ChatFilterLookup::GetChatFilterID(const char* name, uint32_t version, const char* alternate) {
	uint8_t ret = 0xFF;
	std::unordered_map<std::string, uint8_t>* lookup = nullptr;

	//Find the highest version filters we can use
	for (auto& itr : versionedFilters) {
		if (itr.first > version) {
			break;
		}

		lookup = &itr.second;
	}

	if (!lookup) {
		return ret;
	}

	auto itr = lookup->find(name);

	if (itr != lookup->end()) {
		//We found the channel. return the id
		ret = itr->second;
	}
	
	if (ret == 0xFF && alternate) {
		//Maybe a channel isn't available on all clients? Provide something more generic
		itr = lookup->find(alternate);
		if (itr != lookup->end()) {
			ret = itr->second;
		}
	}

	return ret;
}