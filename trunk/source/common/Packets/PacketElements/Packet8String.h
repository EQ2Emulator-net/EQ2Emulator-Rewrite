#pragma once

#include "../PacketElement.h"

class Packet8String : public PacketElement{
public:
	Packet8String(std::string& in_element) : element(in_element) {

	}

	~Packet8String() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		uint8_t size = 0;
		memcpy(&size, srcbuf + offset, 1);
		offset += 1;

		element.assign((char*)srcbuf + offset, size);
		offset += size;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint8_t size = element.length();
		memcpy(outbuf + offset, &size, 1);
		offset += 1;
		memcpy(outbuf + offset, element.c_str(), size);
		offset += size;
	}

	uint32_t GetSize() {
		return element.length() + 1;
	}

private:
	std::string& element;
};