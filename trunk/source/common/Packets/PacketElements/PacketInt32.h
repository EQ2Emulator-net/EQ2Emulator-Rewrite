#pragma once

#include "../PacketElement.h"

class PacketInt32 : public PacketElement {
public:
	PacketInt32(int32_t& in_int) : element(&in_int) {

	}
	~PacketInt32() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(int32_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(int32_t) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(int32_t) * count;
	}

private:
	int32_t* element;

};