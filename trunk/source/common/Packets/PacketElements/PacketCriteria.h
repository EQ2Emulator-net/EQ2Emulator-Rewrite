#pragma once

#include <stdint.h>
#include <cassert>

class PacketElement;

class PacketVariableEquality {
	friend class XmlStructDumper;
public:
	virtual ~PacketVariableEquality() = default;

	void SetIfVariableEquals(int32_t value, PacketElement* e) {
		check_value = value;
		check_element = e;
		equality_type = true;
	}

	void SetIfVariableNotEquals(int32_t value, PacketElement* e) {
		check_value = value;
		check_element = e;
		equality_type = false;
	}

	//Actual equality check (should be called from the other element)
	bool CheckVariableEquality(int32_t value) {
		return (value == check_value) == equality_type;
	}

	//Base handler
	bool CheckVariableEquality();
	
	//This will tell the other element to call our equality check with its value
	virtual bool CheckVariableEquality(PacketVariableEquality*) {
		assert(("Tried to check a packet element's equality on a non-int type!", false));
		return true;
	}

protected:
	PacketVariableEquality() : check_element(nullptr), check_value(0), equality_type(false) {}

private:
	//Note this class could be derived from further for different types, just using int for now
	int32_t check_value;
	PacketElement* check_element;
	bool equality_type;
};

class NetOrderElement {
public:
	virtual ~NetOrderElement() = default;

	void SetIsNetOrder(bool val) {
		bNetOrder = val;
	}

	bool IsNetOrder() {
		return bNetOrder;
	}

protected:
	NetOrderElement() : bNetOrder(false) {}

private:
	bool bNetOrder;
};