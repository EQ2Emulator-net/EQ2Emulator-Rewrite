#pragma once

#include "../ProtocolPacket.h"
#include "../../Crypto.h"
#include <vector>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

class OP_AppCombined_Packet : public ProtocolPacket {
public:
	OP_AppCombined_Packet()
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_AppCombined;
		HasCRC = true;
	}

	OP_AppCombined_Packet(const unsigned char* buf, uint32_t len, Crypto& crypto)
		: ProtocolPacket() {
		RegisterElements();

		opcode = OP_AppCombined;
		HasCRC = true;

		uint16_t offset = 0;
		while (offset < len) {
			uint16_t size = buf[offset];
			offset += 1;

			if (size == 0xFF) {
				//Oversized.
				memcpy(&size, buf + offset, 2);
				size = ntohs(size);
				offset += 2;
			}

			if (size > len || size == 0) {
				break;
			}

			EQ2Packet* p = EQ2Packet::Create(buf + offset, size, crypto, version);

			if (p) {
				packets.push_back(p);
			}

			offset += size;
		}
	}

	~OP_AppCombined_Packet() {
		for (auto& itr : packets) {
			delete itr;
		}
	}

	EQ2Packet* PopPacket() {
		EQ2Packet* ret = packets.back();
		packets.pop_back();
		return ret;
	}

private:
	void RegisterElements() {

	}

	vector<EQ2Packet*> packets;
};