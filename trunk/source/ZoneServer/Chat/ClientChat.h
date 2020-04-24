#pragma once

#include "ClientChatFilters.h"

class ClientChat {
public:
	ClientChat() = default;
	~ClientChat() = default;


	ClientChatFilters filters;
};