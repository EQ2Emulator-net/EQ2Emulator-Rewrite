#pragma once

#include <stdint.h>
#include "Packet.h"
#include <zlib.h>
#include "../Crypto.h"

class PacketAllocatorBase;
class OpcodeManager;
class Client;

class EQ2Packet : public Packet {
	friend OpcodeManager;

public:
	void SetVersion(uint16_t new_version) { Version = new_version; }
	uint16_t GetVersion() { return Version; }
	uint16_t GetOpcode() { return opcode; }
	void SetOpcode(uint16_t opcode) { this->opcode = opcode; }

	bool FindOpcode();

	virtual void HandlePacket(Client* client);

	uint8_t CompressPacket(z_stream& s);
	void EncryptPacket(Crypto& crypto);

	uint32_t Write(unsigned char* buf) override {
		EQ2Compressed = false;
		PacketEncrypted = false;

		uint32_t offset;
		if (opcode >= 255) {
			buf[0] = 0xFF;
			memcpy(buf + 1, &opcode, 2);
			offset = 3;
		}
		else {
			buf[0] = static_cast<unsigned char>(opcode);
			offset = 1;
		}

		return Packet::Write(buf + offset) + offset;
	}

	uint32_t CalculateSize() override {
		return Packet::CalculateSize() + (opcode >= 255 ? 3 : 1);
	}

	bool PacketPrepared;
	bool PacketEncrypted;
	bool EQ2Compressed;

protected:
	EQ2Packet(uint16_t version);

	uint8_t app_opcode_size;
	uint16_t opcode;

private:
	uint16_t Version;
};