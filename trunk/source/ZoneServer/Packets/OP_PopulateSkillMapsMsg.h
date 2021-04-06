#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"

class OP_PopulateSkillMapsMsg_Packet : public EQ2Packet {
public:
	OP_PopulateSkillMapsMsg_Packet(uint32_t version)
		: EQ2Packet(version), skill_count(0) {
		RegisterElements();
	}

	~OP_PopulateSkillMapsMsg_Packet() = default;

	class Substruct_SkillMapEntry : public PacketSubstruct {
	public:
		Substruct_SkillMapEntry(uint32_t ver = 0) : PacketSubstruct(ver, true), skillID(0) {
		}

		void RegisterElements() {
			RegisterUInt32(skillID);
			Register16String(shortName);
			Register16String(name);
			Register16String(description);
		}

		uint32_t skillID;
		std::string shortName;
		std::string name;
		std::string description;
	};

	uint32_t skill_count;
	std::vector<Substruct_SkillMapEntry> skills;

	void RegisterElements() {
		auto e = RegisterUInt32(skill_count);
		e->SetMyArray(RegisterArray(skills, Substruct_SkillMapEntry));
	}
};