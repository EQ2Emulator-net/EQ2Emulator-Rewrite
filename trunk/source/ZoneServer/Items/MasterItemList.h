#pragma once

#include "../../common/Mutex.h"
#include "Item.h"
#include <memory>
#include <cstdint>
#include <unordered_map>
#include "../../common/Packets/EmuPackets/Emu_ItemIDRequest_Packet.h"
#include "../ZoneServer/ZoneOperator.h"
#include "../WorldTalk/WorldStream.h"

extern ZoneOperator g_zoneOperator;

class MasterItemList {
public:
	MasterItemList() : availableUniqueIDCount(0), bRangeRequestSent(false) {}
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

	uint32_t GetNextUniqueID() {
		uint32_t ret = 0;
		bool bSendRequest = false;
		for (;;) {
			SpinLocker lock(m_uniqueIDLock);
			if (ranges.empty()) {
				//This should only happen around startup if at all
				lock.Unlock();
				SleepMS(50);
				continue;
			}

			auto& range = ranges.front();
			ret = range.first++;

			if (ret == range.second) {
				//We've reached the end of the range, trash it
				ranges.pop_front();
			}

			if (--availableUniqueIDCount < 500 && !bRangeRequestSent) {
				bSendRequest = true;
				bRangeRequestSent = true;
			}

			break;
		}

		if (bSendRequest) {
			SendIDRequestToWorld();
		}
	}

	void AddUniqueIDRange(uint32_t low, uint32_t high) {
		SpinLocker lock(m_uniqueIDLock);
		ranges.emplace_back(low, high);
		availableUniqueIDCount += high - low;
	}

	void SendIDRequestToWorld() {
		g_zoneOperator.GetWorldStream()->QueuePacket(new Emu_ItemIDRequest_Packet);
	}

private:
	Mutex m_itemsLock;
	std::unordered_map<uint32_t, std::shared_ptr<const Item>> m_items;

	SpinLock m_uniqueIDLock;
	std::deque<std::pair<uint32_t, uint32_t>> ranges;
	int32_t availableUniqueIDCount;
	bool bRangeRequestSent;
};