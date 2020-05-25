#pragma once

#include "../Packets/OP_ClientCmdMsg_Packet.h"

class OP_EqExamineInfoCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqExamineInfoCmd_Packet(uint32_t version)
		: OP_ClientCmdMsg_Packet(version), bShowName(true), bShowPopup(false), unknown1(0), examineType(0xFF), subtype(0xFF) {
		RegisterElements();
		memset(unknown, 0, sizeof(unknown));
		memset(noExamineInfo, 0xFF, sizeof(noExamineInfo));
	}

private:
	enum ENOREGISTER { _ENOREGISTER };
	OP_EqExamineInfoCmd_Packet(uint32_t version, ENOREGISTER) : OP_ClientCmdMsg_Packet(version) {}

public:
	//I think this is really just to show the examine window...not a great name
	bool bShowName;
	uint8_t unknown[3];
	bool bShowPopup;
	uint8_t unknown1;
	uint8_t subtype;
	//Valid types are 0 = item, 1 = effect, 2 = recipe, 3 = spell, 4 = AA, 5 = maintained, set 0x80 bit if sending 2 descriptions
	uint8_t examineType;
	uint8_t noExamineInfo[3];

	void FindOpcode() override {
		if (typeid(*this) != typeid(OP_EqExamineInfoCmd_Packet)) {
			OP_EqExamineInfoCmd_Packet p(GetVersion(), _ENOREGISTER);
			opcode = p.GetOpcode();
			SetSubOpcode(p.GetSubOpcode());
		}
		else {
			OP_ClientCmdMsg_Packet::FindOpcode();
		}
	}

private:
	void RegisterElements() {
		RegisterBool(bShowName);
		RescopeArrayElement(unknown);
		RegisterUInt8(unknown)->SetCount(GetVersion() >= 1188 ? 3 : 2);
		RegisterBool(bShowPopup);
		RegisterUInt8(unknown1);
		PacketUInt8* e = RegisterUInt8(examineType);
		RegisterUInt8(subtype)->SetIfVariableEquals(0xFE, e);
		//if the first examine type is 0xFE then that means the real examine type is read again after the subtype
		uint8_t& examineTypeOverride = examineType;
		RegisterUInt8(examineTypeOverride)->SetIfVariableEquals(0xFE, e);
			
		//An option for these packets is to send 4 0xFF bytes (-1 int) starting at where the first examine type element is
		//This prevent more data from being sent after that
		RescopeArrayElement(noExamineInfo);
		PacketUInt8* noInfo = RegisterUInt8(noExamineInfo);
		noInfo->SetCount(3);
		noInfo->SetIfVariableEquals(0xFF, e);
	}
};