#include "stdafx.h"

#include "Item.h"
#include "../Packets/ItemPackets.h"
#include "MasterItemList.h"
#include "../ZoneServer/Client.h"

extern MasterItemList g_masterItemList;


//Packet functions
std::unique_ptr<Substruct_ExamineDescItem> ItemGeneric::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Generic>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemMeleeWeapon::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_MeleeWeapon>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemMeleeWeaponData&>(*ret) = static_cast<ItemMeleeWeaponData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRangedWeapon::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RangedWeapon>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemRangedWeaponData&>(*ret) = static_cast<ItemRangedWeaponData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemArmor::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Armor>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemArmorData&>(*ret) = static_cast<ItemArmorData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemShield::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Shield>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemShieldData&>(*ret) = static_cast<ItemShieldData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBag::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Bag>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemBagData&>(*ret) = static_cast<ItemBagData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRecipeBook::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RecipeBook>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemRecipeBookData&>(*ret) = static_cast<ItemRecipeBookData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemProvision::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Provision>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemProvisionData&>(*ret) = static_cast<ItemProvisionData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBauble::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Bauble>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemBaubleData&>(*ret) = static_cast<ItemBaubleData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemHouse::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_House>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemHouseData&>(ret->houseData) = static_cast<ItemHouseData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAmmo::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Ammo>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemAmmoData&>(*ret) = static_cast<ItemAmmoData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAdornment::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Adornment>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemAdornmentData&>(*ret) = static_cast<ItemAdornmentData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemAchievementProfile::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_AchievementProfile>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemAchievementProfileData&>(*ret) = static_cast<ItemAchievementProfileData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRewardVoucher::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RewardVoucher>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemRewardVoucherData&>(*ret) = static_cast<ItemRewardVoucherData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemRewardCrate::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_RewardCrate>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemRewardCrateData&>(*ret) = static_cast<ItemRewardCrateData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemBook::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_Book>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemBookData&>(*ret) = static_cast<ItemBookData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

std::unique_ptr<Substruct_ExamineDescItem> ItemReforgingDecoration::GetPacketData(const std::shared_ptr<Client>& client) {
	uint32_t version = client->GetVersion();
	auto ret = std::make_unique<Substruct_ExamineDescItem_ReforgingDecoration>(version);
	static_cast<ItemDescBaseData&>(ret->header) = static_cast<ItemDescBaseData&>(*this);
	static_cast<ItemReforgingDecorationData&>(*ret) = static_cast<ItemReforgingDecorationData&>(*this);
	static_cast<ItemDescFooterData&>(ret->footer) = static_cast<ItemDescFooterData&>(*this);
	return ret;
}

//Copy functions
Item::Item(const Item& rhs) : ItemDescBaseData(rhs), ItemDescFooterData(rhs) {
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

EItemType Item::GetItemTypeFromName(const char* name) {
	CStringCmpNoCopy cmp(name);
	if (cmp == "Normal") {
		return EItemType::EGENERIC;
	}
	else if (cmp == "Weapon") {
		return EItemType::EWEAPON;
	}
	else if (cmp == "Ranged") {
		return EItemType::ERANGED_WEAPON;
	}
	else if (cmp == "Armor") {
		return EItemType::ESHIELD;
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
}