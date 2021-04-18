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
	const char* arraySizeName;
};

class PacketSubstructArray : public PacketArrayBase {
public:
	virtual void PreWrite() = 0;
	virtual void PostWrite() = 0;
	virtual void PreRead() = 0;
	virtual void PostRead() = 0;
	virtual std::unique_ptr<PacketSubstruct> GetArraySubstruct() = 0;
};

class PacketElementArrayBase : public PacketArrayBase {
public:
	virtual std::unique_ptr<PacketElement> GetArrayTypeElement() = 0;
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
class PacketArray : public PacketSubstructArray {
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
			if (itr.GetVersion() != version) {
				itr.ResetVersion(version);
			}
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

	void PreRead() override {
		for (auto& itr : *element) {
			itr.PreRead();
		}
	}

private:
	std::vector<T>* element;
	uint32_t version;
};

template <typename T, typename E>
class PacketElementArray : public PacketElementArrayBase {
	static_assert(std::is_base_of<PacketElement, E>::value, "PacketElementArray must use a class derived from PacketElement for its template!");

public:
	PacketElementArray(std::vector<T>& in_element) : element(&in_element) {
	}
	~PacketElementArray() = default;

	void SetArraySize(uint32_t size) override {
		for (int i = 0; i < count; i++) {
			element[i].resize(size);
		}
	}

	uint32_t GetArraySize() override {
		return static_cast<uint32_t>(element[0].size());
	}

	std::unique_ptr<PacketElement> GetArrayTypeElement() override {
		static T foo;
		return std::unique_ptr<E>(new E(foo));
	}

private:
	//std::vector<bool> is specialized and we can't get a direct reference to an array of bool
	template <typename X = T>
	inline std::enable_if_t<std::is_same<X, bool>::value, uint32_t> GetSizeInternal() {
		uint32_t size = 0;
		for (int i = 0; i < count; i++) {
			size += static_cast<uint32_t>(element[i].size());
		}
		return size;
	}

	template <typename X = T>
	inline std::enable_if_t<!std::is_same<X, bool>::value, uint32_t> GetSizeInternal() {
		uint32_t size = 0;
		for (int i = 0; i < count; i++) {
			auto& v = element[i];
			if (!v.empty()) {
				E e(v[0]);
				e.SetCount(static_cast<int32_t>(v.size()));
				size += e.GetSize();
			}
		}
		return size;
	}

	template <typename X = T>
	inline std::enable_if_t<std::is_same<X, bool>::value, bool>
		ReadElementInternal(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
			auto& v = element[i];
			if (!v.empty()) {
				if (offset + v.size() > bufsize) {
					return false;
				}
				for (int j = 0; j < v.size(); j++) {
					v[i] = srcbuf[offset++];
				}
			}
		}
		return true;
	}

	template <typename X = T>
	inline std::enable_if_t<!std::is_same<X, bool>::value, bool>
		ReadElementInternal(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) {
		for (int i = 0; i < count; i++) {
			auto& v = element[i];
			if (!v.empty()) {
				E e(v[0]);
				e.SetCount(static_cast<int32_t>(v.size()));
				if (!e.ReadElement(srcbuf, offset, bufsize)) {
					return false;
				}
			}
		}
		return true;
	}

	template <typename X = T>
	inline std::enable_if_t<std::is_same<X, bool>::value>
		WriteElementInternal(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			auto& v = element[i];
			for (auto& itr : v) {
				outbuf[offset++] = itr;
			}
		}
	}

	template <typename X = T>
	inline std::enable_if_t<!std::is_same<X, bool>::value>
		WriteElementInternal(unsigned char* outbuf, uint32_t& offset) {
		for (int i = 0; i < count; i++) {
			auto& v = element[i];
			if (!v.empty()) {
				E e(v[0]);
				e.SetCount(static_cast<int32_t>(v.size()));
				e.WriteElement(outbuf, offset);
			}
		}
	}

public:
	uint32_t GetSize() override {
		return GetSizeInternal();
	}

	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		return ReadElementInternal(srcbuf, offset, bufsize);
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		WriteElementInternal(outbuf, offset);
	}

private:
	std::vector<T>* element;
};