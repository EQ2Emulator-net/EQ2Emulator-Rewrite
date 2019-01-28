#pragma once

#include "../PacketElement.h"

class PacketUInt8 : public PacketElement {
public:
	PacketUInt8(uint8_t& in_int) : element(in_int) {

	}
	~PacketUInt8() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(&element, srcbuf + offset, 1);
		offset += 1;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, &element, 1);
		offset += 1;
	}

	uint32_t GetSize() {
		return 1;
	}

private:
	uint8_t& element;

};