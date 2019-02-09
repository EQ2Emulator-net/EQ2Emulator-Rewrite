#pragma once

#include "../../common/EmuStream.h"

class ZoneStream : public EmuStream {
public:
	ZoneStream(uint32_t ip, uint32_t port) : EmuStream(ip, port) {}
	~ZoneStream() = default;
};