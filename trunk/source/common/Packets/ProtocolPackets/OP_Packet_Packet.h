#pragma once

#include "../ProtocolPacket.h"
#include "../PacketElements/PacketElements.h"
#include "../EQ2Packet.h"
#include "../EQ2Packets/OpcodeManager.h"

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
	}

	OP_Packet_Packet(const unsigned char* buf, uint32_t len, uint16_t version)
		: ProtocolPacket() {

		opcode = OP_Packet;
		HasCRC = true;
		SetVersion(version);

		memcpy(&sequence, buf, 2);
		sequence = ntohs(sequence);
		uint32_t offset = 2;

		//Make an EQ2Packet here?
		uint16_t dataOp = buf[offset];
		if (dataOp == 255) {
			offset += 1;
			//oversized
			memcpy(&dataOp, buf + offset, 2);
			offset += 2;
		}
		
		if (version != 0) {
			//Handle crypto from here?
			p = nullptr;
			//p = OpcodeManager::GetGlobal()->GetPacketForVersion(version, dataOp);
			//p->SetOpcode(dataOp);
			//p->Read(buf, offset, len);
		}
	}

	OP_Packet_Packet(EQ2Packet* packet)
		: ProtocolPacket() {
		opcode = OP_Packet;
		HasCRC = true;
		p = packet;
	}

	~OP_Packet_Packet() {
		if (p) {
			delete p;
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

private:
	EQ2Packet* p;
};
