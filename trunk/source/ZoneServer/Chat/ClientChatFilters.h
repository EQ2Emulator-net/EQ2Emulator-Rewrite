#pragma once

#include <vector>
#include <cstdint>

class OP_ChatFiltersMsg_Packet;

class ClientChatFilters {
public:
	ClientChatFilters() = default;
	~ClientChatFilters() = default;

	void Update(const OP_ChatFiltersMsg_Packet& p);

	bool IsChannelEnabled(uint8_t filterId);

private:
	std::vector<uint8_t> filterBytes;
};