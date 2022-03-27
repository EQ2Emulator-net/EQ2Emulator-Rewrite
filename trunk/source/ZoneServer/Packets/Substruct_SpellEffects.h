#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

class Substruct_SpellEffects : public PacketSubstruct {
public:
	Substruct_SpellEffects(uint32_t ver = 0)
		: PacketSubstruct(ver) {

		spell_id = 0xFFFFFFFF;
		total_time = 0;
		expire_timestamp = 0;
		icon = 0;
		icon_type = 0;
		unknown2 = 0;
		cancellable = 0;
		memset(CoEunknown, 0, sizeof(CoEunknown));
	}

	uint32_t spell_id;
	float total_time;
	uint32_t expire_timestamp;
	uint16_t icon;
	uint16_t icon_type;
	uint8_t unknown2;
	uint8_t unknown3;
	uint8_t cancellable;
	uint8_t CoEunknown[21];

private:
	void RegisterElements() {
		RegisterUInt32(spell_id);
		RegisterFloat(total_time);
		RegisterUInt32(expire_timestamp);
		RegisterUInt16(icon);
		RegisterUInt16(icon_type);
		RegisterUInt8(unknown2);
		RegisterUInt8(cancellable);
		if (GetVersion() < 839) {
			RegisterUInt8(unknown3);
		}
		if (GetVersion() >= 1193) {
			RescopeArrayElement(CoEunknown);
			RegisterUInt8(CoEunknown)->SetCount(21);
		}
	}

};

/*
<Struct Name="Substruct_SpellEffects" ClientVersion="1193">
<Data ElementName="spell_id" Type="int32" Size="1" />
<Data ElementName="total_time" Type="float" Size="1" />
<Data ElementName="expire_timestamp" Type="int32" Size="1" />
<Data ElementName="icon" Type="int16" Size="1" />
<Data ElementName="icon_type" Type="int16" Size="1" />
<Data ElementName="unknown2" Type="int8" Size="1" />
<Data ElementName="cancellable" Type="int8" Size="1" />
<Data ElementName="CoEunknown" Type="int8" Size="21" />
</Struct>
*/