#pragma once

#include "../PacketElement.h"

class PacketUInt16 : public PacketElement {
public:
	PacketUInt16(uint16_t& in_int) : element(in_int) {

	}
	~PacketUInt16() = default;

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
	uint16_t& element;

};