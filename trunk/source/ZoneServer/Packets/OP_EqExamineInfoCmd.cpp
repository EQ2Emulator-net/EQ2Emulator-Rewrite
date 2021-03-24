#include "stdafx.h"

#include "OP_EqExamineInfoCmd.h"
#include "ItemPackets.h"

EQ2Packet* OP_EqExamineInfoCmd_Packet::GetSubPacket() {
	EQ2Packet* ret = nullptr;
	if (typeid(*this) == typeid(OP_EqExamineInfoCmd_Packet)) {
		uint8_t examineType = baseSubstruct->examineType;
		if (examineType == 0xFE) {
			examineType = baseSubstruct->examineTypeOverride;
		}

		//Return a subpacket based on the examine type...for item spell etc
		bool bPvpDesc = (examineType & 0x80) != 0;
		examineType &= ~0x80;

		//Item
		if (examineType == 0) {
			ret = new ExamineInfoCmd_Item_Packet(
				GetVersion(), new Substruct_ExamineDescItem_HeaderOnly(GetVersion()), true);
		}
	}
	return ret;
}