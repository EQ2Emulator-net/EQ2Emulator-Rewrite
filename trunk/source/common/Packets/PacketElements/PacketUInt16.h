#pragma once

#include "../PacketElement.h"

class PacketUInt16 : public PacketElement {
public:
	PacketUInt16(uint16_t& in_int) : element(&in_int) {

	}
	~PacketUInt16() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint16_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;
		
		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint16_t) * count;
		memcpy(outbuf + offset, element, 2);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(uint16_t) * count;
	}

private:
	uint16_t* element;

};