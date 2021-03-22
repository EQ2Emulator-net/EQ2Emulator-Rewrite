#pragma once

#include "Substruct_SpellDesc.h"

class Substruct_ExamineDescSpell : public Substruct_ExamineDescBase {
public:
	Substruct_ExamineDescSpell(uint32_t ver = 0) : Substruct_ExamineDescBase(ver), spell(ver) {
		examineType = 0xFE;
		examineTypeOverride = 3;
		subVersion = GetDescriptionVersion(ver);
		RegisterSubstruct(spell);
	}

	~Substruct_ExamineDescSpell() = default;

	Substruct_SpellDesc spell;
};