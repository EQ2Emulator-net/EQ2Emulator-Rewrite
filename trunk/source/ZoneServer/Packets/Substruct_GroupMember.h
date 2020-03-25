#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

class Substruct_GroupMember : public PacketSubstruct {
public:
	Substruct_GroupMember()
		: PacketSubstruct(0) {
		RegisterElements();

		spawn_id = 0;
		pet_id = 0;
		hp_current = 0;
		hp_current2 = 0;
		hp_max = 0;
		hp_max2 = 0;
		power_current = 0;
		power_max = 0;
		level_current = 0;
		level_max = 0;
		memset(name, 0, sizeof(name));
		memset(zone, 0, sizeof(zone));
		instance = 0;
		unknown3 = 0;
		unknown4 = 0;
		race_id = 0;
		class_id = 0;
		trauma_count = 0;
		arcane_count = 0;
		noxious_count = 0;
		elemental_count = 0;
		curse_count = 0;
		memset(unknown5, 0, sizeof(unknown5));
		memset(CoEunknown, 0, sizeof(CoEunknown));
	}

	uint32_t spawn_id;
	uint32_t pet_id;
	int32_t hp_current;
	int32_t hp_current2;
	int32_t hp_max;
	int32_t hp_max2;
	int32_t power_current;
	int32_t power_max;
	uint16_t level_current;
	uint16_t level_max;
	char name[41];
	char zone[60];
	uint8_t instance;
	uint8_t unknown3;
	uint16_t unknown4;
	uint8_t race_id;
	uint8_t class_id;
	uint8_t trauma_count;
	uint8_t arcane_count;
	uint8_t noxious_count;
	uint8_t elemental_count;
	uint8_t curse_count;
	uint8_t unknown5[2];
	uint8_t CoEunknown[9];

private:
	void RegisterElements() {
		RegisterUInt32(spawn_id);
		RegisterUInt32(pet_id);
		RegisterInt32(hp_current);
		RegisterInt32(hp_current2);
		RegisterInt32(hp_max);
		RegisterInt32(hp_max2);
		RegisterInt32(power_current);
		RegisterInt32(power_max);
		RegisterUInt16(level_current);
		RegisterUInt16(level_max);
		// Error parsing the struct, unknown is an unknown type(name);
		char& Name = name[0];
		RegisterChar(Name)->SetCount(41);

		// Error parsing the struct, unknown is an unknown type(zone);
		char& Zone = zone[0];
		RegisterChar(Zone)->SetCount(60);

		RegisterUInt8(instance);
		RegisterUInt8(unknown3);
		RegisterUInt16(unknown4);
		RegisterUInt8(race_id);
		RegisterUInt8(class_id);
		RegisterUInt8(trauma_count);
		RegisterUInt8(arcane_count);
		RegisterUInt8(noxious_count);
		RegisterUInt8(elemental_count);
		RegisterUInt8(curse_count);

		uint8_t& Unknown5 = unknown5[0];
		RegisterUInt8(Unknown5)->SetCount(2);

		uint8_t& COEunknown = CoEunknown[0];
		RegisterUInt8(COEunknown)->SetCount(9);
	}

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