#include "stdafx.h"

#include "ClientChatFilters.h"
#include "../Packets/OP_ChatFiltersMsg.h"

void ClientChatFilters::Update(const OP_ChatFiltersMsg_Packet& packet) {
	std::vector<uint8_t> newFilters;
	newFilters.reserve(packet.filtersArray.size());

	for (auto& itr : packet.filtersArray) {
		newFilters.push_back(itr.filter);
	}

	filterBytes = std::move(newFilters);
}

bool ClientChatFilters::IsChannelEnabled(uint8_t filterId) {
	if (filterBytes.empty()) {
		//Only start using filters if the client has given us a list
		return true;
	}

	uint8_t byteIndex = filterId / 8;
	uint8_t bitMask = 1 << (filterId % 8);

	return (filterBytes[byteIndex] & bitMask) != 0;
}