#pragma once

#include "../PacketElement.h"
#include "PacketArray.h"

class PacketChar : public PacketElement, public PacketArraySize {
public:
	PacketChar(char& in_int) : element(&in_int) {

	}
	~PacketChar() = default;

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
		if (myArray) {
			uint8_t asize = static_cast<uint8_t>(myArray->GetArraySize());
			outbuf[offset] = asize;
		}
		else {
			memcpy(outbuf + offset, element, count);
		}
		offset += count;
	}

	uint32_t GetSize() {
		return sizeof(char) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}

private:
	char* element;

};