#pragma once

#include <stdint.h>
#include "Packet.h"

class EQ2PacketAllocatorBase;
class OpcodeManager;
class Client;

class EQ2Packet : public Packet {
	friend OpcodeManager;

public:
	void SetVersion(uint32_t new_version) { Version = new_version; }
	uint32_t GetVersion() { return Version; }
	uint16_t GetOpcode() { return opcode; }
	uint8_t PreparePacket(uint16_t MaxLen);
	uint32_t serialize(unsigned char *dest) const;

	void FindOpcode();

	virtual void HandlePacket(Client* client);

	bool PacketPrepared;
	bool PacketEncrypted;
	bool EQ2Compressed;

protected:
	EQ2Packet(uint32_t version);

	uint8_t app_opcode_size;
	uint16_t opcode;

private:
	uint32_t Version;
};