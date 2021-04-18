#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "ItemTypes.h"
#include "../../common/Packets/PacketElements/PacketEQ2EquipmentItem.h"

class Substruct_ExamineDescItem;
class Client;
class DatabaseResult;

enum class EItemSlot : uint32_t {
	PRIMARY = 0,
	SECONDARY = 1,
	HEAD = 2,
	CHEST = 3,
	SHOULDERS = 4,
	FOREARMS = 5,
	HANDS = 6,
	LEGS = 7,
	FEET = 8,
	RING1 = 9,
	RING2 = 10,
	EAR1 = 11,
	EAR2 = 12,
	NECK = 13,
	WRIST1 = 14,
	WRIST2 = 15,
	RANGE = 16,
	AMMO = 17,
	WAIST = 18,
	CLOAK = 19,
	CHARM1 = 20,
	CHARM2 = 21,
	FOOD = 22,
	DRINK = 23,
	TEXTURE = 24
};

struct ItemStatMod {
	uint8_t statType;
	int16_t statSubtype;
	std::string stringVal;

	union {
		float fValue;
		int32_t iValue;
	};

	uint8_t statLevel;
	uint8_t unknown64;
	uint8_t unknown92;
	float statAsFloat; // not sure what this is used for yet
	int16_t stringStatUnknown;
};

struct ItemStringMod {
	ItemStringMod() : unknown2(0) {}
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
	//Primitives
	bool bHasCreator;

	int32_t uniqueID;
	uint64_t brokerID;
	uint32_t itemID;
	uint32_t itemCRC;
	uint16_t icon;
	uint8_t tier;
	uint64_t flags;
	int16_t unknownOversized1;
	int16_t unknownOversized2;
	uint8_t unknownBytes5[5];
	uint8_t unknownBytes10[10];
	uint8_t statModCount;
	uint8_t unknownArrayCount;

	uint8_t stringModCount;
	uint32_t unknown19;
	uint8_t condition;
	uint32_t skillReq1;
	uint32_t skillReq2;
	int32_t weight;
	int16_t skillMin;
	uint8_t classReqCount;
	uint8_t slotCount;
	uint32_t slotBitmask;
	int32_t nameColor;
	uint32_t set_id;
	uint8_t itemType;

	//Dynamic
protected:
	uint8_t ItemDesc_START_DYNAMIC_DATA;
public:
	std::vector<ItemClassReq> classReqs;
	std::string creatorName;
	std::vector<ItemStatMod> statMods;
	std::vector<ItemArrayUnknown> unknownArray;
	std::vector<ItemStringMod> stringMods;
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
	uint32_t bonusID;
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
	//Primitives
	uint8_t numEffects;
	uint8_t numTierLine;
	uint8_t recommendedLevel;
	uint32_t requiredLevel;
	uint64_t requiredClasses;
	uint64_t requiredClasses2;
	uint8_t adornSlots[11];
	uint16_t stackSize;
	uint8_t footerUnknownArrayCount;
	uint8_t adornCount;
	bool bQuestExamine;
	bool bCollectable;
	bool bExamineNeeded;
	uint8_t unknown60055a;
	uint8_t unknown60055b;
	uint8_t offersQuestColor;
	uint8_t requiredByQuestColor;
	bool bHasCharges;
	uint16_t chargesMax;
	uint16_t chargesRemaining;
	uint64_t sell_status_amount;
	bool bHarvestedMaterial;
	bool bTransmutedMaterial;
	uint8_t adornSlotDetailsCount;
	bool bLockedFlag;
	uint8_t unknown61;
	uint8_t unknown76;
	uint8_t unknown79;
	bool bAccountRestricted;
	uint8_t unknown64a;
	uint32_t unknown64b;
	uint32_t COEAppearanceIDUnknown;
	uint8_t unknown67;
	uint8_t unknown87;
	uint8_t unknown94;
	uint8_t footer_unknown_7;

	//Dynamic
protected:
	uint8_t ItemFooter_START_DYNAMIC_DATA;
public:
	ItemSetDetails setDetails;
	std::vector<ItemAdornmentSlotDetails> adornSlotDetails;
	std::vector<ItemAdornmentDetails> adornments;
	std::vector<ItemArrayUnknown> footerUnknownArray;
	std::vector<ItemEffect> effects;
	std::vector<ItemTierLineEntry> tierLine;
	std::string unknown68;
	std::string unknown71;
	std::string tradeRestrictedTime;
	std::string itemName;
	std::string description;
	std::string unknown88String;
	std::string unknown93String;
	std::string offersQuest;
	std::string requiredByQuest;
	std::string setName;


protected:
	//For some versions this was a bitmask, later changed to just array of bool
	uint32_t adornSlotBitmask;
};

enum class EItemType : uint8_t {
	EINVALID = 0xFF,
	EGENERIC = 0,
	EMELEE_WEAPON = 1,
	ERANGED_WEAPON = 2,
	EARMOR = 3,
	ESHIELD = 4,
	EBAG = 5,
	ESPELL_SCROLL = 6,
	ERECIPE_BOOK = 7,
	EPROVISION = 8,
	EBAUBLE = 9,
	EHOUSE = 10,
	EAMMO = 11,
	EHOUSE_CONTAINER = 12,
	EADORNMENT = 13,
	EACHIEVEMENT_PROFILE = 16,
	EREWARD_VOUCHER = 17,
	EREWARD_CRATE = 18,
	EBOOK = 19,
	EREFORGING_DECORATION = 20,
	EDUNGEON_MAKER = 21,
	EMARKETPLACE = 22,
	EREWARD_CRATE_2 = 23,
	EINFUSER_1 = 24,
	EINFUSER_2 = 25,
	EEXPERIENCE_VIAL = 26,
	EOVERSEER = 27,
	ECOUPON = 28
};

class Item : public ItemDescBaseData, public ItemDescFooterData, public std::enable_shared_from_this<Item> {
protected:
	Item();
	Item(const Item& rhs);

	virtual std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const = 0;

public:
	virtual ~Item() = default;
	std::unique_ptr<Substruct_ExamineDescItem> GetPacketData(uint32_t version, bool bPvp = false) const;
	virtual std::shared_ptr<Item> Copy() const = 0;
	virtual void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) = 0;

	static std::shared_ptr<Item> CreateItemWithType(EItemType type);

	bool bUseable;
	uint32_t scriptID;
	std::optional<EQ2EquipmentItem> appearance;

	static EItemType GetItemTypeFromName(const char* name);

	static const uint64_t ITEM_FLAG_ATTUNED = 1 << 0;
	static const uint64_t ITEM_FLAG_ATTUNEABLE = 1 << 1;
	static const uint64_t ITEM_FLAG_ARTIFACT = 1 << 2;
	static const uint64_t ITEM_FLAG_LORE = 1 << 3;
	static const uint64_t ITEM_FLAG_TEMPORARY = 1 << 4;
	static const uint64_t ITEM_FLAG_NO_TRADE = 1 << 5;
	static const uint64_t ITEM_FLAG_NO_VALUE = 1 << 6;
	static const uint64_t ITEM_FLAG_NO_ZONE = 1 << 7;
	static const uint64_t ITEM_FLAG_NO_DESTROY = 1 << 8;
	static const uint64_t ITEM_FLAG_CRAFTED = 1 << 9;
	static const uint64_t ITEM_FLAG_GOOD_ONLY = 1 << 10;
	static const uint64_t ITEM_FLAG_EVIL_ONLY = 1 << 11;
	static const uint64_t ITEM_FLAG_STACK_LORE = 1 << 12;
	static const uint64_t ITEM_FLAG_LORE_EQUIP = 1 << 13;
	static const uint64_t ITEM_FLAG_NO_TRANSMUTE = 1 << 14; //resilient on older clients (coe)
	static const uint64_t ITEM_FLAG_CURSED = 1 << 15;
	static const uint64_t ITEM_FLAG_ORNATE = 1 << 16;
	static const uint64_t ITEM_FLAG_HEIRLOOM = 1 << 17;
	static const uint64_t ITEM_FLAG_APPEARANCE_ONLY = 1 << 18;
	static const uint64_t ITEM_FLAG_UNLOCKED = 1 << 19;
	static const uint64_t ITEM_FLAG_REFORGED = 1 << 20;
	static const uint64_t ITEM_FLAG_NO_REPAIR = 1 << 21;
	static const uint64_t ITEM_FLAG_ETHEREAL = 1 << 22;
	static const uint64_t ITEM_FLAG_REFINED = 1 << 23;
	static const uint64_t ITEM_FLAG_NO_SALVAGE = 1 << 24;
	//Above flags are all on COE
	static const uint64_t ITEM_FLAG_INDESTRUCTIBLE = 1 << 25;
	static const uint64_t ITEM_FLAG_NO_EXPERIMENT = 1 << 26;
	static const uint64_t ITEM_FLAG_HOUSE_LORE = 1 << 27;
	static const uint64_t ITEM_FLAG_UNK_28 = 1 << 28;
	static const uint64_t ITEM_FLAG_BUILDING_BLOCK = 1 << 29;
	static const uint64_t ITEM_FLAG_FREE_REFORGE = 1 << 30;
	static const uint64_t ITEM_FLAG_INFUSABLE = 1u << 31;
	static const uint64_t ITEM_FLAG_MERC_ONLY = 1ull << 32;
	static const uint64_t ITEM_FLAG_MOUNT_ONLY = 1ull << 33;
};

class ItemGeneric : public Item {
public:
	ItemGeneric() = default;
	~ItemGeneric() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;

	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemMeleeWeapon : public Item, public ItemMeleeWeaponData {
public:
	ItemMeleeWeapon() = default;
	~ItemMeleeWeapon() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemRangedWeapon : public Item, public ItemRangedWeaponData {
public:
	ItemRangedWeapon() = default;
	~ItemRangedWeapon() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemArmor : public Item, public ItemArmorData {
public:
	ItemArmor() = default;
	~ItemArmor() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemShield : public Item, public ItemShieldData {
public:
	ItemShield() = default;
	~ItemShield() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemBag : public Item, public ItemBagData {
public:
	ItemBag() = default;
	~ItemBag() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemRecipeBook : public Item, public ItemRecipeBookData {
public:
	ItemRecipeBook() = default;
	~ItemRecipeBook() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemProvision : public Item, public ItemProvisionData {
public:
	ItemProvision() = default;
	~ItemProvision() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemBauble : public Item, public ItemBaubleData {
public:
	ItemBauble() = default;
	~ItemBauble() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemHouse : public Item, public ItemHouseData {
public:
	ItemHouse() = default;
	~ItemHouse() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemAmmo : public Item, public ItemAmmoData {
public:
	ItemAmmo() = default;
	~ItemAmmo() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemAdornment : public Item, public ItemAdornmentData {
public:
	ItemAdornment() = default;
	~ItemAdornment() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemAchievementProfile : public Item, public ItemAchievementProfileData {
public:
	ItemAchievementProfile() = default;
	~ItemAchievementProfile() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemRewardVoucher : public Item, public ItemRewardVoucherData {
public:
	ItemRewardVoucher() = default;
	~ItemRewardVoucher() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemRewardCrate : public Item, public ItemRewardCrateData {
public:
	ItemRewardCrate() = default;
	~ItemRewardCrate() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemBook : public Item, public ItemBookData {
public:
	ItemBook() = default;
	~ItemBook() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemReforgingDecoration : public Item, public ItemReforgingDecorationData {
public:
	ItemReforgingDecoration() = default;
	~ItemReforgingDecoration() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemHouseContainer : public Item, public ItemHouseContainerData {
public:
	ItemHouseContainer() = default;
	~ItemHouseContainer() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};

class ItemSpellScroll : public Item, public ItemSpellScrollData {
public:
	ItemSpellScroll() = default;
	~ItemSpellScroll() = default;

	std::unique_ptr<Substruct_ExamineDescItem> GetItemTypeData(uint32_t version, bool bPvp = false) const override;
	std::shared_ptr<Item> Copy() const override;
	void LoadTypeSpecificData(DatabaseResult& res, uint32_t startIndex) override;
};