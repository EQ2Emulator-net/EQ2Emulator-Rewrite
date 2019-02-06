#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"


class OP_AllCharactersDescReplyMsg_Packet : public EQ2Packet {
public:
	OP_AllCharactersDescReplyMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();

		NumCharacters = 0;
		AccountID = 0;
		AccountUnknown1 = 0xFFFFFFFF;
		AccountUnknown2 = 0;
		AccountUnknown3 = 10;
		AccouontUnknown4 = 0;
		VeteranAdventureBonus = 0;
		VeteranTradeskillBonus = 0;
		AccountUnknown5[0] = 0xFFFFFFFF;
		AccountUnknown5[1] = 0xFFFFFFFF;
		AccountUnknown5[2] = 0xFFFFFFFF;
		AccountUnknown5[3] = 0;
	}

	uint8_t NumCharacters;

	// Array
	struct CharacterListEntry : public PacketSubstruct {
		uint32_t version;
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
		std::string zonename2;
		std::string zonedesc;
		uint32_t unknown5;
		std::string server_name;
		uint32_t account_id;
		uint8_t unknown6[2]; // Size = "2"
		uint32_t unknown7;
		uint8_t tradeskill_class; // 887
		uint32_t tradeskill_level; //887
		uint8_t unknown8;
		uint16_t race_type;
		EQ2Color skin_color;
		EQ2Color eye_color;
		EQ2EquipmentItem equip[25]; // " Type = "EQ2_EquipmentItem" Size = "25" / >
		uint16_t hair_type;
		EQ2Color hair_type_color;
		EQ2Color hair_type_highlight_color;
		uint16_t hair_face_type;
		EQ2Color hair_face_color;
		EQ2Color hair_face_highlight_color;
		uint16_t wing_type;
		EQ2Color wing_color1;
		EQ2Color wing_color2;
		uint16_t chest_type;
		EQ2Color shirt_color;
		EQ2Color unknown_chest_color;
		uint16_t legs_type;
		EQ2Color pants_color;
		EQ2Color unknown_legs_color;
		EQ2Color unknown9;
		int8_t eye_type[3]; // Size = "3"
		int8_t ear_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t eye_brow_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t cheek_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t lip_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t chin_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t nose_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t body_size;
		uint8_t unknown10[9]; // " Type = "int8" Size = "9" / >
		EQ2Color hair_color1;
		EQ2Color hair_color2;
		uint8_t unknown11[13]; // " Type = "int8" Size = "13" / >
		uint16_t soga_race_type;
		EQ2Color soga_skin_color;
		EQ2Color soga_eye_color;
		uint8_t Unknown12[3]; // " Type = "int8" Size = "3" / >
		int8_t soga_eye_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_ear_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_eye_brow_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_cheek_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_lip_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_chin_type[3]; // " Type = "sint8" Size = "3" / >
		int8_t soga_nose_type[3]; // " Type = "sint8" Size = "3" / >
		uint16_t unknown13;
		EQ2Color soga_hair_color1;
		EQ2Color soga_hair_color2;
		EQ2Color unknown14;
		uint16_t soga_hair_type;
		EQ2Color soga_hair_type_color;
		EQ2Color soga_hair_type_highlight_color;
		uint16_t soga_hair_face_type;
		EQ2Color soga_hair_face_color;
		EQ2Color soga_hair_face_highlight_color;
		uint8_t unknown15[7]; // " Type = "int8" Size = "7" / >

		CharacterListEntry() {
			RegisterElements();
			version = 0;
			charid = 0;
			server_id = 0;
			name = "";
			unknown = 0;
			race = 0;;
			_class = 0;
			gender = 0;
			level = 0;
			zone = "";
			unknown1 = 0;
			unknown2 = 0;
			created_date = 0;
			last_played = 0;
			unknown3 = 57;
			unknown4 = 56;
			zonename2 = "";
			zonedesc = "";
			unknown5 = 0;
			server_name = "";
			account_id = 0;
			unknown6[0] = 0;
			unknown6[1] = 1; //if not here will not display character
			unknown7 = 0;
			tradeskill_class = 0; // 887
			tradeskill_level = 0; //887
			unknown8 = 15;
			race_type = 0;
			skin_color.Red = 255;
			skin_color.Green = 255;
			skin_color.Blue = 255;
			eye_color.Red = 255;
			eye_color.Green = 255;
			eye_color.Blue = 255;

			for (uint8_t i = 0; i < 24; i++) {
				equip[i].type = 0;
				equip[i].color.Red = 255;
				equip[i].color.Green = 255;
				equip[i].color.Blue = 255;
				equip[i].highlight.Red = 255;
				equip[i].highlight.Green = 255;
				equip[i].highlight.Blue = 255;
			}
			hair_type = 0;
			hair_type_color.Red = 255;
			hair_type_color.Green = 255;
			hair_type_color.Blue = 255;
			hair_type_highlight_color.Red = 255;
			hair_type_highlight_color.Green = 255;
			hair_type_highlight_color.Blue = 255;
			hair_face_type = 0;
			hair_face_color.Red = 255;
			hair_face_color.Green = 255;
			hair_face_color.Blue = 255;
			hair_face_highlight_color.Red = 255;
			hair_face_highlight_color.Green = 255;
			hair_face_highlight_color.Blue = 255;
			wing_type = 0;
			wing_color1.Red = 255;
			wing_color1.Green = 255;
			wing_color1.Blue = 255;
			wing_color2.Red = 255;
			wing_color2.Green = 255;
			wing_color2.Blue = 255;
			chest_type = 0;
			shirt_color.Red = 255;
			shirt_color.Green = 255;
			shirt_color.Blue = 255;
			unknown_chest_color.Red = 255;
			unknown_chest_color.Green = 255;
			unknown_chest_color.Blue = 255;
			legs_type = 0;
			pants_color.Red = 255;
			pants_color.Green = 255;
			pants_color.Blue = 255;
			unknown_legs_color.Red = 255;
			unknown_legs_color.Green = 255;
			unknown_legs_color.Blue = 255;
			unknown9.Red = 255;
			unknown9.Green = 255;
			unknown9.Blue = 255;
			eye_type[0] = 0;
			eye_type[1] = 0;
			eye_type[2] = 0;
			ear_type[0] = 0;
			ear_type[1] = 0;
			ear_type[2] = 0;
			eye_brow_type[0] = 0;
			eye_brow_type[1] = 0;
			eye_brow_type[2] = 0;
			cheek_type[0] = 0;
			cheek_type[1] = 0;
			cheek_type[2] = 0;
			lip_type[0] = 0;
			lip_type[1] = 0;
			lip_type[2] = 0;
			chin_type[0] = 0;
			chin_type[1] = 0;
			chin_type[2] = 0;
			nose_type[0] = 0;
			nose_type[1] = 0;
			nose_type[2] = 0;
			body_size = 0;
			unknown10[0] = 0;
			unknown10[1] = 0;
			unknown10[2] = 0;
			unknown10[3] = 0;
			unknown10[4] = 0;
			unknown10[5] = 0;
			unknown10[6] = 0;
			unknown10[7] = 0;
			unknown10[8] = 0;
			hair_color1.Red = 255;
			hair_color1.Green = 255;
			hair_color1.Blue = 255;
			hair_color2.Red = 255;
			hair_color2.Green = 255;
			hair_color2.Blue = 255;
			
			unsigned char tmp[] = { 0xFF, 0xFF, 0xFF, 0x61, 0x00, 0x2C, 0x04, 0xA5, 0x09, 0x02, 0x0F, 0x00, 0x00 };
			for (int y = 0; y < sizeof(tmp); y++)
				unknown11[y] = tmp[y];

			soga_race_type = 0;
			soga_skin_color.Red = 255;
			soga_skin_color.Green = 255;
			soga_skin_color.Blue = 255;
			soga_eye_color.Red = 255;
			soga_eye_color.Green = 255;
			soga_eye_color.Blue = 255;
			Unknown12[0] = 0;
			Unknown12[1] = 0;
			Unknown12[2] = 0;
			soga_eye_type[0] = 0;
			soga_eye_type[1] = 0;
			soga_eye_type[2] = 0;
			soga_ear_type[0] = 0;
			soga_ear_type[1] = 0;
			soga_ear_type[2] = 0;
			soga_eye_brow_type[0] = 0;
			soga_eye_brow_type[1] = 0;
			soga_eye_brow_type[2] = 0;
			soga_cheek_type[0] = 0;
			soga_cheek_type[1] = 0;
			soga_cheek_type[2] = 0;
			soga_lip_type[0] = 0;
			soga_lip_type[1] = 0;
			soga_lip_type[2] = 0;
			soga_chin_type[0] = 0;
			soga_chin_type[1] = 0;
			soga_chin_type[2] = 0;
			soga_nose_type[0] = 0;
			soga_nose_type[1] = 0;
			soga_nose_type[2] = 0;
			unknown13 = 212;
			soga_hair_color1.Red = 255;
			soga_hair_color1.Green = 255;
			soga_hair_color1.Blue = 255;
			soga_hair_color2.Red = 255;
			soga_hair_color2.Green = 255;
			soga_hair_color2.Blue = 255;
			unknown14.Red = 255;
			unknown14.Green = 255;
			unknown14.Blue = 255;
			soga_hair_type = 0;
			soga_hair_type_color.Red = 255;
			soga_hair_type_color.Green = 255;
			soga_hair_type_color.Blue = 255;
			soga_hair_type_highlight_color.Red = 255;
			soga_hair_type_highlight_color.Green = 255;
			soga_hair_type_highlight_color.Blue = 255;
			soga_hair_face_type = 0;
			soga_hair_face_color.Red = 255;
			soga_hair_face_color.Green = 255;
			soga_hair_face_color.Blue = 255;
			soga_hair_face_highlight_color.Red = 255;
			soga_hair_face_highlight_color.Green = 255;
			soga_hair_face_highlight_color.Blue = 255;
			unknown15[0] = 0;
			unknown15[1] = 0;
			unknown15[2] = 0;
			unknown15[3] = 0;
			unknown15[4] = 0;
			unknown15[5] = 0;
			unknown15[6] = 0;
			
		}

		void RegisterElements() {
			RegisterUInt32(version);
			RegisterUInt32(charid);
			RegisterUInt32(server_id);
			Register16String(name);
			RegisterUInt8(unknown);
			RegisterUInt8(race);
			RegisterUInt8(_class);
			RegisterUInt8(gender);
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
			RegisterUInt32(unknown5);
			Register16String(server_name);
			RegisterUInt32(account_id);
			uint8_t& Unknown6 = unknown6[0]; // Size = "2"
			RegisterUInt8(Unknown6)->SetCount(2);
			RegisterUInt32(unknown7);
			RegisterUInt8(tradeskill_class); // 887
			RegisterUInt32(tradeskill_level); //887
			RegisterUInt8(unknown8);
			RegisterUInt16(race_type);
			RegisterEQ2Color(skin_color);
			RegisterEQ2Color(eye_color);
			EQ2EquipmentItem& Equip = equip[0]; // " Type = "EQ2_EquipmentItem" Size = "25" / >
			RegisterEQ2EquipmentItem(Equip)->SetCount(25);
			RegisterUInt16(hair_type);
			RegisterEQ2Color(hair_type_color);
			RegisterEQ2Color(hair_type_highlight_color);
			RegisterUInt16(hair_face_type);
			RegisterEQ2Color(hair_face_color);
			RegisterEQ2Color(hair_face_highlight_color);
			RegisterUInt16(wing_type);
			RegisterEQ2Color(wing_color1);
			RegisterEQ2Color(wing_color2);
			RegisterUInt16(chest_type);
			RegisterEQ2Color(shirt_color);
			RegisterEQ2Color(unknown_chest_color);
			RegisterUInt16(legs_type);
			RegisterEQ2Color(pants_color);
			RegisterEQ2Color(unknown_legs_color);
			RegisterEQ2Color(unknown9);
			int8_t& Eye_Type = eye_type[0]; // Size = "3"
			RegisterInt8(Eye_Type)->SetCount(3);
			int8_t& Ear_Type = ear_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Ear_Type)->SetCount(3);
			int8_t& Eye_Brow_Type = eye_brow_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Eye_Brow_Type)->SetCount(3);
			int8_t& Cheek_Type = cheek_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Cheek_Type)->SetCount(3);
			int8_t& Lip_Type = lip_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Lip_Type)->SetCount(3);
			int8_t& Chin_Type = chin_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Chin_Type)->SetCount(3);
			int8_t& Nose_Type = nose_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Nose_Type)->SetCount(3);
			RegisterInt8(body_size);
			uint8_t& Unknown10 = unknown10[0];
			RegisterUInt8(Unknown10)->SetCount(9); // " Type = "int8" Size = "9" / >
			RegisterEQ2Color(hair_color1);
			RegisterEQ2Color(hair_color2);
			uint8_t& Unknown11 = unknown11[0]; // " Type = "int8" Size = "13" / >
			RegisterUInt8(Unknown11)->SetCount(13);
			RegisterUInt16(soga_race_type);
			RegisterEQ2Color(soga_skin_color);
			RegisterEQ2Color(soga_eye_color);
			uint8_t& unknown12 = Unknown12[0]; // " Type = "int8" Size = "3" / >
			RegisterUInt8(unknown12)->SetCount(3);
			int8_t& Soga_Eye_Type = soga_eye_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Eye_Type)->SetCount(3);
			int8_t& Soga_Ear_Type = soga_ear_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Ear_Type)->SetCount(3);
			int8_t& Soga_Eye_Brow_Type = soga_eye_brow_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Eye_Brow_Type)->SetCount(3);
			int8_t& Soga_Cheek_Type = soga_cheek_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Cheek_Type)->SetCount(3);
			int8_t& Soga_Lip_Type = soga_lip_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Lip_Type)->SetCount(3);
			int8_t& Soga_Chin_Type = soga_chin_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Chin_Type)->SetCount(3);
			int8_t& Soga_Nose_Type = soga_nose_type[0]; // " Type = "sint8" Size = "3" / >
			RegisterInt8(Soga_Nose_Type)->SetCount(3);
			RegisterUInt16(unknown13);
			RegisterEQ2Color(soga_hair_color1);
			RegisterEQ2Color(soga_hair_color2);
			RegisterEQ2Color(unknown14);
			RegisterUInt16(soga_hair_type);
			RegisterEQ2Color(soga_hair_type_color);
			RegisterEQ2Color(soga_hair_type_highlight_color);
			RegisterUInt16(soga_hair_face_type);
			RegisterEQ2Color(soga_hair_face_color);
			RegisterEQ2Color(soga_hair_face_highlight_color);
			uint8_t& Unknown15 = unknown15[0]; // " Type = "int8" Size = "7" / >
			RegisterUInt8(Unknown15)->SetCount(7);
		}
	};
	std::vector<CharacterListEntry> CharacterList;
	// account info
	uint32_t AccountID;
	uint32_t AccountUnknown1;
	uint16_t AccountUnknown2;
	uint32_t AccountUnknown3;
	uint8_t AccouontUnknown4;
	uint32_t AccountUnknown5[4];
	uint8_t VeteranAdventureBonus; // 1096
	uint8_t VeteranTradeskillBonus; // 1096


private:
	void RegisterElements() {
		PacketUInt8* asize = RegisterUInt8(NumCharacters);
		asize->SetMyArray(RegisterArray(CharacterList, CharacterListEntry));
		// account info
		RegisterUInt32(AccountID);
		RegisterUInt32(AccountUnknown1);
		RegisterUInt16(AccountUnknown2);
		RegisterUInt32(AccountUnknown3);
		RegisterUInt8(AccouontUnknown4);
		uint32_t& accountUnknown5 = AccountUnknown5[0];
		RegisterUInt32(accountUnknown5)->SetCount(4);
		if (GetVersion() >= 1096) {
			RegisterUInt8(VeteranAdventureBonus);
			RegisterUInt8(VeteranTradeskillBonus);
		}
	}
};

RegisterLoginStruct("OP_AllCharactersDescReplyMsg", OP_AllCharactersDescReplyMsg_Packet, 1, 1096);


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