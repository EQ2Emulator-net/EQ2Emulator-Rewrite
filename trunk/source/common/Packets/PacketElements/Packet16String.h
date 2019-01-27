#pragma once

#include "../PacketElement.h"

class Packet16String : public PacketElement{
public:
	Packet16String(std::string& in_element) : element(in_element) {

	}

	~Packet16String() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		uint16_t size = 0;
		memcpy(&size, srcbuf + offset, 2);
		offset += 2;

		element.assign((char*)srcbuf + offset, size);
		offset += size;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint16_t size = element.length();
		memcpy(outbuf + offset, &size, 2);
		offset += 2;
		memcpy(outbuf + offset, element.c_str(), size);
		offset += size;
	}

	uint32_t GetSize() {
		return element.length() + 2;
	}

private:
	std::string& element;
};