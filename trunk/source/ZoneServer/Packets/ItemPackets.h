#pragma once

#include "../Items/ItemTypes.h"
#include "Substruct_ExamineDescItem.h"
#include "OP_EqExamineInfoCmd.h"

class ExamineInfoCmd_Item_Packet : public OP_EqExamineInfoCmd_Packet {
public:
	ExamineInfoCmd_Item_Packet(uint32_t ver, Substruct_ExamineDescItem* item) : OP_EqExamineInfoCmd_Packet(ver, false), itemDesc(item) {
		RegisterElements();
	}

	~ExamineInfoCmd_Item_Packet() = default;

	Substruct_ExamineDescItem* itemDesc;

	void RegisterElements() {
		assert(itemDesc);
		Substruct_ExamineDescItem& itemDesc = *this->itemDesc;
		RegisterSubstruct(itemDesc);
	}
};

class Substruct_ExamineDescItem_Generic : public Substruct_ExamineDescItem {
public:
	Substruct_ExamineDescItem_Generic(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Generic() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_MeleeWeapon : public Substruct_ExamineDescItem, public ItemMeleeWeaponData {
public:
	Substruct_ExamineDescItem_MeleeWeapon(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_MeleeWeapon() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterUInt8(wieldType);
		RegisterOversizedByte(minDmg);
		RegisterOversizedByte(maxDmg);
		RegisterOversizedByte(masteryMinDmg);
		RegisterOversizedByte(masteryMaxDmg);
		RegisterOversizedByte(baseMinDamage);
		RegisterOversizedByte(baseMaxDamage);
		RegisterUInt8(delay);
		RegisterUInt8(damageType);
		RegisterFloat(damageRating);
		if (subVersion >= 53) {
			RegisterUInt32(itemScore);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_RangedWeapon : public Substruct_ExamineDescItem, public ItemRangedWeaponData {
public:
	Substruct_ExamineDescItem_RangedWeapon(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_RangedWeapon() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterOversizedByte(minDmg);
		RegisterOversizedByte(maxDmg);
		RegisterOversizedByte(masteryMinDmg);
		RegisterOversizedByte(masteryMaxDmg);
		RegisterOversizedByte(baseMinDamage);
		RegisterOversizedByte(baseMaxDamage);
		RegisterUInt8(delay);
		RegisterOversizedByte(minRange);
		RegisterOversizedByte(maxRange);
		RegisterUInt8(damageType);
		RegisterFloat(damageRating);
		if (subVersion >= 53) {
			RegisterUInt32(itemScore);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Armor : public Substruct_ExamineDescItem, public ItemArmorData {
public:
	Substruct_ExamineDescItem_Armor(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Armor() = default;

	void PreWrite() override {
		if (subVersion < 90) {
			sMitigationLow = mitigationLow;
			sMitigationHigh = mitigationHigh;
		}
	}

	void RegisterElements() {
		RegisterSubstruct(header);
		if (subVersion < 90) {
			auto& mitigationLow = sMitigationLow;
			auto& mitigationHigh = sMitigationHigh;
			RegisterOversizedByte(mitigationLow);
			RegisterOversizedByte(mitigationHigh);
		}
		else {
			RegisterInt32(mitigationLow);
			RegisterInt32(mitigationHigh);
		}
		RegisterOversizedByte(absorb);
		if (subVersion >= 44) {
			RegisterUInt32(skillID_NEEDS_VERIFY);
		}
		if (subVersion >= 53) {
			RegisterUInt32(itemScore);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Shield : public Substruct_ExamineDescItem, public ItemShieldData {
public:
	Substruct_ExamineDescItem_Shield(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Shield() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterOversizedByte(mitigationLow);
		RegisterOversizedByte(mitigationHigh);
		if (subVersion >= 53) {
			RegisterUInt32(itemScore);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Bag : public Substruct_ExamineDescItem, public ItemBagData {
public:
	Substruct_ExamineDescItem_Bag(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Bag() = default;

	class Substruct_ItemName : public PacketSubstruct {
	public:
		Substruct_ItemName(uint32_t ver = 0) : PacketSubstruct(ver, true) {
			RegisterElements();
		}

		~Substruct_ItemName() = default;

		void RegisterElements() override {
			Register8String(itemName);
		}

		std::string itemName;
	};

	std::vector<Substruct_ItemName> itemNames;

	void PreWrite() override {
		itemNames.clear();
		for (auto& itr : ItemBagData::itemNames) {
			itemNames.emplace_back();
			itemNames.back().itemName = itr;
		}
		PacketSubstruct::PreWrite();
	}

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterUInt8(numSlots);
		RegisterUInt8(numEmptySlots);
		RegisterUInt8(weightReduction);
		Register8String(customName);
		auto e = RegisterOversizedByte(numItems);
		e->SetMyArray(RegisterArray(itemNames, Substruct_ItemName));
		RegisterUInt8(bagUnknown1);
		RegisterUInt8(bagUnknown2);
		RegisterUInt8(bagUnknown3);
		RescopeArrayElement(bagUnknown4);
		RegisterUInt8(bagUnknown4)->SetCount(4);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_RecipeBook : public Substruct_ExamineDescItem, public ItemRecipeBookData {
public:
	Substruct_ExamineDescItem_RecipeBook(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_RecipeBook() = default;

	class Substruct_RecipeBook : public PacketSubstruct, public RecipeBookItem {
	public:
		Substruct_RecipeBook(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
			if (p_itemVersion == 0) {
				itemVersion = GetItemStructVersion(ver);
			}
			else {
				itemVersion = p_itemVersion;
			}
			RegisterElements();
		}

		~Substruct_RecipeBook() = default;

		uint8_t itemVersion;

		void RegisterElements() override {
			if (itemVersion >= 57) {
				RegisterUInt32(recipeID);
				RegisterUInt16(icon);
			}
			Register8String(recipeName);
		}
	};

	std::vector<Substruct_RecipeBook> recipeArray;
	uint16_t recipeCount;

	void PreWrite() override {
		recipeArray.clear();
		for (auto& itr : ItemRecipeBookData::items) {
			recipeArray.emplace_back(GetVersion(), subVersion);
			static_cast<RecipeBookItem&>(recipeArray.back()) = itr;
		}
		PacketSubstruct::PreWrite();
	}

	void RegisterElements() {
		RegisterSubstruct(header);
		auto e = RegisterOversizedByte(recipeCount);
		e->SetMyArray(RegisterArray(recipeArray, Substruct_RecipeBook));
		if (subVersion >= 19) {
			RegisterUInt16(numUses);
		}
		if (subVersion >= 17) {
			RegisterBool(bScribed);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Provision : public Substruct_ExamineDescItem, public ItemProvisionData {
public:
	Substruct_ExamineDescItem_Provision(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Provision() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterUInt8(provisionType);
		RegisterOversizedByte(provisionLevel);
		RegisterFloat(duration);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Bauble : public Substruct_ExamineDescItem, public ItemBaubleData {
public:
	Substruct_ExamineDescItem_Bauble(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Bauble() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterOversizedByte(castTime);
		RegisterOversizedByte(recovery);
		RegisterInt32(duration);
		RegisterFloat(recast);
		RegisterBool(bDisplaySlotOptional);
		if (subVersion >= 14) {
			RegisterBool(bDisplayCastTime);
		}
		RegisterBool(bDisplayBaubleType);
		if (subVersion >= 15) {
			RegisterFloat(effectRadius);
			RegisterInt32(maxAoeTargets);
		}
		if (subVersion >= 31) {
			RegisterBool(bDisplayUntilCancelled);
		}
		if (subVersion >= 53) {
			RegisterUInt32(itemScore);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_HouseItem : public PacketSubstruct, public ItemHouseData {
public:
	Substruct_HouseItem(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver) {
		if (p_itemVersion == 0) {
			itemVersion = GetItemStructVersion(ver);
		}
		else {
			itemVersion = p_itemVersion;
		}
		RegisterElements();
	}

	uint8_t itemVersion;

	~Substruct_HouseItem() = default;

	void RegisterElements() {
		RegisterInt32(rentStatusReduction);
		RegisterFloat(rentCoinReduction);
		if (itemVersion < 38) {
			RegisterUInt8(unknown1);
		}
		RegisterUInt8(houseType);
		if (itemVersion >= 91) {
			RegisterUInt8(unknown2);
		}
	}
};

class Substruct_ExamineDescItem_House : public Substruct_ExamineDescItem {
public:
	Substruct_ExamineDescItem_House(uint32_t ver = 0) : Substruct_ExamineDescItem(ver), houseData(ver, header.itemVersion) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_House() = default;

	Substruct_HouseItem houseData;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterSubstruct(houseData);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Ammo : public Substruct_ExamineDescItem, public ItemAmmoData {
public:
	Substruct_ExamineDescItem_Ammo(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Ammo() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		if (subVersion >= 18) {
			RegisterInt32(range);
			RegisterInt32(damageModifier);
			RegisterFloat(hitBonus);
			RegisterUInt32(damageType);
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_HouseContainer : public Substruct_ExamineDescItem, public ItemHouseContainerData {
public:
	Substruct_ExamineDescItem_HouseContainer(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_HouseContainer() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		RegisterUInt64(allowedTypes);
		RegisterUInt32(unknown1);
		RegisterUInt16(numSlots);
		RegisterUInt8(unknown2);
		RegisterUInt16(brokerCommission);
		RegisterUInt16(fenceCommission);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_Adornment : public Substruct_ExamineDescItem, public ItemAdornmentData {
public:
	Substruct_ExamineDescItem_Adornment(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Adornment() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		if (subVersion >= 49) {
			Register16String(description);
			RegisterFloat(duration);
			if (subVersion >= 54) {
				RegisterUInt8(slotColor);
			}
			if (subVersion >= 86) {
				RegisterUInt64(itemTypes);
			}
			else {
				uint32_t& itemTypes = reinterpret_cast<uint32_t&>(this->itemTypes);
				RegisterUInt32(itemTypes);
			}
			if (subVersion >= 56) {
				RegisterUInt32(unknown2);
			}
			if (subVersion >= 59) {
				Register16String(description2);
				RegisterUInt8(spiritLevel);
				RegisterUInt32(unknown);
			}
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_AchievementProfile : public Substruct_ExamineDescItem, public ItemAchievementProfileData {
public:
	Substruct_ExamineDescItem_AchievementProfile(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_AchievementProfile() = default;

	void RegisterElements() {
		RegisterSubstruct(header);
		//Guessed these
		RegisterUInt32(rentStatusReduction);
		RegisterFloat(rentCoinReduction);
		RegisterUInt8(houseType);
		Register16String(unknownString);
		RegisterUInt8(unknown);
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_RewardVoucher : public Substruct_ExamineDescItem, public ItemRewardVoucherData {
public:
	Substruct_ExamineDescItem_RewardVoucher(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_RewardVoucher() = default;

	class Substruct_RewardVoucherItem : public PacketSubstruct, public RewardVoucherItem {
	public:
		Substruct_RewardVoucherItem(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
			if (p_itemVersion == 0) {
				itemVersion = GetItemStructVersion(ver);
			}
			else {
				itemVersion = p_itemVersion;
			}
			RegisterElements();
		}

		~Substruct_RewardVoucherItem() = default;

		uint8_t itemVersion;

		void RegisterElements() override {
			RegisterUInt32(itemID);
			RegisterUInt32(crc);
			RegisterUInt16(icon);
			if (itemVersion >= 95) {
				RegisterUInt8(unknown);
			}
			Register8String(itemName);
		}
	};

private:
	std::vector<Substruct_RewardVoucherItem> itemsArray;

public:
	void PreWrite() override {
		itemsArray.clear();
		for (auto& itr : items) {
			itemsArray.emplace_back(GetVersion(), subVersion);
			static_cast<RewardVoucherItem&>(itemsArray.back()) = itr;
		}
		PacketSubstruct::PreWrite();
	}

	void RegisterElements() {
		RegisterSubstruct(header);
		if (subVersion >= 34) {
			auto e = RegisterUInt8(itemCount);
			e->SetMyArray(RegisterArray(itemsArray, Substruct_RewardVoucherItem));
			if (subVersion >= 75) {
				RegisterUInt8(unknown1);
				RegisterUInt8(unknown2);
			}
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_RewardCrate : public Substruct_ExamineDescItem, public ItemRewardCrateData {
public:
	Substruct_ExamineDescItem_RewardCrate(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		unknown1 = 0;
		unknown2 = 0;
		RegisterElements();
	}

	~Substruct_ExamineDescItem_RewardCrate() = default;

	class Substruct_RewardCrateItem : public PacketSubstruct, public RewardCrateItem {
	public:
		Substruct_RewardCrateItem(uint32_t ver = 0, uint8_t p_itemVersion = 0) : PacketSubstruct(ver, true) {
			if (p_itemVersion == 0) {
				itemVersion = GetItemStructVersion(ver);
			}
			else {
				itemVersion = p_itemVersion;
			}
			RegisterElements();
		}

		~Substruct_RewardCrateItem() = default;

		uint8_t itemVersion;

		void RegisterElements() override {
			RegisterUInt32(itemID);
			RegisterUInt32(crc);
			RegisterUInt16(icon);
			if (itemVersion >= 40) {
				if (itemVersion >= 42) {
					RegisterUInt32(unknown2);
				}
				else {
					uint8_t& unknown2 = reinterpret_cast<uint8_t&>(this->unknown2);
					RegisterUInt8(unknown2);
				}
			}
			if (itemVersion >= 95) {
				RegisterUInt8(unknown);
			}
			Register16String(itemName);
			RegisterUInt32(unknown3);
		}
	};
	
private:
	std::vector<Substruct_RewardCrateItem> itemsArray;

public:
	void PreWrite() override {
		itemsArray.clear();
		for (auto& itr : items) {
			itemsArray.emplace_back(GetVersion(), subVersion);
			static_cast<RewardCrateItem&>(itemsArray.back()) = itr;
		}
		PacketSubstruct::PreWrite();
	}

	void RegisterElements() {
		RegisterSubstruct(header);
		if (subVersion >= 36) {
			auto e = RegisterUInt8(itemCount);
			e->SetMyArray(RegisterArray(itemsArray, Substruct_RewardCrateItem));
			if (subVersion >= 39) {
				RegisterUInt8(unknown1);
			}
			if (subVersion <= 41) {
				RegisterUInt8(unknown2);
			}
		}
		RegisterSubstruct(footer);
	}
};

class Substruct_ExamineDescItem_ReforgingDecoration : public Substruct_ExamineDescItem, public ItemReforgingDecorationData {
public:
	Substruct_ExamineDescItem_ReforgingDecoration(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_ReforgingDecoration() = default;

	void RegisterElements() {
		Register16String(decoName);
	}
};

class Substruct_ExamineDescItem_Book : public Substruct_ExamineDescItem, public ItemBookData {
public:
	Substruct_ExamineDescItem_Book(uint32_t ver = 0) : Substruct_ExamineDescItem(ver) {
		RegisterElements();
	}

	~Substruct_ExamineDescItem_Book() = default;

	Substruct_HouseItem houseData;

	void PreWrite() override {
		static_cast<ItemHouseData&>(houseData) = this->ItemBookData::houseData;
		PacketSubstruct::PreWrite();
	}

	void RegisterElements() {
		RegisterUInt8(language);
		Register16String(author);
		Register16String(title);
		RegisterSubstruct(houseData);
	}
};