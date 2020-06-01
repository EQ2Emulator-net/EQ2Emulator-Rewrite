#pragma once

#include "OP_ClientCmdMsg_Packet.h"
#include "Substruct_ExamineDescBase.h"

class OP_EqExamineInfoCmd_Packet : public OP_ClientCmdMsg_Packet {
public:
	OP_EqExamineInfoCmd_Packet(uint32_t version, bool bStructDump = true)
		: OP_ClientCmdMsg_Packet(version), bShowName(true), bShowPopup(false), unknown1(0), unknown{ 0, 0, 0 } {
		RegisterElements(bStructDump);
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
	
	void FindOpcode() override {
		if (typeid(*this) != typeid(OP_EqExamineInfoCmd_Packet)) {
			OP_EqExamineInfoCmd_Packet p(GetVersion(), _ENOREGISTER);
			p.FindOpcode();
			opcode = p.GetOpcode();
			SetSubOpcode(p.GetSubOpcode());
		}
		else {
			OP_ClientCmdMsg_Packet::FindOpcode();
		}
	}

private:
	std::optional<Substruct_ExamineDescBase> baseSubstruct;

	void RegisterElements(bool bStructDump) {
		RegisterBool(bShowName);
		RescopeArrayElement(unknown);
		RegisterUInt8(unknown)->SetCount(GetVersion() >= 1188 ? 3 : 2);
		RegisterBool(bShowPopup);
		RegisterUInt8(unknown1);

		if (bStructDump) {
			baseSubstruct.emplace(GetVersion(), true);
			RegisterSubstruct(*baseSubstruct);
		}
	}
};