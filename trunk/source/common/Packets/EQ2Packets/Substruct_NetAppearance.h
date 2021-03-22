#pragma once

#include "../PacketElements/PacketSubstruct.h"

struct SpawnMorphSliders {
	int8_t skull[3];
	int8_t eyes[3];
	int8_t ears[3];
	int8_t eyebrow[3];
	int8_t cheeks[3];
	int8_t mouth[3];
	int8_t chin[3];
	int8_t nose[3];
	int8_t bodyscale;
	int8_t bumpscale;
};

struct SpawnFloatMorphSliders {
	EQ2ColorFloat skull;
	EQ2ColorFloat eyes;
	EQ2ColorFloat ears;
	EQ2ColorFloat eyebrow;
	EQ2ColorFloat cheeks;
	EQ2ColorFloat mouth;
	EQ2ColorFloat chin;
	EQ2ColorFloat nose;
	float bodyscale;
	float bumpscale;
};

class Substruct_NetAppearance : public PacketSubstruct {
public:
	Substruct_NetAppearance(uint32_t ver = 0) : PacketSubstruct(ver) {
		race_type = 0;

		memset(sliderBytes, 0, 26);
		memset(sogaSliderBytes, 0, 26);

		combat_voice = 0;
		emote_voice = 0;
		unkType1 = 0;

		soga_race_type = 0;
		info_vis_flags = 0;

		memset(unknown15, 0, sizeof(unknown15));
		memset(unknown18, 0, sizeof(unknown18));
	}

	~Substruct_NetAppearance() = default;

	uint8_t netAppearanceVersion;
	uint32_t race_type;
	EQ2Color skin_color;
	EQ2Color eye_color;
	EQ2Color model_color;
	EQ2EquipmentItem equip[24]; // " Type = "EQ2_EquipmentItem" Size = "25" / >
	EQ2EquipmentItem texture_slot_item;
	EQ2EquipmentItem hair;
	EQ2EquipmentItem hair_face;
	EQ2EquipmentItem wings;
	EQ2EquipmentItem shirt;
	EQ2EquipmentItem pants;
	EQ2EquipmentItem tail;
	union {
		SpawnMorphSliders sliders;
		int8_t sliderBytes[26];
	};
	EQ2Color hair_color1;
	EQ2Color hair_color2;
	EQ2Color hair_scatter;
	uint32_t emote_voice;
	uint32_t combat_voice;
	uint32_t unkType1;
	uint32_t info_vis_flags;
	uint32_t soga_race_type;
	EQ2Color soga_skin_color;
	EQ2Color soga_eye_color;
	EQ2Color soga_model_color;
	EQ2EquipmentItem back_slot_item;
	EQ2EquipmentItem mount_adornment_slot_item;
	EQ2EquipmentItem mount_armor_slot_item;
	union {
		SpawnMorphSliders sogaSliders;
		int8_t sogaSliderBytes[26];
	};
	EQ2Color soga_hair_color1;
	EQ2Color soga_hair_color2;
	EQ2Color soga_hair_scatter;
	EQ2EquipmentItem soga_hair;
	EQ2EquipmentItem soga_hair_face;
	uint8_t unknown15[7]; // " Type = "int8" Size = "7" / >
	uint8_t unknown18[7]; //maybe soga version of unknown15?
	EQ2EquipmentItem mount;
	EQ2Color mountColor1;
	EQ2Color mountColor2;

	uint8_t GetNetAppearanceVersion() {
		const uint32_t version = GetVersion();
		if (version >= 67804) {
			netAppearanceVersion = 24;
		}
		else if (version >= 67650) {
			netAppearanceVersion = 23;
		}
		else if (version >= 60114) {
			netAppearanceVersion = 22;
		}
		else if (version >= 1193) {
			netAppearanceVersion = 20;
		}
		else if (version > 283) {
			netAppearanceVersion = 16;
		}
		else {
			netAppearanceVersion = 5;
		}
		return netAppearanceVersion;
	}

	void RegisterElements() override {
		const uint8_t appVersion = GetNetAppearanceVersion();

		RegisterUInt8(netAppearanceVersion);
		if (appVersion < 21) {
			RescopeToReference(race_type, uint16_t);
			RegisterUInt16(race_type);
		}
		else {
			RegisterUInt32(race_type);//netapp ver 21
		}

		RegisterEQ2Color(skin_color);
		RegisterEQ2Color(eye_color);
		if (appVersion >= 16) {
			RegisterEQ2Color(model_color);
		}
		EQ2EquipmentItem& Equip = equip[0];
		RegisterEQ2EquipmentItem(Equip, appVersion < 21)->SetCount(24);
		if (appVersion >= 20) {
			RegisterEQ2EquipmentItem(mount_adornment_slot_item, appVersion < 21);
			RegisterEQ2EquipmentItem(mount_armor_slot_item, appVersion < 21);
		}
		RegisterEQ2EquipmentItem(texture_slot_item, appVersion < 21);

		if (version <= 283) {
			RescopeArrayElement(sliderBytes);
			RegisterInt8(sliderBytes)->SetCount(26);
			RegisterEQ2EquipmentItem(mount, appVersion < 21);
			RegisterEQ2Color(hair_color1);
			RegisterEQ2Color(hair_color2);
			RegisterEQ2Color(hair_scatter);
			RescopeToReference(info_vis_flags, uint8_t);
			RegisterUInt8(info_vis_flags);
			return;
		}

		RegisterEQ2EquipmentItem(hair, appVersion < 21);
		RegisterEQ2EquipmentItem(hair_face, appVersion < 21);
		RegisterEQ2EquipmentItem(wings, appVersion < 21);
		if (appVersion >= 24) {
			RegisterEQ2EquipmentItem(tail, appVersion < 21);
		}
		RegisterEQ2EquipmentItem(shirt, appVersion < 21);
		RegisterEQ2EquipmentItem(pants, appVersion < 21);
		if (appVersion >= 17) {
			RegisterEQ2EquipmentItem(back_slot_item, appVersion < 21);
		}
		RescopeArrayElement(sliderBytes);
		RegisterInt8(sliderBytes)->SetCount(26);
		RegisterEQ2EquipmentItem(mount, appVersion < 21);
		RegisterEQ2Color(hair_color1);
		RegisterEQ2Color(hair_color2);
		RegisterEQ2Color(hair_scatter);
		if (appVersion < 22) {
			RescopeToReference(combat_voice, uint16_t);
			RegisterUInt16(combat_voice);
			RescopeToReference(emote_voice, uint16_t);
			RegisterUInt16(emote_voice);
		}
		else {
			RegisterUInt32(combat_voice);//netapp ver 22
			RegisterUInt32(emote_voice);
		}
		if (appVersion < 21) {
			RescopeToReference(unkType1, uint16_t);
			RegisterUInt16(unkType1);
		}
		else {
			RegisterUInt32(unkType1);//netapp ver 21
		}
		//Vis flags and interaction bitflags? (might be 4 bytes in that struct as well)
		RegisterUInt32(info_vis_flags);
		if (appVersion < 21) {
			RescopeToReference(soga_race_type, uint16_t);
			RegisterUInt16(soga_race_type);
		}
		else {
			RegisterUInt32(soga_race_type);//netapp ver 21
		}
		RegisterEQ2Color(soga_skin_color);
		RegisterEQ2Color(soga_eye_color);
		if (appVersion >= 16) {
			RegisterEQ2Color(soga_model_color);
		}
		RescopeArrayElement(sogaSliderBytes);
		RegisterInt8(sogaSliderBytes)->SetCount(26);
		RegisterEQ2Color(soga_hair_color1);
		RegisterEQ2Color(soga_hair_color2);
		RegisterEQ2Color(soga_hair_scatter);
		RegisterEQ2EquipmentItem(soga_hair, appVersion < 21);
		RegisterEQ2EquipmentItem(soga_hair_face, appVersion < 21);
		uint8_t& Unknown15 = unknown15[0]; // " Type = "int8" Size = "7" / >
		RegisterUInt8(Unknown15)->SetCount(7);

		if (appVersion >= 18) {
			RescopeArrayElement(unknown18);
			RegisterUInt8(unknown18)->SetCount(7);
		}
	}
};