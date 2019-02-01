#pragma once

#include "../PacketElement.h"
#include "PacketSubstruct.h"
#include <type_traits>
#include <vector>

template <typename T>
class PacketArray : public PacketElement {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "PacketArray must use a class derived from PacketSubtruct for its template!");

public:
	PacketArray(std::vector<T>& in_element) : element(&in_element) {
	}
	~PacketArray() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		for (int i = 0; i < count; i++) {
			for (auto& itr : element[i]) {
				itr.ReadElement(srcbuf, offset, bufsize);
			}
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		for (int i = 0; i < count; i++) {
			for (auto& itr : element[i]) {
				itr.WriteElement(outbuf, offset);
			}
		}
	}

	uint32_t GetSize() override {
		uint32_t size = 0;
		for (int i = 0; i < count; i++) {
			for (auto& itr : element[i]) {
				size += itr.GetSize();
			}
		}
		return size;
	}

private:
	std::vector<T>* element;
};