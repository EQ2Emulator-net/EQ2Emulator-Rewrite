#pragma once

#include "../PacketElement.h"
#include "PacketArray.h"

class PacketInt8 : public PacketElement, public PacketArraySize {
public:
	PacketInt8(int8_t& in_int) : element(&in_int) {

	}
	~PacketInt8() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		memcpy(element, srcbuf + offset, count);
		offset += count;

		//Array size handling
		if (myArray) {
			myArray->SetArraySize(element[0]);
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		memcpy(outbuf + offset, element, count);
		offset += count;
	}

	uint32_t GetSize() {
		return sizeof(int8_t) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}

private:
	int8_t* element;

};