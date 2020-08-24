#pragma once

#include "../PacketElement.h"
#include "PacketEQ2Color.h"

struct EQ2EquipmentItem {
	EQ2EquipmentItem() : type(0) {}
	uint32_t type;
	EQ2Color color;
	EQ2Color highlight;
};

class PacketEQ2EquipmentItem : public PacketElement {
	friend class XmlStructDumper;
public:
	PacketEQ2EquipmentItem(EQ2EquipmentItem& in_int, bool _bShortType) : element(&in_int), bShortType(_bShortType) {

	}

	~PacketEQ2EquipmentItem() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		if (!BoundsCheckRead(offset, bufsize)) {
			return false;
		}

		for (int i = 0; i < count; i++) {
			EQ2EquipmentItem& e = element[i];
			if (bShortType) {
				e.type = *reinterpret_cast<const uint16_t*>(srcbuf + offset);
				offset += 2;
			}
			else {
				e.type = *reinterpret_cast<const uint32_t*>(srcbuf + offset);
				offset += 4;
			}
			memcpy(&e.color, srcbuf + offset, 3);
			offset += 3;
			memcpy(&e.highlight, srcbuf + offset, 3);
			offset += 3;
		}
		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			const EQ2EquipmentItem& e = element[i];
			if (bShortType) {
				*reinterpret_cast<uint16_t*>(outbuf + offset) = static_cast<uint16_t>(e.type);
				offset += 2;
			}
			else {
				*reinterpret_cast<uint32_t*>(outbuf + offset) = e.type;
				offset += 4;
			}
			memcpy(outbuf + offset, &e.color, 3);
			offset += 3;
			memcpy(outbuf + offset, &e.highlight, 3);
			offset += 3;
		}
	}

	uint32_t GetSize() {
		if (bShortType) {
			//Type was changed from a 2 byte value to a 4 byte value in later clients
			return 8 * count;
		}

		return 10 * count;
	}

private:
	EQ2EquipmentItem* element;
	bool bShortType;
};