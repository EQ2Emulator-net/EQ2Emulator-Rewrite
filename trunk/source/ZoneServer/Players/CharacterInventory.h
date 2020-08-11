#pragma once

#include <vector>
#include <memory>
#include <map>
#include "../Items/Item.h"
#include "../Items/ItemTypes.h"

class BaseInventorySlot {
public:
	BaseInventorySlot() : numSlots(0) {}
	virtual ~BaseInventorySlot() = default;

	virtual bool AddItem(const std::shared_ptr<Item>& item) {
		if (items.size() >= numSlots) {
			return false;
		}

		items.push_back(item);
	}

	uint32_t GetNumEmptySlots() {
		return numSlots - static_cast<uint32_t>(items.size());
	}

	uint32_t numSlots;
	std::vector<std::shared_ptr<Item>> items;
};

class InventoryBag : public BaseInventorySlot {
	virtual bool AddItem(const std::shared_ptr<Item>& item) {
		if (items.size() >= numSlots) {
			return false;
		}

		items.push_back(item);
	}
};

class BaseInventory {
protected:
	BaseInventory(uint8_t p_inventoryType) : inventoryType(p_inventoryType) {}

public:
	virtual ~BaseInventory() = default;

	uint8_t inventoryType;
	std::map<int32_t, std::unique_ptr<BaseInventorySlot>> slots;
};

class CharacterInventory : public BaseInventory {
public:
	CharacterInventory() : BaseInventory(0) {
		for (int32_t i = 1; i <= 6; i++) {
			auto bag = new BaseInventorySlot;
			bag->numSlots = 1;
			bags.emplace(i, );
		}
	}

	~CharacterInventory() = default;
};