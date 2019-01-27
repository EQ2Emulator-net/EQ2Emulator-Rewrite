#pragma once

#include "../PacketElement.h"

class PacketInt16 : public PacketElement {
public:
	PacketInt16(int16_t& in_int) : element(in_int) {

	}
	~PacketInt16() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(&element, srcbuf + offset, 2);
		offset += 2;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, &element, 2);
		offset += 2;
	}

	uint32_t GetSize() {
		return 2;
	}

private:
	int16_t& element;

};