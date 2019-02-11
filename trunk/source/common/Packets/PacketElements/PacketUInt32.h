#pragma once

#include "../PacketElement.h"
#include "PacketArray.h"

class PacketUInt32 : public PacketElement, public PacketArraySize {
public:
	PacketUInt32(uint32_t& in_int) : element(&in_int) {

	}
	~PacketUInt32() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint32_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		//Array size handling
		if (myArray) {
			myArray->SetArraySize(element[0]);
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint32_t) * count;
		
		if (myArray) {
			uint32_t asize = static_cast<uint32_t>(myArray->GetArraySize());
			memcpy(outbuf + offset, &asize, 4);
		}
		else {
			memcpy(outbuf + offset, element, writeSize);
		}

		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(uint32_t) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}

private:
	uint32_t* element;

};