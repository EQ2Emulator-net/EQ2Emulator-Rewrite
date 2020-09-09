#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../Spawns/SpawnStructs.h"

enum class EGroupMemberSlotState : uint8_t {
	EMPTY = 0,
	IN_ZONE = 1,
	NOT_IN_ZONE = 2,
	UNKNOWN = 3 
};

class Substruct_GroupMember : public PacketSubstruct {
public:
	~Substruct_GroupMember() = default;

	uint64_t uniqueId; //first 4 bytes is the "dbid" listed on census for players, not sure the second 4 bytes
	uint32_t spawnId;
	uint32_t petId;
	int64_t hpCurrent;
	int64_t hpMax;
	int32_t powerCurrent;
	int32_t powerMax;
	int16_t effectiveLevel;
	int16_t level;
	uint8_t instance;
	EGroupMemberSlotState slotState;
	uint16_t unknown4;
	uint8_t race;
	uint8_t advClass;
	int8_t trauma;
	int8_t arcane;
	int8_t noxious;
	int8_t elemental;
	int8_t curse;
	uint8_t unknown5[2];
	uint8_t coeUnknown;
	uint8_t unknown54;

	std::string name;
	std::string zoneName;

	Substruct_GroupMember(uint32_t ver = 0) : PacketSubstruct(ver) {
		memset(&uniqueId, 0, reinterpret_cast<uint8_t*>(&name) - reinterpret_cast<uint8_t*>(&uniqueId));
		//name = "Test";
		//trauma = 1;
		//curse = 2;
		//slotState = EGroupMemberSlotState::IN_ZONE;
		//hpCurrent = 42;
		//hpMax = 42;
		//powerCurrent = 57;
		//powerMax = 57;
		//level = 120;
		//effectiveLevel = 80;
	}

	void RegisterElements() override {
		RegisterUInt64(uniqueId);
		RegisterUInt32(spawnId);
		RegisterUInt32(petId);
		if (GetVersion() >= 57048) {
			RegisterInt64(hpCurrent);
			RegisterInt64(hpMax);
		}
		else {
			int32_t& hpCurrent = hp_do_not_set;
			int32_t hpMax = hp_max_do_not_set;
			RegisterInt32(hpCurrent);
			RegisterInt32(hpMax);
		}
		RegisterInt32(powerCurrent);
		RegisterInt32(powerMax);
		RegisterInt16(effectiveLevel);
		RegisterInt16(level);
		RegisterCharString(name, 40);
		RegisterUInt8(unknown54);
		RegisterCharString(zoneName, 60);
		RegisterUInt8(instance);
		RescopeToReference(slotState, uint8_t);
		RegisterUInt8(slotState);
		RegisterUInt16(unknown4);
		RegisterUInt8(race);
		RegisterUInt8(advClass);
		RegisterInt8(trauma);
		RegisterInt8(arcane);
		RegisterInt8(noxious);
		RegisterInt8(elemental);
		RegisterInt8(curse);
		RescopeArrayElement(unknown5);
		RegisterUInt8(unknown5)->SetCount(2);
		RegisterUInt8(coeUnknown);
	}

	void PreWrite() override {
		if (GetVersion() >= 57048) {
			hp_do_not_set = static_cast<int32_t>(hpCurrent);
			hp_max_do_not_set = static_cast<int32_t>(hpMax);
		}
	}

	void PostRead() override {
		if (GetVersion() < 57048) {
			hpCurrent = hp_do_not_set;
			hpMax = hp_max_do_not_set;
		}
	}

private:
	int32_t hp_do_not_set;
	int32_t hp_max_do_not_set;
};

struct GroupMemberAppearanceData {
	GroupMemberAppearanceData() {
		memset(this, 0, sizeof(*this));
	}
	uint32_t model_type;
	uint32_t soga_model_type;

	//Slot appearances
	uint32_t head_slot_type;
	uint32_t chest_slot_type;
	uint32_t shoulders_slot_type;
	uint32_t textures_slot_type;
	uint32_t hair_slot_type;
	uint32_t hair_face_slot_type;
	uint32_t wings_slot_type;
	uint32_t soga_hair_slot_type;
	uint32_t soga_hair_face_slot_type;

	//Misc
	uint32_t vis_flags;
	union {
		SpawnMorphSliders              sliders;
		int8_t                         sliderBytes[26];
	};
	union {
		SpawnMorphSliders              sogaSliders;
		int8_t                         sogaSliderBytes[26];
	};

	//Model pallettes
	EQ2Color eye_color;
	EQ2Color skin_color;
	EQ2Color model_color;
	EQ2Color hair_color1;
	EQ2Color hair_color2;
	EQ2Color hair_scatter;
	EQ2Color soga_eye_color;
	EQ2Color soga_skin_color;
	EQ2Color soga_model_color;
	EQ2Color soga_hair_color1;
	EQ2Color soga_hair_color2;
	EQ2Color soga_hair_scatter;

	//Slot colors
	EQ2Color head_slot_color;
	EQ2Color chest_slot_color;
	EQ2Color shoulders_slot_color;
	EQ2Color textures_slot_color;
	EQ2Color hair_slot_color;
	EQ2Color hair_face_slot_color;
	EQ2Color wings_slot_color;
	EQ2Color soga_hair_slot_color;
	EQ2Color soga_hair_face_slot_color;

	//Slot highlights
	EQ2Color head_slot_highlight;
	EQ2Color chest_slot_highlight;
	EQ2Color shoulders_slot_highlight;
	EQ2Color textures_slot_highlight;
	EQ2Color hair_slot_highlight;
	EQ2Color hair_face_slot_highlight;
	EQ2Color wings_slot_highlight;
	EQ2Color soga_hair_slot_highlight;
	EQ2Color soga_hair_face_slot_highlight;
};

class Substruct_GroupMemberAppearance : public PacketSubstruct, public GroupMemberAppearanceData {
public:
	Substruct_GroupMemberAppearance(uint32_t ver = 0) : PacketSubstruct(ver), align_filler(0) {

	}

	~Substruct_GroupMemberAppearance() = default;

	void RegisterElements() override {
		RegisterUInt32(model_type);
		RegisterUInt32(soga_model_type);
		RegisterUInt32(head_slot_type);
		RegisterUInt32(chest_slot_type);
		RegisterUInt32(shoulders_slot_type);
		RegisterUInt32(textures_slot_type);
		RegisterUInt32(hair_slot_type);
		RegisterUInt32(hair_face_slot_type);
		RegisterUInt32(wings_slot_type);
		RegisterUInt32(soga_hair_slot_type);
		RegisterUInt32(soga_hair_face_slot_type);
		RegisterUInt32(vis_flags);
		RegisterEQ2Color(eye_color);
		RegisterEQ2Color(skin_color);
		RegisterEQ2Color(model_color);
		RegisterEQ2Color(soga_eye_color);
		RegisterEQ2Color(soga_skin_color);
		RegisterEQ2Color(soga_model_color);
		RegisterEQ2Color(head_slot_color);
		RegisterEQ2Color(chest_slot_color);
		RegisterEQ2Color(shoulders_slot_color);
		RegisterEQ2Color(textures_slot_color);
		RegisterEQ2Color(hair_slot_color);
		RegisterEQ2Color(hair_face_slot_color);
		RegisterEQ2Color(wings_slot_color);
		RegisterEQ2Color(head_slot_highlight);
		RegisterEQ2Color(chest_slot_highlight);
		RegisterEQ2Color(shoulders_slot_highlight);
		RegisterEQ2Color(textures_slot_highlight);
		RegisterEQ2Color(hair_slot_highlight);
		RegisterEQ2Color(hair_face_slot_highlight);
		RegisterEQ2Color(wings_slot_highlight);
		RegisterEQ2Color(soga_hair_slot_color);
		RegisterEQ2Color(soga_hair_slot_highlight);
		RegisterEQ2Color(soga_hair_face_slot_color);
		RegisterEQ2Color(soga_hair_face_slot_highlight);
		RescopeArrayElement(sliderBytes);
		RegisterInt8(sliderBytes)->SetCount(26);
		RescopeArrayElement(sogaSliderBytes);
		RegisterInt8(sogaSliderBytes)->SetCount(26);
		RegisterEQ2Color(hair_color1);
		RegisterEQ2Color(hair_color2);
		RegisterEQ2Color(hair_scatter);
		RegisterEQ2Color(soga_hair_color1);
		RegisterEQ2Color(soga_hair_color2);
		RegisterEQ2Color(soga_hair_scatter);
		RegisterUInt16(align_filler);
	}

	uint16_t align_filler;
};

class Substruct_GroupMemberMapLocation : public PacketSubstruct {
public:
	Substruct_GroupMemberMapLocation(uint32_t ver = 0) : PacketSubstruct(ver), x(0), y(0), z(0) {
	}

	void RegisterElements() override {
		RegisterCharString(map_name, 128);
		RegisterInt16(x);
		RegisterInt16(y);
		RegisterInt16(z);
	}

	std::string map_name;
	int16_t x;
	int16_t y;
	int16_t z;
};

class Substruct_GroupSheet : public PacketSubstruct {
public:
	Substruct_GroupSheet(uint32_t ver) : PacketSubstruct(ver), leaderIndex(0), alignFiller{ 0,0,0 }, unknown(0) {
		for (int i = 0; i < 5; i++) {
			members[i].ResetVersion(ver);
			appearances[i].ResetVersion(ver);
			locations[i].ResetVersion(ver);
		}

		//If leader index is -1 this player is the leader, otherwise index 0-5 is the leader
		leaderIndex = -1;
	}

	void RegisterElements() override {
		RegisterInt32(leaderIndex);
		RegisterInt32(unknown);
		if (GetVersion() >= 1188 && GetVersion() < 67650) {
			RescopeArrayElement(appearances);
			RegisterSubstruct(appearances)->SetCount(5);
		}
		RescopeArrayElement(members);
		RegisterSubstruct(members)->SetCount(5);
		RescopeArrayElement(locations);
		RegisterSubstruct(locations)->SetCount(5);
		RescopeArrayElement(alignFiller);
		RegisterUInt8(alignFiller)->SetCount(3);
	}

	int32_t leaderIndex;
	int32_t unknown;
	Substruct_GroupMemberAppearance appearances[5];
	Substruct_GroupMember members[5];
	Substruct_GroupMemberMapLocation locations[5];
	uint8_t alignFiller[3];
};

class Substruct_RaidSheet : public PacketEncodedData {
public:
	Substruct_RaidSheet(uint32_t ver) : PacketEncodedData(ver), group_leaders{ -1,-1,-1,-1 } {
		for (int i = 0; i < 24; i++) {
			members[i].ResetVersion(ver);
		}
	}

	void RegisterElements() override {
		RescopeArrayElement(members);
		RegisterSubstruct(members)->SetCount(24);
		RescopeArrayElement(group_leaders);
		RegisterInt8(group_leaders)->SetCount(4);
	}

	int8_t group_leaders[4];
	Substruct_GroupMember members[24];
};

/*
<Struct Name="Substruct_GroupMember" ClientVersion="57048">
<Data ElementName="spawn_id" Type="int32" Size="1" />
<Data ElementName="pet_id" Type="int32" Size="1" />
<Data ElementName="hp_current" Type="sint32" Size="1" />
<Data ElementName="hp_current2" Type="sint32" Size="1" />
<Data ElementName="hp_max" Type="sint32" Size="1" />
<Data ElementName="hp_max2" Type="sint32" Size="1" />
<Data ElementName="power_current" Type="sint32" Size="1" />
<Data ElementName="power_max" Type="sint32" Size="1" />
<Data ElementName="level_current" Type="int16" Size="1" />
<Data ElementName="level_max" Type="int16" Size="1" />
<Data ElementName="name" Type="char" Size="41" />
<Data ElementName="zone" Type="char" Size="60" />
<Data ElementName="instance" Type="int8" Size="1" />
<Data ElementName="unknown3" Type="int8" Size="1" />
<Data ElementName="unknown4" Type="int16" Size="1" />
<Data ElementName="race_id" Type="int8" Size="1" />
<Data ElementName="class_id" Type="int8" Size="1" />
<Data ElementName="trauma_count" Type="int8" Size="1" />
<Data ElementName="arcane_count" Type="int8" Size="1" />
<Data ElementName="noxious_count" Type="int8" Size="1" />
<Data ElementName="elemental_count" Type="int8" Size="1" />
<Data ElementName="curse_count" Type="int8" Size="1" />
<Data ElementName="unknown5" Type="int8" Size="2" />
<Data ElementName="CoEunknown" Type="int8" Size="9" />
</Struct>
*/