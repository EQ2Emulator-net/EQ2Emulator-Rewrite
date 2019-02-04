#pragma once

#include "../PacketElement.h"
#include "PacketEQ2Color.h"

struct EQ2EquipmentItem {
	uint16_t type;
	EQ2Color color;
	EQ2Color highlight;
};

class PacketEQ2EquipmentItem : public PacketElement {
public:
	PacketEQ2EquipmentItem(EQ2EquipmentItem& in_int) : element(&in_int) {

	}

	~PacketEQ2EquipmentItem() = default;

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
		return sizeof(EQ2EquipmentItem) * count;
	}

private:
	EQ2EquipmentItem* element;

};