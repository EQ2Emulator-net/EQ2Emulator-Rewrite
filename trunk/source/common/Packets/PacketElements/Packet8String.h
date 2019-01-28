#pragma once

#include "../PacketElement.h"

class Packet8String : public PacketElement {
public:
	Packet8String(std::string& in_element) : element(in_element) {

	}

	~Packet8String() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		//Bounds check
		if (offset + sizeof(uint8_t) > bufsize) {
			return false;
		}

		uint8_t size = 0;
		memcpy(&size, srcbuf + offset, sizeof(uint8_t));

		if (offset + sizeof(uint8_t) + size > bufsize) {
			return false;
		}

		offset += 1;


		element.assign(reinterpret_cast<const char*>(srcbuf) + offset, size);
		offset += size;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint8_t size = (uint8_t)element.length();
		memcpy(outbuf + offset, &size, 1);
		offset += 1;
		memcpy(outbuf + offset, element.c_str(), size);
		offset += size;
	}

	uint32_t GetSize() {
		return (uint32_t)(element.length() + 1);
	}

private:
	std::string& element;
};