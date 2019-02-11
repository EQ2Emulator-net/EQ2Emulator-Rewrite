#pragma once

#include "../PacketElement.h"
#include "PacketCriteria.h"
#include "../../util.h"

class PacketUInt16 : public PacketElement, public PacketArraySize, public NetOrderElement {
public:
	PacketUInt16(uint16_t& in_int) : element(&in_int) {

	}
	~PacketUInt16() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		uint32_t readSize = sizeof(uint16_t) * count;
		memcpy(element, srcbuf + offset, readSize);
		offset += readSize;

		if (IsNetOrder()) {
			for (int i = 0; i < count; i++) {
				element[i] = ntohs(element[i]);
			}
		}

		//Array size handling
		if (myArray) {
			myArray->SetArraySize(element[0]);
		}
		
		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		uint32_t writeSize = sizeof(uint16_t) * count;

		if (myArray) {
			uint16_t asize = static_cast<uint16_t>(myArray->GetArraySize());
			if (IsNetOrder()) {
				asize = htons(asize);
			}
			memcpy(outbuf + offset, &asize, 2);
		}
		else {
			if (IsNetOrder()) {
				uint16_t* pe = reinterpret_cast<uint16_t*>(outbuf + offset);
				for (int i = 0; i < count; i++) {
					pe[i] = htons(pe[i]);
				}
			}
			else {
				memcpy(outbuf + offset, element, writeSize);
			}
		}

		offset += writeSize;
	}

	uint32_t GetSize() {
		return sizeof(uint16_t) * count;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}

private:
	uint16_t* element;

};