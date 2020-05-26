#pragma once

#include "Substruct_ExamineDescBase.h"

uint8_t GetItemStructVersion(uint32_t version) {
	uint8_t subVersion;

	if (version >= 67650)
		subVersion = 95;
	if (version >= 64707)
		subVersion = 92;
	else if (version >= 63119)
		subVersion = 86;
	else if (version >= 60024)
		subVersion = 81;
	else if (version >= 57107)
		subVersion = 76;
	else if (version >= 57048)
		subVersion = 70;
	else if (version >= 1199)
		subVersion = 68;
	else if (version >= 1195)
		subVersion = 64;
	else if (version >= 1193)
		subVersion = 63;
	else if (version >= 1190)
		subVersion = 62;
	else if (version >= 1188)
		subVersion = 61;
	else if (version >= 1096)
		subVersion = 53;
	else if (version >= 1027)
		subVersion = 49;
	else if (version >= 1008)
		subVersion = 44;
	else if (version >= 927)
		subVersion = 35;
	else if (version >= 893)
		subVersion = 34;
	else if (version >= 860)
		subVersion = 32;
	else
		subVersion = 28;

	return subVersion;
}

class Substruct_ItemStatMod : public PacketSubstruct {
public:
	Substruct_ItemStatMod(uint32_t ver = 0) : PacketSubstruct(ver) {
		RegisterElements();
	}

	uint8_t statType;
	int16_t statSubtype;
	std::string stringVal;

	union {
		float fValue;
		int16_t sValue_do_not_set;
		int32_t iValue;
	};

	uint8_t statLevel;
	uint8_t unknown64;
	uint8_t unknown92;
	int32_t unknown83;
	int16_t unknown89;

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		//itemVersion <= 76
		if (version < 60024) {
			int32_t v = iValue;
			sValue_do_not_set = v;
			PacketSubstruct::WriteElement(outbuf, offset);
			iValue = v;
		}
		else {
			PacketSubstruct::WriteElement(outbuf, offset);
		}
	}

	void RegisterElements() {
		auto e = RegisterUInt8(statType);
		RegisterOversizedByte(statSubtype);
		//This is really if (statType & ~0xF) == 6 or 8, but our DB stats only go up to 7 so no issue
		//NOTE the 0x80, 0x40, 0x20, 0x10 bits of the type set different bools
		RegisterFloat(fValue)->SetIfVariableEquals(6, e);
		uint8_t itemVersion = GetItemStructVersion(version);
		if (itemVersion <= 76) {
			int16_t& sValue = sValue_do_not_set;
			RegisterOversizedByte(sValue);
		}
		else {
			RegisterInt32(iValue);
		}
		Register8String(stringVal);
		RegisterUInt8(statLevel);
		if (itemVersion >= 64) {
			RegisterUInt8(unknown64);
		}
		if (itemVersion >= 92) {
			RegisterUInt8(unknown92);
		}
		if (itemVersion >= 83) {
			RegisterInt32(unknown83);
		}
		if (itemVersion >= 89) {
			RegisterInt16(unknown89);
		}
	}
};

class Substruct_ExamineDescItem : public Substruct_ExamineDescBase {
public:
	Substruct_ExamineDescItem(uint32_t ver = 0) : Substruct_ExamineDescBase(ver) {
		SetItemStructVersion();
		RegisterElements();
	}

	~Substruct_ExamineDescItem() = default;

	void SetItemStructVersion() {
		examineType = 0xFE;
		examineTypeOverride = 0x00;
		subVersion = GetItemStructVersion(version);
	}

	bool bHasCreator;
	std::string creatorName;
	uint32_t uniqueID;
	uint64_t brokerID;
	int32_t itemID;
	int32_t itemCRC;
	uint16_t icon;
	uint8_t tier;
	uint64_t flags;
	int16_t unknownOversized1;
	int16_t unknownOVersized2;
	uint8_t unknownBytes5[5];
	uint8_t unknownBytes10[10];
	uint8_t statModCount;

	void RegisterElements() {
		RegisterBool(bHasCreator);
		Register8String(creatorName);
		auto e = RegisterUInt32(uniqueID);
		RegisterUInt64(brokerID)->SetIfVariableNotEquals(0xFFFFFFFF, e);
		if (subVersion >= 20) {
			RegisterInt32(itemID);
		}
		if (subVersion >= 52) {
			RegisterInt32(itemCRC);
		}
		RegisterUInt16(icon);
		RegisterUInt8(tier);
		if (subVersion >= 84) {
			RegisterUInt64(flags);
		}
		else {
			uint32_t& flags = reinterpret_cast<uint32_t&>(this->flags);
			RegisterUInt32(flags);
		}
		RegisterOversizedByte(unknownOversized1);
		RegisterOversizedByte(unknownOVersized2);
		RescopeArrayElement(unknownBytes5);
		RegisterUInt8(unknownBytes5)->SetCount(5);
		RescopeArrayElement(unknownBytes10);
		RegisterUInt8(unknownBytes10)->SetCount(10);
		RegisterUInt8(statModCount);

	}
};