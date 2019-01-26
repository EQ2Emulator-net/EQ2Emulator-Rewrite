#pragma once

#include <stdint.h>
#include <string>

class PacketElement {
public:
	PacketElement() { }
	virtual ~PacketElement() { }

	virtual void ReadElement(unsigned char* srcbuf, uint32_t& offset) = 0;
	virtual void WriteElement(unsigned char* outbuf) = 0;

	virtual uint32_t GetSize() = 0;

};