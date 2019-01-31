#pragma once

#include "../PacketElement.h"

class PacketUInt8 : public PacketElement {
public:
	PacketUInt8(uint8_t& in_int) : element(&in_int) {

	}
	~PacketUInt8() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint8_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint8_t) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return count;
	}

private:
	uint8_t* element;

};