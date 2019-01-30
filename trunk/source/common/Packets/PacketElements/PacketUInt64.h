#pragma once

#include "../PacketElement.h"

class PacketUInt64 : public PacketElement {
public:
	PacketUInt64(uint64_t& in_int) : element(&in_int) {

	}
	~PacketUInt64() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint64_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint64_t) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(uint64_t) * count;
	}

private:
	uint64_t* element;

};