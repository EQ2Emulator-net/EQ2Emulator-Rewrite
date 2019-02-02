#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"
#include "../EQ2Packet.h"
#include "../EQ2Packets/OpcodeManager.h"
#include "OP_AppCombined_Packet.h"
#include "../../Crypto.h"

#ifdef _WIN32
	#include <WinSock2.h>
#else
	#include <arpa/inet.h>
#endif

class OP_Packet_Packet : public ProtocolPacket {
public:
	OP_Packet_Packet()
		: ProtocolPacket() {

		opcode = OP_Packet;
		HasCRC = true;
		p = nullptr;
		combined_p = nullptr;
	}

	OP_Packet_Packet(const unsigned char* buf, uint32_t len, uint16_t version, Crypto& crypto)
		: ProtocolPacket() {

		opcode = OP_Packet;
		HasCRC = true;
		SetVersion(version);

		memcpy(&sequence, buf, 2);
		sequence = ntohs(sequence);
		uint32_t offset = 2;

		//Check if this packet is combined
		if (buf[offset] == 0x00 && buf[offset + 1] == 0x19) {
			offset += 2;
			//This packet is combined! Create it
			combined_p = new OP_AppCombined_Packet(buf + offset, len - offset, crypto);
			return;
		}

		//This packet is not combined, try to read it
		if (version != 0) {
			p = EQ2Packet::Create(buf + offset, len - offset, crypto, version);
		}
	}

	OP_Packet_Packet(EQ2Packet* packet)
		: ProtocolPacket() {
		opcode = OP_Packet;
		HasCRC = true;
		p = packet;
		combined_p = nullptr;
	}

	~OP_Packet_Packet() {
		if (p) {
			delete p;
		}
		if (combined_p) {
			delete combined_p;
		}
	}

	uint32_t Write(unsigned char* writeBuffer) override {
		uint32_t offset = WriteOpcode(writeBuffer);
		memcpy(writeBuffer + offset, &sequence, 2);
		offset += 2;

		if (p) {
			offset += p->Write(writeBuffer + offset);
		}
		return offset;
	}

	uint32_t Write(EQ2Packet* packet) {
		EQ2Packet* p_old = p;
		p = packet;
		uint32_t ret = Packet::Write();
		p = p_old;
		return ret;
	}

	uint32_t CalculateSize() override {
		//Add 2 bytes for the sequence
		uint32_t ret = ProtocolPacket::CalculateSize() + 2;
		if (p) {
			ret += p->CalculateSize();
		}
		return ret;
	}

	EQ2Packet* p;
	OP_AppCombined_Packet* combined_p;
};
