#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "ItemTypes.h"

struct ItemStatMod {
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
};

struct ItemStringMod {
	std::string stringVal;
	std::string statDescription;
	uint8_t unknown1;
	uint8_t unknown2;
	uint8_t unknown3;
	uint8_t unknown4;
	uint8_t unknown5;
};

struct ItemArrayUnknown {
	uint8_t unknown1;
	int16_t unknown2;
	uint8_t unknown3[4];
	uint8_t unknown4[4];
};

struct ItemClassReq {
	uint8_t adventureClass;
	uint8_t tradeskillClass;
	uint16_t level;
};

struct ItemDescBaseData {
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
	uint8_t unknownArrayCount;
	std::vector<ItemStatMod> statMods;
	std::vector<ItemArrayUnknown> unknownArray;
	std::vector<ItemStringMod> stringMods;
	uint8_t stringModCount;
	uint32_t unknown19;
	uint8_t condition;
	uint32_t skillReq1;
	uint32_t skillReq2;
	union {
		int32_t weight;
		int16_t sWeight_do_not_set;
	};
	int16_t skillMin;
	uint8_t classReqCount;
	uint8_t slotCount;
	std::vector<ItemClassReq> classReqs;
	std::vector<uint8_t> slots;
	uint32_t itemTypeUnknown;
	uint8_t itemType;
};

struct ItemEffect {
	//tabIndex is encoded, 0x80 is a separate bool from the first 7 bits
	uint8_t tabIndex;
	uint8_t percentage;
	std::string effectText;
};

//Not sure what this is yet just going on the name from old structs
struct ItemTierLineEntry {
	std::string addon;
	int32_t unknown1;
	uint8_t unknown2;
	uint8_t unknown3;
	uint8_t unknown4;
	uint8_t unknown5;
};

struct ItemFooterArrayUnknown {
	int32_t unknown1;
	std::string unknown2;
	uint8_t unknown3;
	uint8_t unknown4;
	uint8_t unknown5;
};

struct ItemSetBonusEffect {
	uint8_t tabIndex;
	uint8_t percentage;
	std::string effectText;
};

struct ItemSetBonus {
	uint8_t itemsNeeded;
	uint8_t statCount;
	uint8_t effectCount;
	std::vector<ItemStatMod> stats;
	std::vector<ItemSetBonusEffect> effects;
};

struct ItemSetItem {
	uint8_t unknown1;
	uint8_t unknown2;
	std::string itemName;
};

struct ItemSetDetails {
	uint8_t setBonusCount;
	uint8_t setItemsCount;
	uint8_t numSetItemsEquipped;
	uint8_t numItemsInSet;
	std::vector<ItemFooterArrayUnknown> footerUnknownArray;
	std::vector<ItemSetBonus> setBonuses;
	std::vector<ItemSetItem> setItems;
};

struct ItemAdornmentLevel {
	uint32_t statCount;
	std::vector<ItemStatMod> stats;
};

struct ItemAdornmentDetails {
	uint8_t unknown1;
	uint8_t unknown2;
	uint8_t statsActive;
	uint8_t statsTotal;
	std::string adornName;
	uint32_t adornLevelCount;
	std::vector<ItemAdornmentLevel> adornLevels;
};

struct ItemAdornmentSlotDetails {
	uint8_t slot;
	uint8_t unknown1;
	uint16_t unknown2; //icon maybe?
	uint32_t itemID;
	std::string itemName;
	float timeLeft;
	uint32_t unknown3;
};

struct ItemDescFooterData {
	uint8_t numEffects;
	uint8_t numTierLine;
	uint8_t recommendedLevel;
	uint32_t requiredLevel;
	std::vector<ItemEffect> effects;
	std::vector<ItemTierLineEntry> tierLine;
	uint64_t requiredClasses;
	uint64_t requiredClasses2;
	bool adornSlots[11];
	uint16_t stackSize;
	uint8_t footerUnknownArrayCount;
	std::vector<ItemArrayUnknown> footerUnknownArray;
	std::string setName;
	ItemSetDetails setDetails;
	uint8_t adornCount;
	std::vector<ItemAdornmentDetails> adornments;
	uint8_t unknown1;
	bool bCollectable;
	bool bCollectionNeeded;
	uint8_t unknown60055a;
	uint8_t unknown60055b;
	std::string offersQuest;
	std::string requiredByQuest;
	uint8_t offersQuestColor;
	uint8_t requiredByQuestColor;
	bool bHasCharges;
	uint16_t chargesMax;
	uint16_t chargesRemaining;
	uint64_t unknown26Flags;
	uint8_t unknown27;
	uint8_t unknown32;
	uint8_t adornSlotDetailsCount;
	std::vector<ItemAdornmentSlotDetails> adornSlotDetails;
	bool bLockedFlag;
	uint8_t unknown61;
	uint8_t unknown76;
	uint8_t unknown79;
	bool bAccountRestricted;
	uint8_t unknown64a;
	uint32_t unknown64b;
	uint32_t COEAppearanceIDUnknown;
	uint8_t unknown67;
	std::string unknown68;
	std::string unknown71;
	uint8_t unknown87;
	std::string unknown88String;
	std::string unknown93String;
	uint8_t unknown94;
	std::string itemName;
	std::string description;
	uint8_t footer_unknown_7;

protected:
	//For some versions this was a bitmask, later changed to just array of bool
	uint32_t adornSlotBitmask;
};

class Item : public ItemDescBaseData, public ItemDescFooterData {
protected:
	Item() = default;

public:
	virtual ~Item() = default;
};

class ItemGeneric : public Item {
public:
	ItemGeneric() = default;
	~ItemGeneric() = default;
};

class ItemMeleeWeapon : public Item, public ItemMeleeWeaponData {
public:

};