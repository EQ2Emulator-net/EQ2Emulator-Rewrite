#pragma once

#include "../PacketElement.h"

class Packet32String : public PacketElement{
public:
	Packet32String(std::string& in_element) : element(in_element) {

	}

	~Packet32String() = default;

	bool ReadElement(unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		//Bounds Check
		if (offset + sizeof(uint32_t) > bufsize) {
			return false;
		}

		uint32_t size = 0;
		memcpy(&size, srcbuf + offset, 4);

		if (offset + sizeof(uint32_t) + size > bufsize) {
			return false;
		}

		offset += 4;

		element.assign((char*)srcbuf + offset, size);
		offset += size;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t size = element.length();
		memcpy(outbuf + offset, &size, 4);
		offset += 4;
		memcpy(outbuf + offset, element.c_str(), size);
		offset += size;
	}

	uint32_t GetSize() {
		return element.length() + 4;
	}

private:
	std::string& element;
};