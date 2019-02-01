#pragma once

#include "../PacketElement.h"
#include "PacketSubstruct.h"
#include <type_traits>
#include <vector>

class PacketArrayBase {
protected:
	PacketArrayBase() = default;
	virtual ~PacketArrayBase() = default;
	
public:
	virtual void SetArraySize(uint32_t size) = 0;
};

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
class PacketArray : public PacketElement, public PacketArrayBase {
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

private:
	std::vector<T>* element;
};