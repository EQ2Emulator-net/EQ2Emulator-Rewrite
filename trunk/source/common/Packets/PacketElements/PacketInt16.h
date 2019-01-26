#pragma once

#include "../PacketElement.h"

class PacketInt16 : public PacketElement {
public:
	PacketInt16(int16_t& in_int) : element(in_int) {

	}
	~PacketInt16() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		memcpy(&element, srcbuf + offset, 2);
		offset += 2;
	}

	void WriteElement(unsigned char* outbuf) {
		memcpy(outbuf, &element, 2);
		outbuf += 2;
	}

	uint32_t GetSize() {
		return 2;
	}

private:
	int16_t& element;

};