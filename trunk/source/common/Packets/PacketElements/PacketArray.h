#pragma once

#include "../PacketElement.h"
#include "PacketSubstruct.h"
#include <type_traits>
#include <vector>
#include <memory>

class PacketArrayBase : public PacketElement {
protected:
	PacketArrayBase() : arraySizeName("") {}
	virtual ~PacketArrayBase() = default;
	
public:
	virtual void SetArraySize(uint32_t size) = 0;
	virtual uint32_t GetArraySize() = 0;
	virtual std::unique_ptr<PacketSubstruct> GetArraySubstruct() = 0;
	virtual void PreWrite() = 0;
	virtual void PostWrite() = 0;
	virtual void PostRead() = 0;
	const char* arraySizeName;
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
		if (auto e = dynamic_cast<PacketElement*>(this)) {
			myArray->arraySizeName = e->name;
		}
	}
};

//This element should link to a vector (or vectors) of an object derived from PacketSubstruct
template <typename T>
class PacketArray : public PacketArrayBase {
	static_assert(std::is_base_of<PacketSubstruct, T>::value, "PacketArray must use a class derived from PacketSubtruct for its template!");

public:
	PacketArray(std::vector<T>& in_element) : element(&in_element), version(0) {
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
				if (itr.GetVersion() != version) {
					itr.ResetVersion(version);
				}
				if (MeetsCriteria()) {
					size += itr.GetSize();
				}
			}
		}
		return size;
	}

	void SetArraySize(uint32_t size) override {
		for (int i = 0; i < count; i++) {
			element[i].resize(size);
			for (auto& itr : element[i]) {
				itr.ResetVersion(version);
			}
		}
	}

	uint32_t GetArraySize() override {
		return static_cast<uint32_t>(element[0].size());
	}

	void SetVersion(uint32_t ver) {
		version = ver;
	}

	std::unique_ptr<PacketSubstruct> GetArraySubstruct() override {
		T* ret = new T(version);
		return std::unique_ptr<PacketSubstruct>(static_cast<PacketSubstruct*>(ret));
	}

	void PreWrite() override {
		for (auto& itr : *element) {
			itr.PreWrite();
		}
	}

	void PostWrite() override {
		for (auto& itr : *element) {
			itr.PostWrite();
		}
	}
	
	void PostRead() override {
		for (auto& itr : *element) {
			itr.PostRead();
		}
	}

private:
	std::vector<T>* element;
	uint32_t version;
};