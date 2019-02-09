#pragma once

#include "../../common/EmuStream.h"

class WorldStream : public EmuStream {
public:
	WorldStream(unsigned int ip, unsigned int port) : EmuStream(ip, port) {}
	~WorldStream() = default;
};