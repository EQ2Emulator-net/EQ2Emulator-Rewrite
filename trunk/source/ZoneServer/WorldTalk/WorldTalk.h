#pragma once

#include "../../common/TCPServer.h"

class Stream;

class WorldTalk : public TCPServer {
public:
	WorldTalk() : TCPServer(false) {}
	~WorldTalk() = default;

	Stream* GetNewStream(unsigned int ip, unsigned short port) override;
};