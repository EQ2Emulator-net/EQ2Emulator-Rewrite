#pragma once

#include "../PacketElement.h"

class PacketUInt8 : public PacketElement {
public:
	PacketUInt8(uint8_t& in_int) : element(in_int) {

	}
	~PacketUInt8() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		memcpy(&element, srcbuf + offset, 1);
		offset += 1;
	}

	void WriteElement(unsigned char* outbuf) {
		memcpy(outbuf, &element, 1);
		outbuf += 1;
	}

	uint32_t GetSize() {
		return 1;
	}

private:
	uint8_t& element;

};