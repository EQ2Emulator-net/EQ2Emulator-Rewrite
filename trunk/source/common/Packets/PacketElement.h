#pragma once

#include <stdint.h>
#include <string>

class PacketElement {
public:
	virtual ~PacketElement() = default;

	virtual bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) = 0;
	virtual void WriteElement(unsigned char* outbuf, uint32_t& offset) = 0;

	virtual uint32_t GetSize() = 0;

	virtual bool BoundsCheckRead(uint32_t offset, uint32_t bufsize) {
		return offset + GetSize() <= bufsize;
	}

	void SetCount(int32_t count) {
		this->count = count;
	}

	void SetName(const char* name) {
		this->name = name;
	}

protected:
	PacketElement() : count(1), name(nullptr) {};
	//This is the "size" element in XML structs
	int32_t count;
	//This should be a true const char.. not from a string or anything like that
	const char* name;
};