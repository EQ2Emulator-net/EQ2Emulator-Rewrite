#pragma once

#include "../PacketElement.h"
#include "../../util.h"

class PacketOversizedByte : public PacketElement, public NetOrderElement {
public:
	PacketOversizedByte(uint16_t& element) {
		bSigned = false;
		this->element = &element;
	}

	PacketOversizedByte(int16_t& element) {
		bSigned = true;
		this->element = reinterpret_cast<uint16_t*>(&element);
	}

	~PacketOversizedByte() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		for (int i = 0; i < count; i++) {
			if (offset + 1 > bufsize) {
				return false;
			}

			uint8_t byte = srcbuf[offset++];
			bool oversized = (bSigned ? byte == 0x7f : byte == 0xff);

			if (oversized) {
				if (offset + 2 > bufsize) {
					--offset;
					return false;
				}
				memcpy(element + i, srcbuf + offset, sizeof(uint16_t));
				offset += sizeof(uint16_t);
				if (IsNetOrder()) {
					element[i] = ntohs(element[i]);
				}
			}
			else {
				element[i] = byte;
			}
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		for (int i = 0; i < count; i++) {
			union {
				uint16_t val;
				int16_t  sval;
			};
			val = element[i];
			bool oversized = (bSigned ? sval >= 0x7f : val >= 0xff);

			if (oversized) {
				outbuf[offset++] = (bSigned ? 0x7f : 0xff);

				uint16_t* pe = reinterpret_cast<uint16_t*>(outbuf + offset);
				if (IsNetOrder()) {
					*pe = htons(element[i]);
				}
				else {
					*pe = element[i];
				}

				offset += 2;
			}
			else {
				outbuf[offset++] = static_cast<uint8_t>(val);
			}
		}
	}

	uint32_t GetSize() override {
		uint32_t ret = 0;
		for (int i = 0; i < count; i++) {
			union {
				uint16_t val;
				int16_t  sval;
			};
			val = element[i];
			bool oversized = (bSigned ? sval >= 0x7f : val >= 0xff);
			ret += (oversized ? 3 : 1);
		}
		return ret;
	}

private:
	uint16_t* element;
	bool bSigned;
};