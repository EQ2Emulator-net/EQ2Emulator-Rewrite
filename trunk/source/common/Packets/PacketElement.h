#pragma once

#include <stdint.h>
#include <string>
#include <cassert>
#include "PacketElements/PacketCriteria.h"
#include "../EmuAssert.h"

class PacketElement : public PacketVariableEquality {
	friend class XmlStructDumper;
	friend class PacketArraySize;
public:
	virtual ~PacketElement() = default;

	virtual bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) = 0;
	virtual void WriteElement(unsigned char* outbuf, uint32_t& offset) = 0;

	virtual uint32_t GetSize() = 0;

	virtual bool BoundsCheckRead(uint32_t offset, uint32_t bufsize) {
		return offset + GetSize() <= bufsize;
	}

	void SetCount(int32_t count) {
		this->count = count;
	}

	void SetName(const char* name) {
		this->name = name;
	}

	void SetIsVariableSet(PacketElement* e) {
		ifVariableSet = e;
	}

	void AddIfAnyVariableSet(PacketElement* e) {
		ifAnyVariableSet.push_back(e);
	}

	void SetIsHidden(bool bHide) {
		bHidden = bHide;
	}

	bool MeetsCriteria() {
		auto CheckIfVariableSet = [this](PacketElement* e) -> bool {
			if (e == this) {
				return VariableIsSet() && CheckVariableEquality();
			}

			if (!e->MeetsCriteria() || !e->VariableIsSet()) {
				return false;
			}

			return true;
		};

		if (ifVariableSet) {
			if (!CheckIfVariableSet(ifVariableSet)) {
				return false;
			}
		}

		if (!ifAnyVariableSet.empty()) {
			bool bSet = false;
			for (auto& itr : ifAnyVariableSet) {
				if (CheckIfVariableSet(itr)) {
					bSet = true;
					break;
				}
			}

			if (!bSet) {
				return false;
			}
		}

		if (!CheckVariableEquality()) {
			return false;
		}

		return true;
	}

	virtual bool VariableIsSet() {
		EmuAssert(("Trying to check if a variable is set for an unhandled type!", false));
		return true;
	}

protected:
	PacketElement() : count(1), name(nullptr), ifVariableSet(nullptr), bHidden(false) {};
	//This is the "size" element in XML structs
	int32_t count;
	//This should be a true const char.. not from a string or anything like that
	const char* name;
	//Check if this variable is "set" (!= 0) before writing
	PacketElement* ifVariableSet;
	//Check if any of these variables are set
	std::vector<PacketElement*> ifAnyVariableSet;
	//If true this element will not display in an xml dump
	bool bHidden;
};