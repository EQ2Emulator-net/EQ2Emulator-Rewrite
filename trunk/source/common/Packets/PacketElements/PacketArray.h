#pragma once

#include "../PacketElement.h"
#include "PacketSubstruct.h"
#include <type_traits>
#include <vector>

class PacketArrayBase : public PacketElement {
protected:
	PacketArrayBase() = default;
	virtual ~PacketArrayBase() = default;
	
public:
	virtual void SetArraySize(uint32_t size) = 0;
	virtual uint32_t GetArraySize() = 0;
};

//Just a note to self for xml structs
//You can use dynamic_cast<PacketArraySize*>(this) in a PacketElement function to check if we derive from this class
class PacketArraySize {
protected:
	PacketArrayBase* myArray;

	PacketArraySize() : myArray(nullptr) {}
	virtual ~PacketArraySize() = default;

public:
	void SetMyArray(PacketArrayBase* a) {
		myArray = a;
	}
};

//This element should link to a vector (or vectors) of an object derived from PacketSubstruct
template <typename T>
class PacketArray : public PacketArrayBase {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "PacketArray must use a class derived from PacketSubtruct for its template!");

public:
	PacketArray(std::vector<T>& in_element) : element(&in_element) {
	}
	~PacketArray() = default;

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		for (int i = 0; i < count; i++) {
			for (auto& itr : element[i]) {
				if (!itr.ReadElement(srcbuf, offset, bufsize)) {
					return false;
				}
			}
		}

		return true;
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

	void SetArraySize(uint32_t size) override {
		for (int i = 0; i < count; i++) {
			element[i].resize(size);
		}
	}

	uint32_t GetArraySize() override {
		return static_cast<uint32_t>(element[0].size());
	}

private:
	std::vector<T>* element;
};