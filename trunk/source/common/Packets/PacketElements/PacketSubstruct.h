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
		CheckElementsInitialized();
		for (auto& itr : elements) {
			if (!itr->MeetsCriteria()) {
				continue;
			}
			if (!itr->ReadElement(srcbuf, offset, bufsize)) {
				LogError(LOG_PACKET, 0, "Reading an element went out of bounds in packet substruct %s", name ? name : "");
				return false;
			}
		}

		return true;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		CheckElementsInitialized();
		for (auto& itr : elements) {
			if (!itr->MeetsCriteria()) {
				continue;
			}

			itr->WriteElement(outbuf, offset);
		}
	}

	uint32_t GetSize() override {
		CheckElementsInitialized();

		uint32_t size = 0;
		for (auto& e : elements) {
			size += e->GetSize();
		}
		return size;
	}

	virtual void RegisterElements() {
		elementsInitialized = true;
	}

	void CheckElementsInitialized() {
		if (elementsInitialized) {
			return;
		}

		if (!elements.empty()) {
			elementsInitialized = true;
			return;
		}

		elementsInitialized = true;
		RegisterElements();
	}

protected:
	PacketSubstruct(): elementsInitialized(false) {}

	//Copy constructor
	PacketSubstruct(const PacketSubstruct& other) {
		elements.reserve(other.elements.size());
		elementsInitialized = false;
	}

	//Move constructor
	PacketSubstruct(PacketSubstruct&& other) {
		elements.reserve(other.elements.size());
		elementsInitialized = false;
	}

	std::vector<PacketElement*> elements;

private:
	bool elementsInitialized = false;
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
			if (!substructs[i].ReadElement(srcbuf, offset, bufsize)) {
				return false;
			}
		}

		return true;
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