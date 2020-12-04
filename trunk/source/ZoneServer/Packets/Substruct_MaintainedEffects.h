#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"

class Substruct_MaintainedEffects : public PacketSubstruct {
public:
	Substruct_MaintainedEffects(uint32_t ver = 0)
		: PacketSubstruct(ver) {

		memset(name, 0, sizeof(name));
		target = 0;
		target_type = 0;
		spell_id = 0xFFFFFFFF;
		slot_pos = 0;
		icon = 0xFFFF;
		icon_type = 0;
		unknown3 = 0;
		conc_used = 0;
		total_time = 0;
		expire_timestamp = 0;
		stack_count = 0;
		amount_remaining = 0;
		flags = 0;
	}

	char name[60];
	uint32_t target;
	uint8_t target_type;
	uint32_t spell_id;
	uint32_t slot_pos;
	uint16_t icon;
	uint16_t icon_type;
	uint8_t unknown3;
	uint8_t conc_used;
	float total_time;
	uint32_t expire_timestamp;
	int16_t stack_count;
	int16_t amount_remaining;
	uint16_t flags;

private:
	void RegisterElements() {
		RescopeArrayElement(name);
		RegisterChar(name)->SetCount(60);
		RegisterUInt32(target);
		RegisterUInt8(target_type);
		RegisterUInt32(spell_id);
		RegisterUInt32(slot_pos);
		RegisterUInt16(icon);
		RegisterUInt16(icon_type);
		RegisterUInt8(unknown3);
		RegisterUInt8(conc_used);
		RegisterFloat(total_time);
		RegisterUInt32(expire_timestamp);
		if (GetVersion() >= 1193) {
			RegisterInt16(stack_count);
			RegisterInt16(amount_remaining);
			if (GetVersion() >= 57048) {
				RegisterUInt16(flags);
			}
		}
	}

};

/*
<Struct Name="Substruct_MaintainedEffects" ClientVersion="57048">
<Data ElementName="name" Type="char" Size="60" />
<Data ElementName="target" Type="int32" Size="1" />
<Data ElementName="target_type" Type="int8" Size="1" />
<Data ElementName="spell_id" Type="int32" Size="1" />
<Data ElementName="slot_pos" Type="int32" Size="1" />
<Data ElementName="icon" Type="int16" Size="1" />
<Data ElementName="icon_type" Type="int16" Size="1" />
<Data ElementName="unknown3" Type="int8" Size="1" />
<Data ElementName="conc_used" Type="int8" Size="1" />
<Data ElementName="total_time" Type="float" Size="1" />
<Data ElementName="expire_timestamp" Type="int32" Size="1" />
<Data ElementName="CoEunknown" Type="int8" Size="6" />
</Struct>
*/