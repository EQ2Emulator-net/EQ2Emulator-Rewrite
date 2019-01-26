#pragma once

#include "../PacketElement.h"

class PacketInt32 : public PacketElement {
public:
	PacketInt32(int32_t& in_int) : element(in_int) {

	}
	~PacketInt32() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		memcpy(&element, srcbuf + offset, 4);
		offset += 4;
	}

	void WriteElement(unsigned char* outbuf) {
		memcpy(outbuf, &element, 4);
		outbuf += 4;
	}

	uint32_t GetSize() {
		return 4;
	}

private:
	int32_t& element;

};