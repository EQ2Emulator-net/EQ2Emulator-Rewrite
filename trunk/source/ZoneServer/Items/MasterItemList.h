#pragma once

#include "../../common/Mutex.h"
#include "Item.h"
#include <memory>
#include <cstdint>
#include <unordered_map>

class MasterItemList {
public:
	MasterItemList() : m_nextUniqueID(1) {}
	~MasterItemList() = default;

	void AssignItems(std::unordered_map<uint32_t, std::shared_ptr<const Item>>& items, uint32_t nextUniqueID) {
		if (m_nextUniqueID == 1) {
			m_nextUniqueID.store(nextUniqueID);
		}

		WriteLocker lock(m_itemsLock);
		m_items.swap(items);
	}

	uint32_t GetNextUniqueID() {
		return m_nextUniqueID.fetch_add(1);
	}

private:
	Mutex m_itemsLock;
	std::unordered_map<uint32_t, std::shared_ptr<const Item>> m_items;
	std::atomic<uint32_t> m_nextUniqueID;
};