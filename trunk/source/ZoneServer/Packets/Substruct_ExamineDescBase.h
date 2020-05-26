#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

class Substruct_ExamineDescBase : public PacketSubstruct {
public:
	Substruct_ExamineDescBase(uint32_t ver = 0, bool bInline = false) : PacketSubstruct(ver, bInline), examineType(0xFF), subVersion(0xFF), examineTypeOverride(0xFF), noExamineInfo{ 0xFF, 0xFF, 0xFF } {
		RegisterElements();
	}

	~Substruct_ExamineDescBase() = default;

	uint8_t examineTypeOverride;
	uint8_t noExamineInfo[3];
	uint8_t subVersion;
	//Valid types are 0 = item, 1 = effect, 2 = recipe, 3 = spell, 4 = AA, 5 = maintained, set 0x80 bit if sending 2 descriptions
	uint8_t examineType;

	void RegisterElements() {
		PacketUInt8* e = RegisterUInt8(examineType);
		RegisterUInt8(subVersion)->SetIfVariableEquals(0xFE, e);
		//if the first examine type is 0xFE then that means the real examine type is read again after the subtype
		RegisterUInt8(examineTypeOverride)->SetIfVariableEquals(0xFE, e);

		//An option for these packets is to send 4 0xFF bytes (-1 int) starting at where the first examine type element is
		//This prevent more data from being sent after that
		RescopeArrayElement(noExamineInfo);
		PacketUInt8* noInfo = RegisterUInt8(noExamineInfo);
		noInfo->SetCount(3);
		noInfo->SetIfVariableEquals(0xFF, e);
	}
};