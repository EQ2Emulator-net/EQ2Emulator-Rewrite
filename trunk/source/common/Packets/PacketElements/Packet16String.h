#pragma once

#include "../PacketElement.h"

class Packet16String : public PacketElement{
public:
	Packet16String(std::string& in_element) : element(&in_element) {

	}

	~Packet16String() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
			//Bounds check
			if (offset + sizeof(uint16_t) > bufsize) {
				return false;
			}

			uint16_t size = 0;
			memcpy(&size, srcbuf + offset, sizeof(uint16_t));

			if (offset + sizeof(uint16_t) + size > bufsize) {
				return false;
			}

			offset += sizeof(uint16_t);


			element[i].assign(reinterpret_cast<const char*>(srcbuf) + offset, size);
			offset += size;

			return true;
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			uint16_t size = static_cast<uint16_t>(element[i].length());
			memcpy(outbuf + offset, &size, sizeof(uint16_t));
			offset += sizeof(uint16_t);
			memcpy(outbuf + offset, element[i].c_str(), size);
			offset += size;
		}
	}

	uint32_t GetSize() {
		uint32_t size = count * sizeof(uint16_t);
		for (int i = 0; i < count; i++) {
			size += static_cast<uint32_t>(element[i].length());
		}
		return size;
	}

private:
	std::string* element;
};