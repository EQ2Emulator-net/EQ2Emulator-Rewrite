#pragma once

#include "../PacketElement.h"

class PacketCharString : public PacketElement {
public:
	PacketCharString(std::string& in_element, int16_t count) : element(&in_element), charCount(count) {

	}

	~PacketCharString() = default;

	int16_t charCount;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
			std::string& s = element[i];

			//Bounds check
			if (offset + charCount > bufsize) {
				return false;
			}

			const char* ptr = reinterpret_cast<const char*>(srcbuf + offset);
			s.assign(ptr, strnlen(ptr, charCount));
			offset += charCount;
		}
		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			std::strncpy(reinterpret_cast<char*>(outbuf + offset), element[i].c_str(), charCount);
			offset += charCount;
			//Null terminate just incase
			outbuf[-1] = 0;
		}
	}

	uint32_t GetSize() {
		return charCount * count;
	}

	bool VariableIsSet() override {
		return !element[0].empty();
	}

private:
	std::string* element;
};