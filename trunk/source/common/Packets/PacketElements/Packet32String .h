#pragma once

#include "../PacketElement.h"

class Packet32String : public PacketElement{
public:
	Packet32String(std::string& in_element) : element(&in_element) {

	}

	~Packet32String() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
			//Bounds check
			if (offset + sizeof(uint32_t) > bufsize) {
				return false;
			}

			uint32_t size = 0;
			memcpy(&size, srcbuf + offset, sizeof(uint32_t));

			if (offset + sizeof(uint32_t) + size > bufsize) {
				return false;
			}

			offset += sizeof(uint32_t);


			element[i].assign(reinterpret_cast<const char*>(srcbuf) + offset, size);
			offset += size;

			return true;
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			uint32_t size = static_cast<uint32_t>(element[i].length());
			memcpy(outbuf + offset, &size, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			memcpy(outbuf + offset, element[i].c_str(), size);
			offset += size;
		}
	}

	uint32_t GetSize() {
		uint32_t size = count * sizeof(uint32_t);
		for (int i = 0; i < count; i++) {
			size += static_cast<uint32_t>(element[i].length());
		}
		return size;
	}

private:
	std::string* element;
};