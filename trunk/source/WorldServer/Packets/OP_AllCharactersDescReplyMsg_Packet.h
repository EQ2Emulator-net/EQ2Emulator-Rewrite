#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/Substruct_NetAppearance.h"


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
		Substruct_NetAppearance appearance;

		CharacterListEntry(uint32_t p_version = 0) : PacketSubstruct(p_version, true), appearance(p_version) {
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
			
			RegisterSubstruct(appearance);

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