#pragma once

#include "Substruct_ExamineDescBase.h"
#include "../Items/Item.h"

inline uint8_t GetItemStructVersion(uint32_t version) {
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

class Substruct_ItemStatMod : public PacketSubstruct, public ItemStatMod {
public:
	Substruct_ItemStatMod(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		//itemVersion <= 76
		if (version < 60024 && statType != 6) {
			int32_t v = iValue;
			sValue_do_not_set = v;
			PacketSubstruct::WriteElement(outbuf, offset);
			iValue = v;
		}
		else {
			PacketSubstruct::WriteElement(outbuf, offset);
		}
	}

	uint8_t itemVersion;

	void RegisterElements() {
		auto e = RegisterUInt8(statType);
		RegisterOversizedByte(statSubtype);
		//This is really if (statType & ~0xF) == 6 or 8, but our DB stats only go up to 7 so no issue
		//NOTE the 0x80, 0x40, 0x20, 0x10 bits of the type set different bools
		RegisterFloat(fValue)->SetIfVariableEquals(6, e);
		if (itemVersion <= 76) {
			int16_t& sValue = sValue_do_not_set;
			RegisterOversizedByte(sValue)->SetIfVariableNotEquals(6, e);
		}
		else {
			RegisterInt32(iValue)->SetIfVariableNotEquals(6, e);
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

class Substruct_ItemUnknown : public PacketSubstruct, public ItemArrayUnknown {
public:
	Substruct_ItemUnknown(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	

	void RegisterElements() {
		RegisterUInt8(unknown1);
		RegisterOversizedByte(unknown2);
		RescopeArrayElement(unknown3);
		RegisterUInt8(unknown3)->SetCount(4);
		RescopeArrayElement(unknown4);
		RegisterUInt8(unknown4)->SetCount(4);
	}
};

class Substruct_ItemStringMod : public PacketSubstruct, public ItemStringMod {
public:
	Substruct_ItemStringMod(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	uint8_t itemVersion;

	void RegisterElements() {
		uint8_t itemVersion = GetItemStructVersion(GetVersion());
		Register8String(stringVal);
		if (itemVersion >= 7) {
			RegisterUInt8(unknown1);
		}
		if (itemVersion >= 28) {
			RegisterUInt8(unknown2);
		}
		//if Substruct_ExamineDescItem::unknown1 NOT SET
		Register16String(statDescription);
		if (itemVersion >= 89) {
			RegisterUInt8(unknown3);
			RegisterUInt8(unknown4);
			RegisterUInt8(unknown5);
		}
	}
};

class Substruct_ItemClassReq : public PacketSubstruct, public ItemClassReq {
public:
	Substruct_ItemClassReq(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt8(adventureClass);
		RegisterUInt8(tradeskillClass);
		RegisterUInt16(level);
	}
};

class Substruct_ItemSlot : public PacketSubstruct {
public:
	Substruct_ItemSlot(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	uint8_t slotID;

	void RegisterElements() {
		RegisterUInt8(slotID);
	}
};

class Substruct_ItemDescHeader : public PacketSubstruct, public ItemDescBaseData {
public:
	Substruct_ItemDescHeader(uint32_t ver = 0) : PacketSubstruct(ver) {
		itemVersion = GetItemStructVersion(ver);
	}

	~Substruct_ItemDescHeader() = default;

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		classReqArray.clear();
		for (auto& itr : ItemDescBaseData::classReqs) {
			classReqArray.emplace_back(GetVersion());
			static_cast<ItemClassReq&>(classReqArray.back()) = itr;
		}

		slotArray.clear();
		for (uint32_t i = 0; i < 31; i++) {
			if (slotBitmask & (1 << i)) {
				slotArray.emplace_back(GetVersion());
				slotArray.back().slotID = static_cast<uint8_t>(i);
			}
		}

		stringModArray.clear();
		for (auto& itr : ItemDescBaseData::stringMods) {
			stringModArray.emplace_back(GetVersion());
			static_cast<ItemStringMod&>(stringModArray.back()) = itr;
		}

		unknownArray.clear();
		for (auto& itr : ItemDescBaseData::unknownArray) {
			unknownArray.emplace_back(GetVersion());
			static_cast<ItemArrayUnknown&>(unknownArray.back()) = itr;
		}

		statModArray.clear();
		for (auto& itr : ItemDescBaseData::statMods) {
			statModArray.emplace_back(GetVersion());
			static_cast<ItemStatMod&>(statModArray.back()) = itr;
		}

		if (itemVersion < 11) {
			int32_t w = weight;
			sWeight_do_not_set = weight;
			PacketSubstruct::WriteElement(outbuf, offset);
			weight = w;
		}
		else {
			PacketSubstruct::WriteElement(outbuf, offset);
		}
	}

	uint8_t itemVersion;
	std::vector<Substruct_ItemStatMod> statModArray;
	std::vector<Substruct_ItemUnknown> unknownArray;
	std::vector<Substruct_ItemStringMod> stringModArray;
	std::vector<Substruct_ItemClassReq> classReqArray;
	std::vector<Substruct_ItemSlot> slotArray;

	void RegisterHeader() {
		RegisterBool(bHasCreator);
		Register8String(creatorName);
		auto e = RegisterUInt32(uniqueID);
		RegisterUInt64(brokerID)->SetIfVariableNotEquals(0xFFFFFFFF, e);
		if (itemVersion >= 20) {
			RegisterInt32(itemID);
		}
		if (itemVersion >= 52) {
			RegisterInt32(itemCRC);
		}
		RegisterUInt16(icon);
		RegisterUInt8(tier);
		if (itemVersion >= 84) {
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
		auto modCount = RegisterUInt8(statModCount);
		modCount->SetMyArray(RegisterArray(statModArray, Substruct_ItemStatMod));
		if (itemVersion >= 44) {
			auto unkCount = RegisterUInt8(unknownArrayCount);
			unkCount->SetMyArray(RegisterArray(unknownArray, Substruct_ItemUnknown));
		}
		auto unkCount = RegisterUInt8(stringModCount);
		unkCount->SetMyArray(RegisterArray(stringModArray, Substruct_ItemStringMod));
		RegisterUInt32(unknown19);
		RegisterUInt8(condition);
		if (itemVersion >= 11) {
			RegisterInt32(weight);
		}
		else {
			int16_t& weight = sWeight_do_not_set;
			RegisterOversizedByte(weight);
		}
		RegisterUInt32(skillReq1);
		RegisterUInt32(skillReq2);
		RegisterOversizedByte(skillMin);
		auto classCount = RegisterUInt8(classReqCount);
		classCount->SetMyArray(RegisterArray(classReqArray, Substruct_ItemClassReq));
		auto nSlots = RegisterUInt8(slotCount);
		nSlots->SetMyArray(RegisterArray(slotArray, Substruct_ItemSlot));
		RegisterUInt32(itemTypeUnknown);
		RegisterUInt8(itemType);
	}
};

class Substruct_ItemEffect : public PacketSubstruct, public ItemEffect {
public:
	Substruct_ItemEffect(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt8(tabIndex);
		Register16String(effectText);
		RegisterUInt8(percentage);
	}
};

class Substruct_ItemTierLineEntry : public PacketSubstruct, public ItemTierLineEntry {
public:
	Substruct_ItemTierLineEntry(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	uint8_t itemVersion;

	void RegisterElements() {
		Register16String(addon);
		RegisterInt32(unknown1);
		RegisterUInt8(unknown2);
		RegisterUInt8(unknown3);
		if (itemVersion >= 89) {
			RegisterUInt8(unknown4);
			RegisterUInt8(unknown5);
		}
	}
};

class Substruct_FooterArrayUnknown : public PacketSubstruct, public ItemFooterArrayUnknown {
public:
	Substruct_FooterArrayUnknown(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterInt32(unknown1);
		Register8String(unknown2);
		RegisterUInt8(unknown3);
		RegisterUInt8(unknown4);
		RegisterUInt8(unknown5);
	}
};

class Substruct_ItemSetBonusEffect : public PacketSubstruct, public ItemSetBonusEffect {
public:
	Substruct_ItemSetBonusEffect(uint32_t ver = 0) : PacketSubstruct(ver, true) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt8(tabIndex);
		Register16String(effectText);
		RegisterUInt8(percentage);
	}
};

class Substruct_ItemSetBonus : public PacketSubstruct, public ItemSetBonus {
public:
	Substruct_ItemSetBonus(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		statArray.clear();
		for (auto& itr : stats) {
			statArray.emplace_back(GetVersion(), itemVersion);
			static_cast<ItemStatMod&>(statArray.back()) = itr;
		}
		effectArray.clear();
		for (auto& itr : effects) {
			effectArray.emplace_back(GetVersion());
			static_cast<ItemSetBonusEffect&>(effectArray.back()) = itr;
		}
		PacketSubstruct::WriteElement(outbuf, offset);
	}

	uint8_t itemVersion;
	std::vector<Substruct_ItemStatMod> statArray;
	std::vector<Substruct_ItemSetBonusEffect> effectArray;

	void RegisterElements() {
		RegisterUInt8(itemsNeeded);
		auto e = RegisterUInt8(statCount);
		e->SetMyArray(RegisterArray(statArray, Substruct_ItemStatMod));
		e = RegisterUInt8(effectCount);
		e->SetMyArray(RegisterArray(effectArray, Substruct_ItemSetBonusEffect));
	}
};

class Substruct_ItemSetItem : public PacketSubstruct, public ItemSetItem {
public:
	Substruct_ItemSetItem(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	uint8_t itemVersion;

	void RegisterElements() {
		RegisterUInt8(unknown1);
		if (itemVersion >= 16) {
			RegisterUInt8(unknown2);
		}
		Register16String(itemName);
	}
};

class Substruct_ItemSetDetails : public PacketSubstruct, public ItemSetDetails {
public:
	Substruct_ItemSetDetails(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		setBonusArray.clear();
		for (auto& itr : setBonuses) {
			setBonusArray.emplace_back(GetVersion(), itemVersion);
			static_cast<ItemSetBonus&>(setBonusArray.back()) = itr;
		}
		setItemArray.clear();
		for (auto& itr : setItems) {
			setItemArray.emplace_back(GetVersion(), itemVersion);
			static_cast<ItemSetItem&>(setItemArray.back()) = itr;
		}
		PacketSubstruct::WriteElement(outbuf, offset);
	}

	uint8_t itemVersion;
	std::vector<Substruct_ItemSetBonus> setBonusArray;
	std::vector<Substruct_ItemSetItem> setItemArray;

	void RegisterElements() {
		auto e = RegisterUInt8(setBonusCount);
		e->SetMyArray(RegisterArray(setBonusArray, Substruct_ItemSetBonus));
		e = RegisterUInt8(numItemsInSet);
		e->SetMyArray(RegisterArray(setItemArray, Substruct_ItemSetItem));
		RegisterUInt8(numSetItemsEquipped);
		RegisterUInt8(numItemsInSet);
	}
};

class Substruct_ItemAdornmentLevel : public PacketSubstruct, public ItemAdornmentLevel {
public:
	Substruct_ItemAdornmentLevel(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		statArray.clear();
		for (auto& itr : stats) {
			statArray.emplace_back(GetVersion(), itemVersion);
			static_cast<ItemStatMod&>(statArray.back()) = itr;
		}
		PacketSubstruct::WriteElement(outbuf, offset);
	}

	uint8_t itemVersion;
	std::vector<Substruct_ItemStatMod> statArray;

	void RegisterElements() {
		auto e = RegisterUInt32(statCount);
		e->SetMyArray(RegisterArray(statArray, Substruct_ItemStatMod));
	}
};

class Substruct_ItemAdornmentDetails : public PacketSubstruct, public ItemAdornmentDetails {
public:
	Substruct_ItemAdornmentDetails(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		levelArray.clear();
		for (auto& itr : adornLevels) {
			levelArray.emplace_back(GetVersion(), itemVersion);
			static_cast<ItemAdornmentLevel&>(levelArray.back()) = itr;
		}
		PacketSubstruct::WriteElement(outbuf, offset);
	}

	uint8_t itemVersion;
	std::vector<Substruct_ItemAdornmentLevel> levelArray;

	void RegisterElements() {
		RegisterUInt8(unknown1);
		RegisterUInt8(unknown2);
		if (itemVersion >= 65) {
			RegisterUInt8(statsActive);
		}
		if (itemVersion >= 74) {
			RegisterUInt8(statsTotal);
		}
		Register16String(adornName);
		auto e = RegisterUInt32(adornLevelCount);
		e->SetMyArray(RegisterArray(levelArray, Substruct_ItemAdornmentLevel));
	}
};

class Substruct_ItemAdornmentSlotDetails : public PacketSubstruct, public ItemAdornmentSlotDetails {
public:
	Substruct_ItemAdornmentSlotDetails(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	uint8_t itemVersion;

	void RegisterElements() {
		RegisterUInt8(slot);
		if (itemVersion >= 54) {
			RegisterUInt8(unknown1);
			RegisterUInt16(unknown2);
		}
		RegisterUInt32(itemID);
		Register16String(itemName);
		RegisterFloat(timeLeft);
		if (itemVersion >= 63) {
			RegisterUInt32(unknown3);
		}
	}
};

class Substruct_ItemDescFooter : public Substruct_ExamineDescBase, public ItemDescFooterData {
public:
	Substruct_ItemDescFooter(uint32_t ver = 0, uint8_t p_itemVersion = 0) : Substruct_ExamineDescBase(ver) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
	}

	~Substruct_ItemDescFooter() = default;

	std::vector<Substruct_ItemEffect> effectsArray;
	std::vector<Substruct_ItemTierLineEntry> tierLineArray;
	std::vector<Substruct_FooterArrayUnknown> footerUnknownArray;
	std::vector<Substruct_ItemAdornmentDetails> footerAdornmentArray;
	std::vector<Substruct_ItemAdornmentSlotDetails> adornSlotDetailsArray;

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		if (itemVersion < 54 && itemVersion >= 47) {
			//For select client versions adorn slots were a bitmask
			adornSlotBitmask = 0;
			for (uint32_t i = 0; i < 11; i++) {
				if (adornSlots[i] != 0xFF) {
					adornSlotBitmask |= (1 << i);
				}
			}
		}

		effectsArray.clear();
		for (auto& itr : effects) {
			effectsArray.emplace_back();
			static_cast<ItemEffect&>(effectsArray.back()) = itr;
		}

		if (itemVersion >= 82) {
			tierLineArray.clear();
			for (auto& itr : tierLine) {
				tierLineArray.emplace_back(GetVersion(), itemVersion);
				static_cast<ItemTierLineEntry&>(tierLineArray.back()) = itr;
			}
		}

		footerUnknownArray.clear();
		for (auto& itr : footerUnknownArray) {
			footerUnknownArray.emplace_back();
			static_cast<ItemFooterArrayUnknown&>(footerUnknownArray.back()) = itr;
		}

		footerAdornmentArray.clear();
		for (auto& itr : adornments) {
			footerAdornmentArray.emplace_back();
			static_cast<ItemAdornmentDetails&>(footerAdornmentArray.back()) = itr;
		}

		adornSlotDetailsArray.clear();
		for (auto& itr : adornSlotDetails) {
			adornSlotDetailsArray.emplace_back();
			static_cast<ItemAdornmentSlotDetails&>(adornSlotDetailsArray.back()) = itr;
		}
		
		static_cast<ItemSetDetails&>(itemSetDetails) = setDetails;

		PacketSubstruct::WriteElement(outbuf, offset);
	}

	uint8_t itemVersion;
	Substruct_ItemSetDetails itemSetDetails;

	void RegisterElements() {
		{
			//This is an i var set ExamineInfoCmd unknown[1] but not sure when that is ever set
			auto e = RegisterUInt8(numEffects);
			e->SetMyArray(RegisterArray(effectsArray, Substruct_ItemEffect));
		}
		if (itemVersion >= 82) {
			auto e = RegisterUInt8(numTierLine);
			e->SetMyArray(RegisterArray(tierLineArray, Substruct_ItemTierLineEntry));
		}
		RegisterUInt64(requiredClasses);
		if (itemVersion >= 70) {
			RegisterUInt64(requiredClasses2);
		}
		if (itemVersion < 26) {
			RegisterUInt32(requiredLevel);
		}
		else {
			RegisterUInt8(recommendedLevel);
			uint8_t& requiredLevel = reinterpret_cast<uint8_t&>(this->requiredLevel);
			RegisterUInt8(requiredLevel);
		}
		if (itemVersion >= 33) {
			RegisterUInt16(stackSize);
		}
		if (itemVersion < 54) {
			if (itemVersion >= 47) {
				RegisterUInt32(adornSlotBitmask);
			}
		}
		else {
			RescopeArrayElement(adornSlots);
			auto e = RegisterUInt8(adornSlots);
			e->SetCount(11);
		}
		{
			auto e = RegisterUInt8(footerUnknownArrayCount);
			e->SetMyArray(RegisterArray(footerUnknownArray, Substruct_FooterArrayUnknown));
		}
		{
			auto e = Register8String(setName);
			RegisterSubstruct(itemSetDetails)->SetIsVariableSet(e);
		}
		{
			auto e = RegisterUInt8(adornCount);
			e->SetMyArray(RegisterArray(footerAdornmentArray, Substruct_ItemAdornmentDetails));
		}
		if (itemVersion >= 35) {
			auto e1 = RegisterUInt8(unknown1);
			auto e2 = RegisterBool(bCollectable);
			auto e3 = RegisterBool(bCollectionNeeded);
			e3->AddIfAnyVariableSet(e1);
			e3->AddIfAnyVariableSet(e2);
		}
		else if (itemVersion >= 20) {
			auto e1 = RegisterBool(bCollectable);
			auto e2 = RegisterBool(bCollectionNeeded);
			e2->SetIsVariableSet(e1);
		}
		if (itemVersion >= 80) {
			auto e = RegisterUInt8(unknown60055a);
			RegisterUInt8(unknown60055b)->SetIsVariableSet(e);
		}
		if (itemVersion >= 21) {
			auto e1 = Register8String(offersQuest);
			auto e2 = Register8String(requiredByQuest);
			if (itemVersion >= 24) {
				RegisterUInt8(offersQuestColor)->SetIsVariableSet(e1);
				RegisterUInt8(requiredByQuestColor)->SetIsVariableSet(e2);
			}
		}
		if (itemVersion >= 25) {
			auto e = RegisterBool(bHasCharges);
			RegisterUInt16(chargesMax)->SetIsVariableSet(e);
			RegisterUInt16(chargesRemaining)->SetIsVariableSet(e);
		}
		if (itemVersion >= 26) {
			RegisterUInt64(unknown26Flags);
		}
		if (itemVersion >= 27) {
			RegisterUInt8(unknown27);
		}
		if (itemVersion >= 32) {
			RegisterUInt8(unknown32);
		}
		if (itemVersion >= 47) {
			auto e = RegisterUInt8(adornSlotDetailsCount);
			e->SetMyArray(RegisterArray(adornSlotDetailsArray, Substruct_ItemAdornmentSlotDetails));
		}
		if (itemVersion >= 51) {
			RegisterBool(bLockedFlag);
		}
		if (itemVersion >= 61) {
			RegisterUInt8(unknown61);
		}
		if (itemVersion >= 76) {
			RegisterUInt8(unknown76);
		}
		if (itemVersion >= 79) {
			RegisterUInt8(unknown79);
		}
		if (itemVersion >= 61) {
			RegisterBool(bAccountRestricted);
		}
		if (itemVersion >= 64) {
			RegisterUInt8(unknown64a);
			RegisterUInt32(unknown64b);
		}
		if (itemVersion >= 66) {
			if (itemVersion > 72) {
				RegisterUInt32(COEAppearanceIDUnknown);
			}
			else {
				uint16_t& COEAppearanceIDUnknown = reinterpret_cast<uint16_t&>(this->COEAppearanceIDUnknown);
				RegisterUInt16(COEAppearanceIDUnknown);
			}
		}
		if (itemVersion >= 67) {
			RegisterUInt8(unknown67);
		}
		if (itemVersion >= 68) {
			Register16String(unknown68);
		}
		if (itemVersion >= 71) {
			Register16String(unknown71);
		}
		if (itemVersion >= 87) {
			RegisterUInt8(unknown87);
		}
		if (itemVersion >= 88) {
			Register16String(unknown88String);
		}
		if (itemVersion >= 93) {
			Register16String(unknown93String);
		}
		if (itemVersion >= 94) {
			RegisterUInt8(unknown94);
		}
		Register8String(itemName);
		Register16String(description);
		RegisterUInt8(footer_unknown_7);
	}
};

class Substruct_ExamineDescItem : public Substruct_ExamineDescBase {
public:
	Substruct_ExamineDescItem(uint32_t ver = 0) : Substruct_ExamineDescBase(ver), header(ver), footer(ver, header.itemVersion) {
		SetItemStructVersion();
	}

	~Substruct_ExamineDescItem() = default;

	void SetItemStructVersion() {
		examineType = 0xFE;
		examineTypeOverride = 0x00;
		subVersion = header.itemVersion;
	}

	Substruct_ItemDescHeader header;
	Substruct_ItemDescFooter footer;
};