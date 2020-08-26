#include "stdafx.h"

#include "Item.h"
#include "../Packets/ItemPackets.h"
#include "MasterItemList.h"
#include "../ZoneServer/Client.h"

extern MasterItemList g_masterItemList;

//Packet functions
std::unique_ptr<Substruct_ExamineDescItem> Item::GetPacketData(const std::shared_ptr<Client>& client) const {
	auto ret = GetItemTypeData(client);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<const ItemDescBaseData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<const ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemGeneric::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Generic>(version);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemMeleeWeapon::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_MeleeWeapon>(version);

	static_cast<ItemMeleeWeaponData&>(*ret) = static_cast<const ItemMeleeWeaponData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRangedWeapon::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RangedWeapon>(version);

	static_cast<ItemRangedWeaponData&>(*ret) = static_cast<const ItemRangedWeaponData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemArmor::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Armor>(version);

	static_cast<ItemArmorData&>(*ret) = static_cast<const ItemArmorData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemShield::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Shield>(version);

	static_cast<ItemShieldData&>(*ret) = static_cast<const ItemShieldData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBag::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Bag>(version);

	static_cast<ItemBagData&>(*ret) = static_cast<const ItemBagData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRecipeBook::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RecipeBook>(version);

	static_cast<ItemRecipeBookData&>(*ret) = static_cast<const ItemRecipeBookData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemProvision::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Provision>(version);

	static_cast<ItemProvisionData&>(*ret) = static_cast<const ItemProvisionData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBauble::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Bauble>(version);

	static_cast<ItemBaubleData&>(*ret) = static_cast<const ItemBaubleData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemHouse::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_House>(version);

	static_cast<ItemHouseData&>(ret->houseData) = static_cast<const ItemHouseData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAmmo::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Ammo>(version);

	static_cast<ItemAmmoData&>(*ret) = static_cast<const ItemAmmoData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAdornment::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Adornment>(version);

	static_cast<ItemAdornmentData&>(*ret) = static_cast<const ItemAdornmentData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAchievementProfile::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_AchievementProfile>(version);

	static_cast<ItemAchievementProfileData&>(*ret) = static_cast<const ItemAchievementProfileData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRewardVoucher::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RewardVoucher>(version);

	static_cast<ItemRewardVoucherData&>(*ret) = static_cast<const ItemRewardVoucherData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRewardCrate::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RewardCrate>(version);

	static_cast<ItemRewardCrateData&>(*ret) = static_cast<const ItemRewardCrateData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBook::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Book>(version);

	static_cast<ItemBookData&>(*ret) = static_cast<const ItemBookData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemReforgingDecoration::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_ReforgingDecoration>(version);

	static_cast<ItemReforgingDecorationData&>(*ret) = static_cast<const ItemReforgingDecorationData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemHouseContainer::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_HouseContainer>(version);

	static_cast<ItemHouseContainerData&>(*ret) = static_cast<const ItemHouseContainerData&>(*this);

	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemSpellScroll::GetItemTypeData(const std::shared_ptr<Client>& client) const {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_SpellScroll>(version);

	static_cast<ItemSpellScrollData&>(*ret) = static_cast<const ItemSpellScrollData&>(*this);

	return ret;
}

//Copy functions
Item::Item(const Item& rhs) : ItemDescBaseData(rhs), ItemDescFooterData(rhs), 
bUseable(rhs.bUseable), scriptID(rhs.scriptID), appearance(rhs.appearance) {
	uniqueID = g_masterItemList.GetNextUniqueID();
}

std::shared_ptr<Item> ItemGeneric::Copy() const {
	return std::make_shared<ItemGeneric>(*this);
}

std::shared_ptr<Item> ItemMeleeWeapon::Copy() const {
	return std::make_shared<ItemMeleeWeapon>(*this);
}

std::shared_ptr<Item> ItemRangedWeapon::Copy() const {
	return std::make_shared<ItemRangedWeapon>(*this);
}

std::shared_ptr<Item> ItemArmor::Copy() const {
	return std::make_shared<ItemArmor>(*this);
}

std::shared_ptr<Item> ItemShield::Copy() const {
	return std::make_shared<ItemShield>(*this);
}

std::shared_ptr<Item> ItemBag::Copy() const {
	return std::make_shared<ItemBag>(*this);
}

std::shared_ptr<Item> ItemRecipeBook::Copy() const {
	return std::make_shared<ItemRecipeBook>(*this);
}

std::shared_ptr<Item> ItemProvision::Copy() const {
	return std::make_shared<ItemProvision>(*this);
}

std::shared_ptr<Item> ItemBauble::Copy() const {
	return std::make_shared<ItemBauble>(*this);
}

std::shared_ptr<Item> ItemHouse::Copy() const {
	return std::make_shared<ItemHouse>(*this);
}

std::shared_ptr<Item> ItemAmmo::Copy() const {
	return std::make_shared<ItemAmmo>(*this);
}

std::shared_ptr<Item> ItemAdornment::Copy() const {
	return std::make_shared<ItemAdornment>(*this);
}

std::shared_ptr<Item> ItemAchievementProfile::Copy() const {
	return std::make_shared<ItemAchievementProfile>(*this);
}

std::shared_ptr<Item> ItemRewardVoucher::Copy() const {
	return std::make_shared<ItemRewardVoucher>(*this);
}

std::shared_ptr<Item> ItemRewardCrate::Copy() const {
	return std::make_shared<ItemRewardCrate>(*this);
}

std::shared_ptr<Item> ItemBook::Copy() const {
	return std::make_shared<ItemBook>(*this);
}

std::shared_ptr<Item> ItemReforgingDecoration::Copy() const {
	return std::make_shared<ItemReforgingDecoration>(*this);
}

std::shared_ptr<Item> ItemHouseContainer::Copy() const {
	return std::make_shared<ItemHouseContainer>(*this);
}

std::shared_ptr<Item> ItemSpellScroll::Copy() const {
	return std::make_shared<ItemSpellScroll>(*this);
}

EItemType Item::GetItemTypeFromName(const char* name) {
	CStringCmpNoCopy cmp(name);
	if (cmp == "Normal") {
		return EItemType::EGENERIC;
	}
	else if (cmp == "Weapon") {
		return EItemType::EMELEE_WEAPON;
	}
	else if (cmp == "Ranged") {
		return EItemType::ERANGED_WEAPON;
	}
	else if (cmp == "Armor") {
		return EItemType::EARMOR;
	}
	else if (cmp == "Shield") {
		return EItemType::ESHIELD;
	}
	else if (cmp == "Bag") {
		return EItemType::EBAG;
	}
	else if (cmp == "Recipe") {
		return EItemType::ERECIPE_BOOK;
	}
	else if (cmp == "Food") {
		return EItemType::EPROVISION;
	}
	else if (cmp == "Bauble") {
		return EItemType::EBAUBLE;
	}
	else if (cmp == "House") {
		return EItemType::EHOUSE;
	}
	else if (cmp == "Thrown") {
		return EItemType::EAMMO;
	}
	else if (cmp == "House Container") {
		return EItemType::EHOUSE_CONTAINER;
	}
	else if (cmp == "Adornemnt") {
		return EItemType::EADORNMENT;
	}
	else if (cmp == "Profile") {
		return EItemType::EACHIEVEMENT_PROFILE;
	}
	else if (cmp == "Pattern Set") {
		return EItemType::EREWARD_VOUCHER;
	}
	else if (cmp == "Item Set") {
		return EItemType::EREWARD_CRATE;
	}
	else if (cmp == "Book") {
		return EItemType::EBOOK;
	}
	else if (cmp == "Decoration") {
		return EItemType::EREFORGING_DECORATION;
	}
	else if (cmp == "Dungeon Maker") {
		return EItemType::EDUNGEON_MAKER;
	}
	else if (cmp == "Marketplace") {
		return EItemType::EMARKETPLACE;
	}
	else if (cmp == "Scroll") {
		return EItemType::ESPELL_SCROLL;
	}

	return EItemType::EINVALID;
}

Item::Item() : bUseable(false), scriptID(0) {
	//Initialize primitive data
	ItemDescBaseData* descPrimitiveStart = this;
	memset(descPrimitiveStart, 0, &ItemDesc_START_DYNAMIC_DATA - reinterpret_cast<uint8_t*>(descPrimitiveStart));
	ItemDescFooterData* footerPrimitiveStart = this;
	memset(footerPrimitiveStart, 0, &ItemFooter_START_DYNAMIC_DATA - reinterpret_cast<uint8_t*>(footerPrimitiveStart));
	setDetails.setBonusCount = 0;
	setDetails.setItemsCount = 0;
	setDetails.numSetItemsEquipped = 0;
	setDetails.numItemsInSet = 0;

	memset(adornSlots, 0xFF, sizeof(adornSlots));
	uniqueID = 0xFFFFFFFF;
	footerTypeUnknown = 3;
	condition = 100;
}

std::shared_ptr<Item> Item::CreateItemWithType(EItemType type) {
	switch (type) {
	case EItemType::EACHIEVEMENT_PROFILE:
		return std::make_shared<ItemAchievementProfile>();
	case EItemType::EARMOR:
		return std::make_shared<ItemArmor>();
	case EItemType::EMELEE_WEAPON:
		return std::make_shared<ItemMeleeWeapon>();
	case EItemType::ERANGED_WEAPON:
		return std::make_shared<ItemRangedWeapon>();
	case EItemType::ESHIELD:
		return std::make_shared<ItemShield>();
	case EItemType::EBAG:
		return std::make_shared<ItemBag>();
	case EItemType::EBAUBLE:
		return std::make_shared<ItemBauble>();
	case EItemType::EHOUSE:
		return std::make_shared<ItemHouse>();
	case EItemType::EHOUSE_CONTAINER:
		return std::make_shared<ItemHouseContainer>();
	case EItemType::EBOOK:
		return std::make_shared<ItemBook>();
	case EItemType::EPROVISION:
		return std::make_shared<ItemProvision>();
	case EItemType::EREFORGING_DECORATION:
		return std::make_shared<ItemReforgingDecoration>();
	case EItemType::EMARKETPLACE:
	case EItemType::EEXPERIENCE_VIAL:
	case EItemType::EOVERSEER:
	case EItemType::EREWARD_VOUCHER:
	case EItemType::EREWARD_CRATE_2:
		return std::make_shared<ItemRewardVoucher>();
	case EItemType::EREWARD_CRATE:
		return std::make_shared<ItemRewardCrate>();
	case EItemType::EAMMO:
		return std::make_shared<ItemAmmo>();
	case EItemType::ESPELL_SCROLL:
		return std::make_shared<ItemSpellScroll>();
	default:
		return std::make_shared<ItemGeneric>();
	}
}