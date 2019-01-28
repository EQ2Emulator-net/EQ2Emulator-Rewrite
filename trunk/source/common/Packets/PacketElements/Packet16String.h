#pragma once

#include "../PacketElement.h"

class Packet16String : public PacketElement{
public:
	Packet16String(std::string& in_element) : element(in_element) {

	}

	~Packet16String() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		//Bounds Check
		if (offset + sizeof(uint16_t) > bufsize) {
			return false;
		}

		uint16_t size = 0;
		memcpy(&size, srcbuf + offset, sizeof(uint16_t));

		if (offset + sizeof(uint16_t) + size > bufsize) {
			return false;
		}

		offset += 2;

		element.assign((char*)srcbuf + offset, size);
		offset += size;
		return true;
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