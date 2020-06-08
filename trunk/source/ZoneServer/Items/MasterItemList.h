#pragma once

#include "../../common/Mutex.h"
#include "Item.h"
#include <memory>
#include <cstdint>
#include <unordered_map>

class MasterItemList {
public:
	MasterItemList() {}
	~MasterItemList() = default;

	void AssignItems(std::unordered_map<uint32_t, std::shared_ptr<const Item>>& items) {
		WriteLocker lock(m_itemsLock);
		m_items.swap(items);
	}

	std::shared_ptr<const Item> GetReferenceItem(uint32_t itemID) {
		ReadLocker lock(m_itemsLock);
		auto itr = m_items.find(itemID);
		if (itr != m_items.end()) {
			return itr->second;
		}

		return std::shared_ptr<const Item>();
	}

	std::shared_ptr<Item> GetNewItem(uint32_t itemID) {
		ReadLocker lock(m_itemsLock);
		auto itr = m_items.find(itemID);
		if (itr != m_items.end()) {
			return itr->second->Copy();
		}

		return std::shared_ptr<Item>();
	}

private:
	Mutex m_itemsLock;
	std::unordered_map<uint32_t, std::shared_ptr<const Item>> m_items;
};