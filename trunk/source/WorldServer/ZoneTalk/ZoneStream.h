#pragma once

#include "../../common/EmuStream.h"

class ZoneTalk;

class ZoneStream : public EmuStream {
public:
	ZoneStream(uint32_t ip, uint32_t port) : EmuStream(ip, port) {}
	~ZoneStream() = default;

	void Process() override;
	ZoneTalk* GetServer();
};