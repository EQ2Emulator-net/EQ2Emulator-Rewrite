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

		if (bShortType) {
			for (int i = 0; i < count; i++) {
				EQ2EquipmentItem& e = element[i];
				e.type = *reinterpret_cast<const uint16_t*>(srcbuf + offset);
				offset += 2;
				memcpy(&e.color, srcbuf + offset, 6);
				offset += 6;
			}
		}
		else {
			uint32_t readSize = GetSize();
			memcpy(element, srcbuf + offset, readSize);
			offset += readSize;
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) {
		if (bShortType) {
			for (int i = 0; i < count; i++) {
				const EQ2EquipmentItem& e = element[i];
				*reinterpret_cast<uint16_t*>(outbuf + offset) = static_cast<uint16_t>(e.type);
				offset += 2;
				memcpy(outbuf + offset, &e.color, 6);
				offset += 6;
			}
		}
		else {
			uint32_t writeSize = GetSize();
			memcpy(outbuf + offset, element, writeSize);
			offset += writeSize;
		}
		
	}

	uint32_t GetSize() {
		if (bShortType) {
			//Type was changed from a 2 byte value to a 4 byte value in later clients
			return 8 * count;
		}

		return sizeof(EQ2EquipmentItem) * count;
	}

private:
	EQ2EquipmentItem* element;
	bool bShortType;
};