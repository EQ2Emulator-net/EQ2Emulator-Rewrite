#pragma once

#include "../PacketElement.h"
#include "PacketArray.h"

class PacketUInt64 : public PacketElement, public PacketArraySize {
public:
	PacketUInt64(uint64_t& in_int) : element(&in_int) {

	}
	~PacketUInt64() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint64_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		//Array size handling
		if (myArray) {
			myArray->SetArraySize(static_cast<uint32_t>(element[0]));
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint64_t) * count;
		
		if (myArray) {
			uint64_t asize = myArray->GetArraySize();
			memcpy(outbuf + offset, &asize, 8);
		}
		else {
			memcpy(outbuf + offset, element, writeSize);
		}

		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(uint64_t) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

private:
	uint64_t* element;

};