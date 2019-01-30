#pragma once

#include "../PacketElement.h"

class PacketInt64 : public PacketElement {
public:
	PacketInt64(int64_t& in_int) : element(&in_int) {

	}
	~PacketInt64() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(int64_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(int64_t) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(int64_t) * count;
	}

private:
	int64_t* element;

};