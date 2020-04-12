#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

class Substruct_PassiveEffects : public PacketSubstruct {
public:
	Substruct_PassiveEffects(uint32_t ver = 0)
		: PacketSubstruct(ver) {

		spell_id = 0xFFFFFFFF;
		total_time = 0;
		expire_timestamp = 0;
		icon = 0;
		icon_type = 0;
		unknown2 = 0;
		memset(CoEunknown, 0, sizeof(CoEunknown));
		category = 0;
		category2 = 0;
	}

	uint32_t spell_id;
	float total_time;
	uint32_t expire_timestamp;
	uint16_t icon;
	uint16_t icon_type;
	uint16_t unknown2;
	uint8_t CoEunknown[13];
	uint32_t category;
	uint32_t category2;

private:
	void RegisterElements() {
		RegisterUInt32(spell_id);
		RegisterFloat(total_time);
		RegisterUInt32(expire_timestamp);
		RegisterUInt16(icon);
		RegisterUInt16(icon_type);
		RegisterUInt16(unknown2);
		RescopeArrayElement(CoEunknown);
		RegisterUInt8(CoEunknown)->SetCount(13);
		RegisterUInt32(category);
		RegisterUInt32(category2);
	}

};

/*
<Struct Name="Substruct_PassiveEffects" ClientVersion="1193">
<Data ElementName="spell_id" Type="int32" Size="1" />
<Data ElementName="total_time" Type="float" Size="1" />
<Data ElementName="expire_timestamp" Type="int32" Size="1" />
<Data ElementName="icon" Type="int16" Size="1" />
<Data ElementName="icon_type" Type="int16" Size="1" />
<Data ElementName="unknown2" Type="int16" Size="1" />
<Data ElementName="CoEunknown" Type="int8" Size="13" />
<Data ElementName="category" Type="int32" Size="1" />
<Data ElementName="category2" Type="int32" Size="1" />
</Struct>
*/