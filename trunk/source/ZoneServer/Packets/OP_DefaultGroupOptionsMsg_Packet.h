#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_DefaultGroupOptionsMsg_Packet : public EQ2Packet {
public:
	OP_DefaultGroupOptionsMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		loot_method = 0;
		loot_items_rarity = 0;
		auto_split_coin = 0;
		unknown3 = 0;
		default_yell_method = 0;
		unknown5 = 0;
		group_autolock = 0;
		solo_autolock = 0;
		unknown8 = 0;
	}

	uint8_t loot_method;
	uint8_t loot_items_rarity;
	uint8_t auto_split_coin;
	uint8_t unknown3;
	uint8_t default_yell_method;
	uint8_t unknown5;
	uint8_t group_autolock;
	uint8_t solo_autolock;
	uint8_t unknown8;

private:
	void RegisterElements() {
		RegisterUInt8(loot_method);
		RegisterUInt8(loot_items_rarity);
		RegisterUInt8(auto_split_coin);
		RegisterUInt8(unknown3);
		RegisterUInt8(default_yell_method);
		RegisterUInt8(unknown5);
		RegisterUInt8(group_autolock);
		RegisterUInt8(solo_autolock);
		RegisterUInt8(unknown8);
	}

};

/*
<Struct Name="WS_DefaultGroupOptions" ClientVersion="1" OpcodeName="OP_DefaultGroupOptionsMsg">
<Data ElementName="loot_method" Type="int8" Size="1" />
<Data ElementName="loot_items_rarity" Type="int8" Size="1" />
<Data ElementName="auto_split_coin" Type="int8" Size="1" />
<Data ElementName="unknown3" Type="int8" Size="1" />
<Data ElementName="default_yell_method" Type="int8" Size="1" />
<Data ElementName="unknown5" Type="int8" Size="1" />
<Data ElementName="group_autolock" Type="int8" Size="1" />
<Data ElementName="solo_autolock" Type="int8" Size="1" />
<Data ElementName="unknown8" Type="int8" Size="1" />
</Struct>
*/
