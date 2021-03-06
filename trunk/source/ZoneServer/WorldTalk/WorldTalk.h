#pragma once

#include "../../common/TCPServer.h"

class Stream;

class WorldTalk : public TCPServer {
public:
	WorldTalk() : TCPServer(false) {}
	~WorldTalk() = default;

	std::shared_ptr<Stream> GetNewStream(unsigned int ip, unsigned short port) override;

	bool Process() override;
};