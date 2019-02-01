#pragma once

#include "../PacketElement.h"
#include "../../log.h"
#include <vector>
#include <type_traits>

class PacketSubstruct : public PacketElement {
public:
	~PacketSubstruct() {
		for (auto& itr : elements) {
			delete itr;
		}
	}

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		for (auto& itr : elements) {
			if (!itr->ReadElement(srcbuf, offset, bufsize)) {
				LogError(LOG_PACKET, 0, "Reading an element went out of bounds in packet substruct %s", name ? name : "");
				break;
			}
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		for (auto& itr : elements) {
			itr->WriteElement(outbuf, offset);
		}
	}

	uint32_t GetSize() override {
		uint32_t size = 0;
		for (auto& e : elements) {
			size += e->GetSize();
		}
		return size;
	}

protected:
	PacketSubstruct() = default;

	std::vector<PacketElement*> elements;
};

//PacketSubstructParent simply links to a PacketSubstruct object and writes it
//This is to allow for a size > 1 of PacketSubstruct. It could technically use its *this* pointer and increment it
//but that's super ugly
template<typename T>
class PacketSubstructParent : public PacketElement {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "A registered substruct element must be derived from PacketSubstruct!");

public:
	//initialized substructs with <T>
	PacketSubstructParent(T& element) : substructs(&element) {}
	~PacketSubstructParent() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		for (int i = 0; i < count; i++) {
			substructs[i].ReadElement(srcbuf, offset, bufsize);
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		for (int i = 0; i < count; i++) {
			substructs[i].WriteElement(outbuf, offset);
		}
	}

	uint32_t GetSize() override {
		uint32_t size = 0;
		for (int i = 0; i < count; i++) {
			size += substructs[i].GetSize();
		}
		return size;
	}

	T* substructs;
};