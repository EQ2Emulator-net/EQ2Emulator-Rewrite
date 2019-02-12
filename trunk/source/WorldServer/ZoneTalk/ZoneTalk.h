#pragma once

#include "../../common/TCPServer.h"

class ZoneTalk : public TCPServer {
public:
	ZoneTalk();
	~ZoneTalk() = default;

	Stream* GetNewStream(unsigned int ip, unsigned short port) override;

	bool Process() override;
};