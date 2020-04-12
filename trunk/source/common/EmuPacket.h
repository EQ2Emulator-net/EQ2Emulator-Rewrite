#pragma once

#include "Packets/Packet.h"

enum class EmuOpcode_t : uint8_t;
class WorldStream;
class ZoneStream;

class EmuPacket : public Packet {
protected:
	EmuPacket(EmuOpcode_t op);

public:
	~EmuPacket() = default;

	uint32_t Write(unsigned char*& writeBuffer) override;

	bool Read(const unsigned char* in_buf, uint32_t bufsize);

	bool bCompressed;

#ifdef EQ2_WORLD
	virtual void HandlePacket(std::shared_ptr<ZoneStream> s) { LogError(LOG_PACKET, 0, "Called HandlePacket(ZoneStream*) on packet %u which has no handler.", opcode); }
#else
	virtual void HandlePacket(std::shared_ptr<WorldStream> s) { LogError(LOG_PACKET, 0, "Called HandlePacket(WorldStream*) on packet %u which has no handler.", opcode); }
#endif

private:
	EmuOpcode_t opcode;
};