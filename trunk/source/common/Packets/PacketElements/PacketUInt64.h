#pragma once

#include "../PacketElement.h"

class PacketUInt64 : public PacketElement {
public:
	PacketUInt64(uint64_t& in_int) : element(in_int) {

	}
	~PacketUInt64() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(&element, srcbuf + offset, 8);
		offset += 8;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, &element, 8);
		offset += 8;
	}

	uint32_t GetSize() {
		return 8;
	}

private:
	uint64_t& element;

};