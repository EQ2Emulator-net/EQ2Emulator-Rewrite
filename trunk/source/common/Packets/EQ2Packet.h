#pragma once

#include <stdint.h>
#include "Packet.h"

class EQ2Packet : public Packet {
public:
	EQ2Packet();

	void SetVersion(uint16_t new_version) { Version = new_version; }
	uint16_t GetVersion() { return Version; }
	uint16_t GetOpcode() { return opcode; }
	uint8_t PreparePacket(uint16_t MaxLen);
	uint32_t serialize(unsigned char *dest) const;

	bool PacketPrepared;
	bool PacketEncrypted;
	bool EQ2Compressed;

protected:
	uint8_t app_opcode_size;
	uint16_t opcode;

private:
	uint16_t Version;

};