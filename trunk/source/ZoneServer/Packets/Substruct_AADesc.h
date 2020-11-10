#pragma once

#include "Substruct_SpellDesc.h"
#include <string>
#include <vector>

struct AADescInfo {
	uint32_t id;
	uint16_t tab;
	uint16_t icon;
	uint16_t icon2;
	int8_t rank_current;
	int8_t rank_max;
	int8_t points_cost;
	uint8_t aa_unknown2;
	uint8_t unknownmj;

	AADescInfo() {
		//Zero out our primitive data
		memset(this, 0, reinterpret_cast<uint8_t*>(&name) - reinterpret_cast<uint8_t*>(this));
	}

	std::string name;
	std::string description;
};

class Substruct_AADesc : public PacketSubstruct, public AADescInfo {
public:
	Substruct_AADesc(uint32_t ver = 0, uint8_t p_descVer = 0) : PacketSubstruct(ver), descVer(p_descVer), spell_current(ver, p_descVer), spell_next(ver, p_descVer) {
	}

	~Substruct_AADesc() = default;

	void RegisterElements() override {
		if (descVer == 0) {
			descVer = GetDescriptionVersion(GetVersion());
		}

		RegisterUInt32(id);
		RegisterUInt16(tab);
		RegisterUInt16(icon);
		RegisterUInt16(icon2);
		RegisterInt8(rank_current);
		RegisterInt8(rank_max);
		RegisterInt8(points_cost);
		RegisterUInt8(aa_unknown2);
		RegisterSubstruct(spell_current);
		RegisterSubstruct(spell_next);
		std::string& name = AADescInfo::name;
		Register8String(name);
		Register16String(description);
		RegisterUInt8(unknownmj);
	}

	uint8_t descVer;
	Substruct_SpellDesc spell_current;
	Substruct_SpellDesc spell_next;
};