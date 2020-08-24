#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

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


class OP_AllCharactersDescReplyMsg_Packet : public EQ2Packet {
public:
	OP_AllCharactersDescReplyMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		NumCharacters = 0;
		AccountID = 0;
		AccountUnknown1 = 0xFFFFFFFF;
		AccountUnknown2 = 0;
		MaxAllowedCharacters = 10;
		AccouontUnknown4 = 0;
		VeteranAdventureBonus = 0;
		VeteranTradeskillBonus = 0;
		AccountUnknown5[0] = 0xFFFFFFFF;
		AccountUnknown5[1] = 0xFFFFFFFF;
		AccountUnknown5[2] = 0xFFFFFFFF;
		AccountUnknown5[3] = 0;

		//BOL
		memset(bolUnknown1, 0, sizeof(bolUnknown1));
		bolUnknown2 = 0;
		bolUnknown3 = -1;
		bolUnknown4 = 1;
	}

	uint8_t NumCharacters;

	// Array
	struct CharacterListEntry : public PacketSubstruct {
		uint32_t entryVersion;
		uint32_t charid;
		uint32_t server_id;
		std::string name;
		uint8_t unknown;
		uint8_t race;
		uint8_t _class;
		uint8_t gender;
		uint32_t level;
		std::string zone;
		uint32_t unknown1;
		uint32_t unknown2;
		uint32_t created_date;
		uint32_t last_played;
		uint32_t unknown3;
		uint32_t unknown4;
		uint64_t unknowne11;
		uint8_t unknowne7;
		uint8_t unknowne8;
		uint8_t unknowne9;
		uint8_t unknowne10;
		uint8_t unknowne12;
		std::string unknowne13[3];
		std::string zonename2;
		std::string zonedesc;
		uint8_t unknowne14a;
		uint8_t unknowne14b;
		EQ2Color unknowne15;
		uint8_t unknowne16;
		uint8_t unknowne17;
		uint8_t unknowne18;
		uint8_t unknowne19;
		int32_t unknowne21;
		uint8_t unknowne22;
		uint8_t unknowne23;
		uint32_t unknown5;
		std::string server_name;
		uint32_t account_id;
		uint8_t unknown6[2]; // Size = "2"
		uint32_t unknown7;
		uint8_t tradeskill_class; // 887
		uint32_t tradeskill_level; //887
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

		CharacterListEntry(uint32_t p_version = 0) : PacketSubstruct(p_version, true) {
			RegisterElements();
			version = 0;
			charid = 0;
			server_id = 0;
			//name = "";
			unknown = 0;
			race = 0;
			_class = 0;
			gender = 0;
			level = 0;
			//zone = "";
			unknown1 = 0;
			unknown2 = 0;
			created_date = 0;
			last_played = 0;
			unknown3 = 57;
			unknown4 = 56;
			//zonename2 = "";
			//zonedesc = "";
			unknown5 = 0;
			//server_name = "";
			account_id = 0;
			unknown6[0] = 0;
			unknown6[1] = 1; //if not here will not display character
			unknown7 = 0;
			tradeskill_class = 0; // 887
			tradeskill_level = 0; //887
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

			//character entry versioned unknowns
			unknowne7 = 0;
			unknowne8 = 0;
			unknowne9 = 0;
			unknowne10 = 0;
			unknowne11 = -1ll;
			unknowne12 = 0;
			unknowne14a = 0;
			unknowne14b = 0;
			unknowne16 = 0;
			unknowne17 = 0;
			unknowne18 = 0;
			unknowne19 = 0;
			unknowne21 = 0;
			unknowne22 = 0;
			unknowne23 = 0;
		}

		uint8_t GetNetAppearanceVersion() {
			const uint32_t version = GetVersion();
			if (version >= 67650) {
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

		uint32_t GetCharacterEntryVersion() {
			const uint32_t version = GetVersion();
			if (version >= 67650) {
				entryVersion = 23;
			}
			else if (version >= 60114) {
				entryVersion = 18;
			}
			else if (version >= 1193) {
				entryVersion = 13;
			}
			else if (version > 283) {
				entryVersion = 6;
			}
			else {
				entryVersion = 5;
			}
			return entryVersion;
		}

		void RegisterElements() {
			if (version > 283) {
				RegisterUInt32(entryVersion);
			}

			//Just adding these const versions so the compiler can optimize the checks better
			const uint32_t version = GetVersion();
			const uint8_t appVersion = GetNetAppearanceVersion();
			const uint32_t entryVersion = GetCharacterEntryVersion();

			RegisterUInt32(charid);
			if (entryVersion >= 11) {
				RegisterUInt64(unknowne11);
			}
			RegisterUInt32(server_id);
			Register16String(name);
			if (version > 283) {
				RegisterUInt8(unknown);
			}
			RegisterUInt8(race);
			RegisterUInt8(_class);
			if (version > 283) {
				RegisterUInt8(gender);
			}
			RegisterUInt32(level);
			Register16String(zone);
			RegisterUInt32(unknown1);
			RegisterUInt32(unknown2);
			RegisterUInt32(created_date);
			RegisterUInt32(last_played);
			RegisterUInt32(unknown3);
			RegisterUInt32(unknown4);
			Register16String(zonename2);
			Register16String(zonedesc);
			if (version > 283) {
				RegisterUInt32(unknown5);
				Register16String(server_name);
				RegisterUInt32(account_id);
				uint8_t& Unknown6 = unknown6[0]; // Size = "2"
				RegisterUInt8(Unknown6)->SetCount(2);
				RegisterUInt32(unknown7);
				RegisterUInt8(tradeskill_class); // 887
				RegisterUInt32(tradeskill_level); //887
				if (entryVersion >= 7) RegisterUInt8(unknowne7);
				if (entryVersion >= 8) RegisterUInt8(unknowne8);
				if (entryVersion >= 9) RegisterUInt8(unknowne9);
				if (entryVersion >= 10) RegisterUInt8(unknowne10);
			}
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

			if (entryVersion >= 12) RegisterUInt8(unknowne12);
			if (entryVersion >= 13) {
				RescopeArrayElement(unknowne13);
				Register16String(unknowne13)->SetCount(3);
			}
			if (entryVersion >= 14) {
				RegisterUInt8(unknowne14a);
				RegisterUInt8(unknowne14b);
			}
			if (entryVersion >= 15) RegisterEQ2Color(unknowne15);
			if (entryVersion >= 16) RegisterUInt8(unknowne16);
			if (entryVersion >= 17) RegisterUInt8(unknowne17);
			if (entryVersion >= 18) RegisterUInt8(unknowne18);
			if (entryVersion >= 19) RegisterUInt8(unknowne19);
			if (entryVersion >= 21) RegisterInt32(unknowne21);
			if (entryVersion >= 22) RegisterUInt8(unknowne22);
			if (entryVersion >= 23) RegisterUInt8(unknowne23);
		}
	};
	std::vector<CharacterListEntry> CharacterList;
	// account info
	uint32_t AccountID;
	uint32_t AccountUnknown1;
	uint16_t AccountUnknown2;
	uint32_t MaxAllowedCharacters;// AccountUnknown3;
	uint8_t AccouontUnknown4;
	uint32_t AccountUnknown5[4];
	uint8_t VeteranAdventureBonus; // 1096
	uint8_t VeteranTradeskillBonus; // 1096
	int32_t bolUnknown1[2];
	uint8_t bolUnknown2;
	int32_t bolUnknown3;
	uint8_t bolUnknown4;


private:
	void RegisterElements() {
		PacketUInt8* asize = RegisterUInt8(NumCharacters);
		asize->SetMyArray(RegisterArray(CharacterList, CharacterListEntry));
		// account info
		RegisterUInt32(AccountID);
		if (GetVersion() > 283) {
			RegisterUInt32(AccountUnknown1);
			RegisterUInt16(AccountUnknown2);
		}
		RegisterUInt32(MaxAllowedCharacters);
		if (GetVersion() > 283) {
			RegisterUInt8(AccouontUnknown4);
			uint32_t& accountUnknown5 = AccountUnknown5[0];
			RegisterUInt32(accountUnknown5)->SetCount(4);
			if (GetVersion() >= 1096) {
				RegisterUInt8(VeteranAdventureBonus);
				RegisterUInt8(VeteranTradeskillBonus);
				if (GetVersion() >= 67650) {
					RescopeArrayElement(bolUnknown1);
					RegisterInt32(bolUnknown1)->SetCount(2);
					RegisterUInt8(bolUnknown2);
					RegisterInt32(bolUnknown3);
					RegisterUInt8(bolUnknown4);
				}
			}
		}
	}
};

// These are not complete structs as the old login just shoved this in the middle of other data,
// these are just the character array portion of the packet
/*
<Struct Name="CharSelectProfile" ClientVersion="1">
<Data ElementName="version" Type="int32" Size="1" />
<Data ElementName="charid" Type="int32" Size="1" />
<Data ElementName="server_id" Type="int32" Size="1" />
<Data ElementName="name" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown" Type="int8" Size="1" />
<Data ElementName="race" Type="int8" Size="1" />
<Data ElementName="class" Type="int8" Size="1" />
<Data ElementName="gender" Type="int8" Size="1" />
<Data ElementName="level" Type="int32" Size="1" />
<Data ElementName="zone" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown1" Type="int32" Size="1" />
<Data ElementName="unknown2" Type="int32" Size="1" />
<Data ElementName="created_date" Type="int32" Size="1" />
<Data ElementName="last_played" Type="int32" Size="1" />
<Data ElementName="unknown3" Type="int32" Size="1" />
<Data ElementName="unknown4" Type="int32" Size="1" />
<Data ElementName="zonename2" Type="EQ2_16BitString" Size="1" />
<Data ElementName="zonedesc" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown5" Type="int32" Size="1" />
<Data ElementName="server_name" Type="EQ2_16BitString" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="unknown6" Type="int8" Size="2" />
<Data ElementName="unknown7" Type="int32" Size="1" />
<Data ElementName="unknown8" Type="int8" Size="1" />
<Data ElementName="race_type" Type="int16" Size="1" />
<Data ElementName="skin_color" Type="EQ2_Color" />
<Data ElementName="eye_color" Type="EQ2_Color" />
<Data ElementName="equip" Type="EQ2_EquipmentItem" Size="25" />
<Data ElementName="hair_type" Type="int16" Size="1" />
<Data ElementName="hair_type_color" Type="EQ2_Color" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" />
<Data ElementName="hair_face_type" Type="int16" Size="1" />
<Data ElementName="hair_face_color" Type="EQ2_Color" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" />
<Data ElementName="wing_type" Type="int16" Size="1" />
<Data ElementName="wing_color1" Type="EQ2_Color" />
<Data ElementName="wing_color2" Type="EQ2_Color" />
<Data ElementName="chest_type" Type="int16" Size="1" />
<Data ElementName="shirt_color" Type="EQ2_Color" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" />
<Data ElementName="legs_type" Type="int16" Size="1" />
<Data ElementName="pants_color" Type="EQ2_Color" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" />
<Data ElementName="unknown9" Type="EQ2_Color" />
<Data ElementName="eye_type" Type="sint8" Size="3" />
<Data ElementName="ear_type" Type="sint8" Size="3" />
<Data ElementName="eye_brow_type" Type="sint8" Size="3" />
<Data ElementName="cheek_type" Type="sint8" Size="3" />
<Data ElementName="lip_type" Type="sint8" Size="3" />
<Data ElementName="chin_type" Type="sint8" Size="3" />
<Data ElementName="nose_type" Type="sint8" Size="3" />
<Data ElementName="body_size" Type="sint8" Size="1" />
<Data ElementName="unknown10" Type="int8" Size="9" />
<Data ElementName="hair_color1" Type="EQ2_Color" />
<Data ElementName="hair_color2" Type="EQ2_Color" />
<Data ElementName="unknown11" Type="int8" Size="13" />
<Data ElementName="soga_race_type" Type="int16" Size="1" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="Unknown12" Type="int8" Size="3" />
<Data ElementName="soga_eye_type" Type="sint8" Size="3" />
<Data ElementName="soga_ear_type" Type="sint8" Size="3" />
<Data ElementName="soga_eye_brow_type" Type="sint8" Size="3" />
<Data ElementName="soga_cheek_type" Type="sint8" Size="3" />
<Data ElementName="soga_lip_type" Type="sint8" Size="3" />
<Data ElementName="soga_chin_type" Type="sint8" Size="3" />
<Data ElementName="soga_nose_type" Type="sint8" Size="3" />
<Data ElementName="unknown13" Type="int16" Size="1" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color"  />
<Data ElementName="soga_hair_color2" Type="EQ2_Color"  />
<Data ElementName="unknown14" Type="EQ2_Color" />
<Data ElementName="soga_hair_type" Type="int16" Size="1" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_face_type" Type="int16" Size="1" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" />
<Data ElementName="unknown15" Type="int8" Size="7" />
</Struct>
<Struct Name="CharSelectProfile" ClientVersion="887">
<Data ElementName="version" Type="int32" Size="1" />
<Data ElementName="charid" Type="int32" Size="1" />
<Data ElementName="server_id" Type="int32" Size="1" />
<Data ElementName="name" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown" Type="int8" Size="1" />
<Data ElementName="race" Type="int8" Size="1" />
<Data ElementName="class" Type="int8" Size="1" />
<Data ElementName="gender" Type="int8" Size="1" />
<Data ElementName="level" Type="int32" Size="1" />
<Data ElementName="zone" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown1" Type="int32" Size="1" />
<Data ElementName="unknown2" Type="int32" Size="1" />
<Data ElementName="created_date" Type="int32" Size="1" />
<Data ElementName="last_played" Type="int32" Size="1" />
<Data ElementName="unknown3" Type="int32" Size="1" />
<Data ElementName="unknown4" Type="int32" Size="1" />
<Data ElementName="zonename2" Type="EQ2_16BitString" Size="1" />
<Data ElementName="zonedesc" Type="EQ2_16BitString" Size="1" />
<Data ElementName="unknown5" Type="int32" Size="1" />
<Data ElementName="server_name" Type="EQ2_16BitString" Size="1" />
<Data ElementName="account_id" Type="int32" Size="1" />
<Data ElementName="unknown6" Type="int8" Size="2" />
<Data ElementName="unknown7" Type="int32" Size="1" />
<Data ElementName="tradeskill_class" Type="int8" Size="1" />
<Data ElementName="tradeskill_level" Type="int32" Size="1" />
<Data ElementName="unknown8" Type="int8" Size="1" />
<Data ElementName="race_type" Type="int16" Size="1" />
<Data ElementName="skin_color" Type="EQ2_Color" />
<Data ElementName="eye_color" Type="EQ2_Color" />
<Data ElementName="equip" Type="EQ2_EquipmentItem" Size="25" />
<Data ElementName="hair_type" Type="int16" Size="1" />
<Data ElementName="hair_type_color" Type="EQ2_Color" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" />
<Data ElementName="hair_face_type" Type="int16" Size="1" />
<Data ElementName="hair_face_color" Type="EQ2_Color" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" />
<Data ElementName="wing_type" Type="int16" Size="1" />
<Data ElementName="wing_color1" Type="EQ2_Color" />
<Data ElementName="wing_color2" Type="EQ2_Color" />
<Data ElementName="chest_type" Type="int16" Size="1" />
<Data ElementName="shirt_color" Type="EQ2_Color" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" />
<Data ElementName="legs_type" Type="int16" Size="1" />
<Data ElementName="pants_color" Type="EQ2_Color" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" />
<Data ElementName="unknown9" Type="EQ2_Color" />
<Data ElementName="eye_type" Type="sint8" Size="3" />
<Data ElementName="ear_type" Type="sint8" Size="3" />
<Data ElementName="eye_brow_type" Type="sint8" Size="3" />
<Data ElementName="cheek_type" Type="sint8" Size="3" />
<Data ElementName="lip_type" Type="sint8" Size="3" />
<Data ElementName="chin_type" Type="sint8" Size="3" />
<Data ElementName="nose_type" Type="sint8" Size="3" />
<Data ElementName="body_size" Type="sint8" Size="1" />
<Data ElementName="unknown10" Type="int8" Size="9" />
<Data ElementName="hair_color1" Type="EQ2_Color" />
<Data ElementName="hair_color2" Type="EQ2_Color" />
<Data ElementName="unknown11" Type="int8" Size="13" />
<Data ElementName="soga_race_type" Type="int16" Size="1" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="Unknown12" Type="int8" Size="3" />
<Data ElementName="soga_eye_type" Type="sint8" Size="3" />
<Data ElementName="soga_ear_type" Type="sint8" Size="3" />
<Data ElementName="soga_eye_brow_type" Type="sint8" Size="3" />
<Data ElementName="soga_cheek_type" Type="sint8" Size="3" />
<Data ElementName="soga_lip_type" Type="sint8" Size="3" />
<Data ElementName="soga_chin_type" Type="sint8" Size="3" />
<Data ElementName="soga_nose_type" Type="sint8" Size="3" />
<Data ElementName="unknown13" Type="int16" Size="1" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color"  />
<Data ElementName="soga_hair_color2" Type="EQ2_Color"  />
<Data ElementName="unknown14" Type="EQ2_Color" />
<Data ElementName="soga_hair_type" Type="int16" Size="1" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_face_type" Type="int16" Size="1" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" />
<Data ElementName="unknown15" Type="int8" Size="7" />
</Struct>
*/