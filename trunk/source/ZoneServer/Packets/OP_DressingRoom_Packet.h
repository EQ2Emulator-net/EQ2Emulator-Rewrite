#pragma once

#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/Substruct_NetAppearance.h"
#include <string>

class OP_DressingRoom_Packet : public EQ2Packet {
public:
	OP_DressingRoom_Packet(uint32_t ver) : EQ2Packet(ver) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt16(unk1);
		//type 4 is net appearance, type 3 with model_type == -1 is also net appearance
		//type 1 = changing mount, type 3 = changing main model 
		RegisterUInt16(type);
		RegisterUInt32(vis_state);
		RegisterUInt32(vis_state2);
		RegisterUInt32(appearance_type);
	}

	EQ2Packet* GetSubPacket() override;

	uint16_t unk1;
	uint16_t type;
	uint32_t appearance_type;
	uint32_t vis_state;
	uint32_t vis_state2;
};

class DressingRoom_NetAppearance_Packet : public OP_DressingRoom_Packet {
public:
	DressingRoom_NetAppearance_Packet(uint32_t ver) : OP_DressingRoom_Packet(ver), netApp(ver) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterSubstruct(netApp);
	}

	EQ2Packet* GetSubPacket() override { return nullptr; }

	Substruct_NetAppearance netApp;
};

class DressingRoom_Standard_Packet : public OP_DressingRoom_Packet {
public:
	DressingRoom_Standard_Packet(uint32_t ver) : OP_DressingRoom_Packet(ver) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt16(slot);
		RegisterUInt8(unknown4a);
		RegisterBool(b2hWeapon);
		RegisterEQ2ColorFloat(color);
		RegisterEQ2ColorFloat(highlight);
		RegisterBool(bEquipment);
		RegisterBool(b1hWeapon);
		RegisterUInt16(icon);
		RegisterUInt32(item_id);
		RegisterUInt32(item_crc);
		RegisterUInt32(item_id2);
		RegisterUInt32(unk_type);
		RescopeArrayElement(heraldry);
		RegisterUInt8(heraldry)->SetCount(7);
		RegisterUInt32(mount_type);
		RegisterUInt8(housePlacementType);
	}

	EQ2Packet* GetSubPacket() override { return nullptr; }

	uint16_t slot;
	bool b2hWeapon;
	uint8_t unknown4a;
	EQ2ColorFloat color;
	EQ2ColorFloat highlight;
	bool bEquipment;
	bool b1hWeapon;
	uint16_t icon;
	//id+crc are not always sent!
	uint32_t item_id;
	uint32_t item_crc;
	uint32_t item_id2;
	uint32_t unk_type;
	//BG Color, Emblem, Emblem Color, Edge, Edge Color, pattern/color (unknown order)
	uint8_t heraldry[7];
	uint32_t mount_type;
	uint8_t housePlacementType;
};