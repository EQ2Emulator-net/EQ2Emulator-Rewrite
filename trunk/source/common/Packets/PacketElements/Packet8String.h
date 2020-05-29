#pragma once

#include "../PacketElement.h"

class Packet8String : public PacketElement {
public:
	Packet8String(std::string& in_element) : element(&in_element) {

	}

	~Packet8String() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
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


			element[i].assign(reinterpret_cast<const char*>(srcbuf) + offset, size);
			offset += size;
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			uint8_t size = static_cast<uint8_t>(element[i].length());
			memcpy(outbuf + offset, &size, 1);
			offset += 1;
			memcpy(outbuf + offset, element[i].c_str(), size);
			offset += size;
		}
	}

	uint32_t GetSize() {
		uint32_t size = count * sizeof(uint8_t);
		for (int i = 0; i < count; i++) {
			size += static_cast<uint32_t>(element[i].length());
		}
		return size;
	}

	bool VariableIsSet() override {
		return !element[0].empty();
	}

private:
	std::string* element;
};