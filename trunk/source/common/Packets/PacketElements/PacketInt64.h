#pragma once

#include "../PacketElement.h"

class PacketInt64 : public PacketElement {
public:
	PacketInt64(int64_t& in_int) : element(in_int) {

	}
	~PacketInt64() = default;

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
	int64_t& element;

};