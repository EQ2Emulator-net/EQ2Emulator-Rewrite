#pragma once

#include "../PacketElement.h"

class PacketBool : public PacketElement {
public:
	PacketBool(bool& in_int) : element(&in_int) {

	}
	~PacketBool() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		for (int i = 0; i < count; ++i) {
			element[i] = srcbuf[offset + i] != 0;
		}
		offset += count;

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		for (int i = 0; i < count; ++i) {
			outbuf[offset + i] = (element[i] ? 0x01 : 0x00);
		}

		offset += count;
	}

	uint32_t GetSize() override {
		return sizeof(int8_t) * count;
	}

	bool VariableIsSet() override {
		return element[0] != false;
	}

	bool CheckVariableEquality(PacketVariableEquality* cmp) override {
		return cmp->CheckVariableEquality(element[0]);
	}

private:
	bool* element;
};