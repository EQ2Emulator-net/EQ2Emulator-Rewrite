#pragma once

#include "../PacketElement.h"

class PacketUInt64 : public PacketElement {
public:
	PacketUInt64(uint64_t& in_int) : element(in_int) {

	}
	~PacketUInt64() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		memcpy(&element, srcbuf + offset, 8);
		offset += 8;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, &element, 8);
		offset += 8;
	}

	uint32_t GetSize() {
		return 8;
	}

private:
	uint64_t& element;

};