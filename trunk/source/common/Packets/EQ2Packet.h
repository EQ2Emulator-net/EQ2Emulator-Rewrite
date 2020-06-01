#pragma once

#include <stdint.h>
#include "Packet.h"

class EQ2PacketAllocatorBase;
class OpcodeManager;
class Client;

class EQ2Packet : public Packet {
	friend OpcodeManager;
	friend class XmlStructDumper;

public:
	void SetVersion(uint32_t new_version) { Version = new_version; }
	uint32_t GetVersion() { return Version; }
	uint16_t GetOpcode() { return opcode; }
	uint8_t PreparePacket(uint16_t MaxLen);
	uint32_t serialize(unsigned char *dest) const;

	virtual void FindOpcode();

	virtual void HandlePacket(std::shared_ptr<Client> client);

	virtual void PreWrite();
	virtual void PostWrite();
	virtual void PostRead();

	virtual EQ2Packet* GetSubPacket() {
		return nullptr;
	}

	uint32_t Write(unsigned char*& buffer, uint32_t size) {
		FindOpcode();
		return Packet::Write(buffer, size);
	}

	uint32_t Write(unsigned char*& buffer) {
		FindOpcode();
		return Packet::Write(buffer);
	}

	bool TryCombine(EQ2Packet* rhs, uint32_t MaxLength);
	//This copy function only copies the following bools and buffer data, not elements etc
	EQ2Packet* CopyRaw();

	bool PacketPrepared;
	bool PacketEncrypted;
	bool EQ2Compressed;
	bool bOpcodeError;
	bool bCombined;

protected:
	EQ2Packet(uint32_t version);

	uint8_t app_opcode_size;
	uint16_t opcode;

private:
	uint32_t Version;
};