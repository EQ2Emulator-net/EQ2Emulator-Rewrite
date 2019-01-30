#pragma once

#include "../PacketElement.h"

class PacketInt8 : public PacketElement {
public:
	PacketInt8(int8_t& in_int) : element(&in_int) {

	}
	~PacketInt8() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(element, srcbuf + offset, count);
		offset += count;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, element, count);
		offset += count;
	}

	uint32_t GetSize() {
		return sizeof(int8_t) * count;
	}

private:
	int8_t* element;

};