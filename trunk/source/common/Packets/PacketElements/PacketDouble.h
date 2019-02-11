#pragma once

#include "../PacketElement.h"

class PacketDouble : public PacketElement {
public:
	PacketDouble(double& in_int) : element(&in_int) {

	}
	~PacketDouble() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(double) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(double) * count;
		memcpy(outbuf + offset, element, writeSize);
		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(double) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	/*bool CheckVariableEquality(PacketVariableEquality* cmp) override {
	return cmp->CheckVariableEquality(element[0]);
	}*/

private:
	double* element;

};