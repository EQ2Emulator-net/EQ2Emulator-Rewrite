#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

inline uint8_t GetDescriptionVersion(uint32_t version) {
	uint8_t subVersion;

	if (version >= 70704)
		subVersion = 99;
	else if (version >= 69195)
		subVersion = 98;
	else if (version >= 67727)
		subVersion = 97;
	else if (version >= 67650)
		subVersion = 95;
	else if (version >= 64707)
		subVersion = 92;
	else if (version >= 63119)
		subVersion = 86;
	else if (version >= 60024)
		subVersion = 81;
	else if (version >= 57107)
		subVersion = 76;
	else if (version >= 57048)
		subVersion = 70;
	else if (version >= 1199)
		subVersion = 68;
	else if (version >= 1195)
		subVersion = 64;
	else if (version >= 1191)
		subVersion = 63;
	else if (version >= 1190)
		subVersion = 62;
	else if (version >= 1188)
		subVersion = 61;
	else if (version >= 1096)
		subVersion = 53;
	else if (version >= 1027)
		subVersion = 49;
	else if (version >= 1008)
		subVersion = 44;
	else if (version >= 927)
		subVersion = 35;
	else if (version >= 893)
		subVersion = 34;
	else if (version >= 860)
		subVersion = 32;
	else
		subVersion = 28;

	return subVersion;
}

class Substruct_ExamineDescBase : public PacketSubstruct {
public:
	Substruct_ExamineDescBase(uint32_t ver = 0, bool bInline = false, bool p_bIncludeExamineHeader = true) : PacketSubstruct(ver, bInline), 
		examineType(0xFF), subVersion(0xFF), examineTypeOverride(0xFF), noExamineInfo{ 0xFF, 0xFF, 0xFF }, bIncludeExamineHeader(p_bIncludeExamineHeader) {
		if (bIncludeExamineHeader) {
			RegisterElements();
		}
	}

	~Substruct_ExamineDescBase() = default;

	uint8_t examineTypeOverride;
	uint8_t noExamineInfo[3];
	uint8_t subVersion;
	//Valid types are 0 = item, 1 = effect, 2 = recipe, 3 = spell, 4 = AA, 5 = maintained, set 0x80 bit if sending 2 descriptions
	uint8_t examineType;

	void RegisterElements() {
		if (bIncludeExamineHeader) {
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
	}

private:
	bool bIncludeExamineHeader;
};