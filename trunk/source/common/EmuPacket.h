#pragma once

#include "Packets\Packet.h"

enum EmuOpcode_t : uint8_t;

class EmuPacket : public Packet {
protected:
	EmuPacket(EmuOpcode_t op);

public:
	~EmuPacket() = default;

	uint32_t Write(unsigned char*& writeBuffer) override;

	bool Read(const unsigned char* in_buf, uint32_t bufsize);

	bool bCompressed;

private:
	EmuOpcode_t opcode;
};