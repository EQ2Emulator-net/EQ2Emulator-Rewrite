#pragma once

#include "../PacketElement.h"

class Packet32String : public PacketElement{
public:
	Packet32String(std::string& in_element) : element(in_element) {

	}

	~Packet32String() = default;

	void ReadElement(unsigned char* srcbuf, uint32_t& offset) {
		uint32_t size = 0;
		memcpy(&size, srcbuf + offset, 4);
		offset += 4;

		element.assign((char*)srcbuf + offset, size);
		offset += size;
	}

	void WriteElement(unsigned char* outbuf) {
		uint32_t size = element.length();
		memcpy(outbuf, &size, 4);
		outbuf += 4;
		memcpy(outbuf, element.c_str(), size);
		outbuf += size;
	}

	uint32_t GetSize() {
		return element.length() + 4;
	}

private:
	std::string& element;
};