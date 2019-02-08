#pragma once

#include "../PacketElement.h"

struct EQ2ColorFloat {
	float Red;
	float Green;
	float Blue;
};

class PacketEQ2ColorFloat : public PacketElement {
public:
	PacketEQ2ColorFloat(EQ2ColorFloat& in_int) : element(&in_int) {

	}

	~PacketEQ2ColorFloat() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(element, srcbuf + offset, GetSize());
		offset += GetSize();

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, element, GetSize());
		offset += GetSize();
	}

	uint32_t GetSize() {
		return sizeof(EQ2ColorFloat) * count;
	}

private:
	EQ2ColorFloat* element;

};