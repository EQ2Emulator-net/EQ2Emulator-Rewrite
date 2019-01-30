#pragma once

#include <stdint.h>
#include <string>

class PacketElement {
public:
	PacketElement() { }
	virtual ~PacketElement() { }

	virtual bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) = 0;
	virtual void WriteElement(unsigned char* outbuf, uint32_t& offset) = 0;

	virtual uint32_t GetSize() = 0;


	virtual bool BoundsCheckRead(uint32_t offset, uint32_t bufsize) {
		return offset + GetSize() <= bufsize;
	}
};