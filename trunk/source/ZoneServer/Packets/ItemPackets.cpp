#include "stdafx.h"

#include "ItemPackets.h"

EQ2Packet* ExamineInfoCmd_Item_Packet::GetSubPacket() {
	EQ2Packet* ret = nullptr;
	if (typeid(*itemDesc) == typeid(Substruct_ExamineDescItem_HeaderOnly)) {
		auto item = Item::CreateItemWithType(static_cast<EItemType>(itemDesc->header.itemType));
		std::unique_ptr<Substruct_ExamineDescItem> new_desc = item->GetPacketData(GetVersion());
		Substruct_ExamineDescItem* pvp = nullptr;

		if (itemDesc->examineTypeOverride & 0x80) {
			//This item has a pvp desc
			pvp = item->GetPacketData(GetVersion(), true).release();
		}

		ret = new ExamineInfoCmd_Item_Packet(GetVersion(), new_desc.release(), true, pvp);	
	}
	return ret;
}