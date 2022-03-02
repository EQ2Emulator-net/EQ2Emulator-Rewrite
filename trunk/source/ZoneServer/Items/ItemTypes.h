#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct ItemMeleeWeaponData {
	uint8_t wieldType;
	uint8_t damageType;
	uint8_t delay;
	int16_t minDmg;
	int16_t maxDmg;
	int16_t masteryMinDmg;
	int16_t masteryMaxDmg;
	int16_t baseMinDamage;
	int16_t baseMaxDamage;
	float damageRating;
	uint32_t itemScore;
};

struct ItemRangedWeaponData {
	uint8_t damageType;
	uint8_t delay;
	int16_t minDmg;
	int16_t maxDmg;
	int16_t masteryMinDmg;
	int16_t masteryMaxDmg;
	int16_t baseMinDamage;
	int16_t baseMaxDamage;
	uint16_t minRange;
	uint16_t maxRange;
	float damageRating;
	uint32_t itemScore;
};

struct ItemArmorData {
	int32_t mitigationLow;
	int32_t mitigationHigh;
	int16_t absorb;
	//Guess
	uint32_t skillID_NEEDS_VERIFY;
	uint32_t itemScore;

protected:
	int16_t sMitigationLow;
	int16_t sMitigationHigh;
};

struct ItemShieldData {
	int16_t mitigationLow;
	int16_t mitigationHigh;
	uint32_t itemScore;
};

struct ItemBagData {
	uint16_t numItems;
	uint8_t numSlots;
	uint8_t numEmptySlots;
	uint8_t weightReduction;
	uint8_t bagUnknown1;
	bool bBackpack;
	uint8_t bagUnknown3;
	std::string customName;
	std::vector<std::string> itemNames;
	uint32_t bagUnknown4;
};

struct RecipeBookItem {
	uint16_t icon;
	uint32_t recipeID;
	std::string recipeName;
};

struct ItemRecipeBookData {
	std::vector<RecipeBookItem> items;
	uint16_t numUses;
	bool bScribed;
};

struct ItemProvisionData {
	uint8_t provisionType;
	int16_t provisionLevel;
	float duration;
};

struct ItemBaubleData {
	int16_t castTime;
	int16_t recovery;
	int32_t duration;
	float recast;
	bool bDisplaySlotOptional;
	bool bDisplayCastTime;
	bool bDisplayBaubleType;
	bool bDisplayUntilCancelled;
	float effectRadius;
	int32_t maxAoeTargets;
	uint32_t itemScore;
	uint64_t unknown96b;
	uint64_t unknown96c;
	uint8_t unknown96a;
};

struct ItemHouseData {
	int32_t rentStatusReduction;
	float rentCoinReduction;
	uint8_t houseType;
	//House types
	//0 = any house type
	//1 = house only
	//2 = guild hall only
	//3 = player-made dungeon only
	//4 = NOT player-made dungeon
	//5 = NOT guild hall
	//6 = NOT house
	uint8_t unknown1;
	uint8_t unknown2;
};

struct ItemAmmoData {
	int32_t range;
	int32_t damageModifier;
	float hitBonus;
	uint32_t damageType;
};

struct ItemHouseContainerData {
	uint64_t allowedTypes;
	uint32_t unknown1;
	uint16_t numSlots;
	uint16_t brokerCommission;
	uint16_t fenceCommission;
	uint8_t unknown2;
};

struct ItemAdornmentData {
	std::string description;
	std::string description2;
	std::string unkString99;
	float duration;
	uint8_t slotColor;
	uint8_t spiritLevel;
	uint64_t itemTypes;
	uint32_t unknown2;
	uint32_t unknown;
};

struct ItemAchievementProfileData {
	uint32_t rentStatusReduction;
	float rentCoinReduction;
	uint8_t houseType;
	uint8_t unknown;
	std::string unknownString;
};

struct ItemBookData {
	std::string author;
	std::string title;
	ItemHouseData houseData;
	uint8_t language;
};

struct RewardVoucherItem {
	uint32_t itemID;
	uint32_t crc;
	uint16_t icon;
	uint8_t unknown;
	std::string itemName;
};

struct ItemRewardVoucherData {
	uint8_t itemCount;
	uint8_t unknown1;
	uint8_t unknown2;
	std::vector<RewardVoucherItem> items;
};

struct ItemReforgingDecorationData {
	std::string decoName;
};

struct RewardCrateItem {
	uint32_t itemID;
	uint32_t crc;
	uint32_t stackSize;
	uint32_t colorID;
	uint16_t icon;
	uint8_t language;
	std::string itemName;
};

struct ItemRewardCrateData {
	uint8_t itemCount;
	uint8_t unknown1;
	uint8_t unknown2;
	std::vector<RewardCrateItem> items;
};

struct ItemSpellScrollData {
	bool bScribed;
	bool bScribed_better_version;
	bool bRequire_previous_tier;
	uint8_t unknown78a;
	uint8_t unknown78b;
};