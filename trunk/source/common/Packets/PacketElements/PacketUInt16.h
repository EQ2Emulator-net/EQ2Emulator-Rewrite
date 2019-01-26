#pragma once

#include "../PacketElement.h"

class PacketUInt16 : public PacketElement {
public:
	PacketUInt16(uint16_t& in_int) : element(in_int) {

	}
	~PacketUInt16() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		memcpy(&element, srcbuf + offset, 2);
		offset += 2;
	}

	void WriteElement(unsigned char* outbuf) {
		memcpy(outbuf, &element, 2);
		outbuf += 2;
	}

	uint32_t GetSize() {
		return 2;
	}

private:
	uint16_t& element;

};