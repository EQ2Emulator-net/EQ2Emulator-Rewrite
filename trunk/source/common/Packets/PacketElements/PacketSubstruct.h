#pragma once

#include "../PacketElement.h"
#include "../../log.h"
#include <vector>
#include <type_traits>

class PacketSubstruct : public PacketElement {
public:
	friend class XmlStructDumper;
	~PacketSubstruct() {
		for (auto& itr : elements) {
			delete itr;
		}
	}

	void ResetVersion(uint32_t p_version) {
		if (version != p_version) {
			version = p_version;
			for (auto& itr : elements) {
				delete itr;
			}
			elements.clear();
		}
		RegisterElements();
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
			if (e->MeetsCriteria()) {
				size += e->GetSize();
			}
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

	bool IsInline() {
		return bInline;
	}

	uint32_t GetVersion() { return version; }

	virtual void PreWrite();
	virtual void PostWrite();
	virtual void PostRead();

protected:
	PacketSubstruct(uint32_t p_version, bool p_inline = false): elementsInitialized(false), version(p_version), bInline(p_inline) {
	}

	//Copy constructor
	PacketSubstruct(const PacketSubstruct& other) noexcept {
		elements.reserve(other.elements.size());
		version = other.version;
		elementsInitialized = false;
		bInline = other.bInline;
	}

	//Move constructor
	PacketSubstruct(PacketSubstruct&& other) noexcept {
		elements.reserve(other.elements.size());
		version = other.version;
		elementsInitialized = false;
		bInline = other.bInline;
	}

	std::vector<PacketElement*> elements;

	uint32_t version;

private:
	bool elementsInitialized;
	bool bInline;
};

class PacketSubstructParentBase : public PacketElement {
protected:
	PacketSubstructParentBase() = default;
	~PacketSubstructParentBase() = default;

public:
	virtual PacketSubstruct* GetSubstructs() = 0;
	virtual void PreWrite() = 0;
	virtual void PostWrite() = 0;
	virtual void PostRead() = 0;
};

//PacketSubstructParent simply links to a PacketSubstruct object and writes it
//This is to allow for a size > 1 of PacketSubstruct. It could technically use its *this* pointer and increment it
//but that's super ugly
template<typename T>
class PacketSubstructParent : public PacketSubstructParentBase {
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

	PacketSubstruct* GetSubstructs() override {
		return substructs;
	}

	void PreWrite() override {
		for (int i = 0; i < count; i++) {
			substructs[i].PreWrite();
		}
	}

	void PostWrite() override {
		for (int i = 0; i < count; i++) {
			substructs[i].PostWrite();
		}
	}

	void PostRead() override {
		for (int i = 0; i < count; i++) {
			substructs[i].PostWrite();
		}
	}
};