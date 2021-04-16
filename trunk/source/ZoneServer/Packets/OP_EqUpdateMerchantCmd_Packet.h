#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "OP_ClientCmdMsg_Packet.h"

class Substruct_MerchantItem : public PacketSubstruct {
public:
	Substruct_MerchantItem(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	class Substruct_MerchantItemToken : public PacketSubstruct {
	public:
		Substruct_MerchantItemToken(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		uint16_t icon;
		uint8_t unk67650;
		uint16_t quantity;
		uint32_t token_id;
		uint32_t token_id2;
		std::string name;

		void RegisterElements() override {
			RegisterUInt16(icon);
			RegisterUInt8(unk67650);
			RegisterUInt16(quantity);
			RegisterUInt32(token_id);
			RegisterUInt32(token_id2);
			Register16String(name);
		}
	};

	std::string name;
	uint64_t price;
	uint64_t uniqueID;
	uint32_t stack_size;
	uint16_t icon;
	uint8_t unk67650;
	uint8_t name_color;
	uint8_t level;
	uint16_t flags;
	uint8_t quantity;
	uint8_t unknown5;
	uint8_t tier;
	uint32_t status;
	uint32_t itemID;
	uint32_t maxStackSize;
	uint32_t status2;
	uint32_t station_cash;
	uint16_t sorting;
	uint8_t num_tokens;
	std::vector<Substruct_MerchantItemToken> tokens;
	std::string description;

	void RegisterElements() override {
		Register8String(name);
		RegisterUInt64(price);
		//Item id for "Buy" items (not yet unique, for sell this is unique id)
		RegisterUInt64(uniqueID);
		RegisterUInt32(stack_size);
		RegisterUInt16(icon);
		RegisterUInt8(unk67650);
		RegisterUInt8(name_color);
		RegisterUInt8(level);
		RegisterUInt16(flags);
		RegisterUInt8(quantity);
		RegisterUInt8(unknown5);
		RegisterUInt8(tier);
		RegisterUInt32(status);
		RegisterUInt32(itemID);
		RegisterUInt32(maxStackSize);
		RegisterUInt32(status2);
		RegisterUInt32(station_cash);
		RegisterUInt16(sorting);
		auto e = RegisterUInt8(num_tokens);
		e->SetMyArray(RegisterArray(tokens, Substruct_MerchantItemToken));
		Register16String(description);
	}
};

class OP_EqUpdateMerchantCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqUpdateMerchantCmd_Packet(uint32_t ver) : OP_ClientCmdMsg_Packet(ver) {
		RegisterElements();
	}

	class Substruct_MerchantUnkArray1 : public PacketSubstruct {
	public:
		Substruct_MerchantUnkArray1(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		int32_t unk1;
		int32_t unk2;

		void RegisterElements() override {
			RegisterInt32(unk1);
			RegisterInt32(unk2);
		}
	};

	class Substruct_MerchantUnkArray2 : public PacketSubstruct {
	public:
		Substruct_MerchantUnkArray2(uint32_t ver = 0) : PacketSubstruct(ver, true) {

		}

		uint8_t unk1[2];
		int32_t unk2;
		uint8_t unk3[2];
		std::string unk4;
		std::string unk5;

		void RegisterElements() override {
			RescopeArrayElement(unk1);
			RegisterUInt8(unk1)->SetCount(2);
			RegisterInt32(unk2);
			RescopeArrayElement(unk3);
			RegisterUInt8(unk3)->SetCount(2);
			Register16String(unk4);
			Register16String(unk5);
		}
	};

	uint32_t spawn_id;
	uint16_t num_items;
	std::vector<Substruct_MerchantItem> items;
	uint32_t type;
	uint8_t page_index_current;
	uint8_t page_index_last;
	float unkFloats[5];
	uint32_t unkArray1Count;
	std::vector<Substruct_MerchantUnkArray1> unkArray1;
	int32_t unkInts[4];
	uint8_t unknownArray2Count;
	std::vector<Substruct_MerchantUnkArray2> unkArray2;

	void RegisterElements() {
		RegisterUInt32(spawn_id);
		auto e = RegisterUInt16(num_items);
		e->SetMyArray(RegisterArray(items, Substruct_MerchantItem));
		RegisterUInt32(type);
		RegisterUInt8(page_index_current);
		RegisterUInt8(page_index_last);
		RescopeArrayElement(unkFloats);
		RegisterFloat(unkFloats)->SetCount(5);
		auto e2 = RegisterUInt32(unkArray1Count);
		e2->SetMyArray(RegisterArray(unkArray1, Substruct_MerchantUnkArray1));
		RescopeArrayElement(unkInts);
		RegisterInt32(unkInts)->SetCount(4);
		auto e3 = RegisterUInt8(unknownArray2Count);
		e3->SetMyArray(RegisterArray(unkArray2, Substruct_MerchantUnkArray2));
	}
};