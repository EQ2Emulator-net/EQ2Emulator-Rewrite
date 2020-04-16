#pragma once

#include "../EQ2Packet.h"
#include "../PacketElements/PacketElements.h"
#include "OpcodeManager.h"

class OP_UIEvent_Packet : public EQ2Packet {
public:
	OP_UIEvent_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();


	}

	void HandlePacket(std::shared_ptr<Client> client) override {

	}

	struct Substruct_Byte : public PacketSubstruct {
		Substruct_Byte(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		void RegisterElements() override {
			RegisterUInt8(myByte);
		}

		uint8_t myByte;
	};

	struct Substruct_String : public PacketSubstruct {
		Substruct_String(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		void RegisterElements() override {
			Register16String(myString);
		}

		std::string myString;
	};

	struct Substruct_Int : public PacketSubstruct {
		Substruct_Int(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		void RegisterElements() override {
			RegisterInt32(myInt);
		}

		int32_t myInt;
	};

	struct Substruct_Float : public PacketSubstruct {
		Substruct_Float(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		void RegisterElements() override {
			RegisterFloat(myFloat);
		}

		float myFloat;
	};

	int32_t unknown0;
	int32_t unknown1;
	uint64_t unknown2;
	int16_t unknown3;
	int32_t unknown4;
	uint8_t unknown5;
	std::string uiEvent;
	int32_t byteArraySize;
	std::vector<Substruct_Byte> byteArray;
	int32_t stringArraySize;
	std::vector<Substruct_String> stringArray;
	//There are 2 arrays here of 4 byte values, im assuming one is float one int but not sure the types, never seen them set
	int32_t intArraySize;
	std::vector<Substruct_Int> intArray;
	int32_t floatArraySize;
	std::vector<Substruct_Float> floatArray;

private:
	void RegisterElements() {
		RegisterInt32(unknown0);
		RegisterInt32(unknown1);
		RegisterUInt64(unknown2);
		RegisterInt16(unknown3);
		RegisterInt32(unknown4);
		RegisterUInt8(unknown5);
		Register16String(uiEvent);
		auto asize = RegisterInt32(byteArraySize);
		asize->SetMyArray(RegisterArray(byteArray, Substruct_Byte));
		asize = RegisterInt32(stringArraySize);
		asize->SetMyArray(RegisterArray(stringArray, Substruct_String));
		asize = RegisterInt32(intArraySize);
		asize->SetMyArray(RegisterArray(intArray, Substruct_Int));
		asize = RegisterInt32(floatArraySize);
		asize->SetMyArray(RegisterArray(floatArray, Substruct_Float));
	}
};