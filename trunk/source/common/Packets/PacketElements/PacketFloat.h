#pragma once

#include "../PacketElement.h"

class PacketFloat : public PacketElement {
public:
	PacketFloat(float& in_int) : element(&in_int) {

	}
	~PacketFloat() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(float) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(float) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(float) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	/*bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}*/

private:
	float* element;

};