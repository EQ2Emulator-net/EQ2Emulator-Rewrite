#pragma once

#include "../PacketElement.h"
#include "../../util.h"

class PacketOversizedByte : public PacketElement, public NetOrderElement, public PacketArraySize {
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

			union {
				uint8_t byte;
				int8_t sbyte;
			};
			
			byte = srcbuf[offset++];
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
				union {
					uint16_t newVal;
					int16_t snewVal;
				};
				if (bSigned) {
					snewVal = sbyte;
				}
				else {
					newVal = byte;
				}

				element[i] = newVal;
			}
		}

		//Array size handling
		if (myArray) {
			myArray->SetArraySize(element[0]);
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		//Array size handling
		if (myArray) {
			element[0] = myArray->GetArraySize();
		}

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
				union {
					uint8_t newVal;
					int8_t snewVal;
				};

				if (bSigned) {
					snewVal = static_cast<int8_t>(sval);
				}
				else {
					newVal = static_cast<uint8_t>(val);
				}

				outbuf[offset++] = newVal;
			}
		}
	}

	uint32_t GetSize() override {
		uint32_t ret = 0;

		//Array size handling
		if (myArray) {
			ret = myArray->GetArraySize() >= 255 ? 3 : 1;
		}
		else {
			for (int i = 0; i < count; i++) {
				union {
					uint16_t val;
					int16_t  sval;
				};
				val = element[i];
				bool oversized = (bSigned ? sval >= 0x7f : val >= 0xff);
				ret += (oversized ? 3 : 1);
			}
		}
		return ret;
	}

	bool VariableIsSet() override {
		return element[0] != 0;
	}

	bool CheckVariableEquality(PacketVariableEquality* e) override {
		return e->CheckVariableEquality(element[0]);
	}

	bool bSigned;
private:
	uint16_t* element;
};