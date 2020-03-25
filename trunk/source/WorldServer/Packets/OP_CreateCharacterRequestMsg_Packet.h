#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"
#include "../Database/WorldDatabase.h"

#include "OP_CreateCharacterReplyMsg_Packet.h"

extern WorldDatabase database;

class OP_CreateCharacterRequestMsg_Packet : public EQ2Packet {
public:
	OP_CreateCharacterRequestMsg_Packet(uint16_t version)
		: EQ2Packet(version) {
		RegisterElements();
	}

	void HandlePacket(std::shared_ptr<Client> client) override {
		uint8_t resp = database.CheckNameFilter(name.c_str());
		OP_CreateCharacterReplyMsg_Packet* reply = new OP_CreateCharacterReplyMsg_Packet(client->GetVersion());
		reply->account_id = client->GetAccountID();
		reply->response = resp;
		reply->name = "";
		if (resp == CREATESUCCESS_REPLY) {
			if (database.CreateCharacter(client->GetAccountID(), this) > 0) {
				reply->name = name;
			}
			else {
				reply->response = 0;
			}
		}
		
		client->QueuePacket(reply);
	}

	uint8_t unknown0;
	uint32_t unknown1;
	uint32_t account_id;
	uint8_t unknown3;
	uint32_t server_id;
	std::string name; // Type = "EQ2_16Bit_String" / >
	uint8_t race;
	uint8_t gender;
	uint8_t deity;
	uint8_t _class;
	uint8_t level;
	uint8_t starting_zone;
	uint8_t version;
	uint16_t unknown10; // version = 57080
	std::string race_file; // Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat skin_color;
	EQ2ColorFloat skin_color2;// not in 57080
	EQ2ColorFloat eye_color;
	EQ2ColorFloat hair_color1;
	EQ2ColorFloat hair_color2;
	EQ2ColorFloat hair_highlight; // version = 869
	uint8_t unknown8[26]; // Size = "26" / >
	std::string hair_file; // " Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat hair_type_color;
	EQ2ColorFloat hair_type_highlight_color;
	std::string face_file;
	EQ2ColorFloat hair_face_color;
	EQ2ColorFloat hair_face_highlight_color;
	std::string wing_file;
	EQ2ColorFloat wing_color1;
	EQ2ColorFloat wing_color2;
	std::string chest_file;
	EQ2ColorFloat shirt_color;
	EQ2ColorFloat unknown_chest_color;
	std::string legs_file;
	EQ2ColorFloat pants_color;
	EQ2ColorFloat unknown_legs_color;
	EQ2ColorFloat unknown9;
	float eyes2[3]; // Size = "3" / >
	float ears[3]; // Size = "3" / >
	float eye_brows[3]; // Size = "3" / >
	float cheeks[3]; // Size = "3" / >
	float lips[3]; // Size = "3" / >
	float chin[3]; // Size = "3" / >
	float nose[3]; // Size = "3" / >
	float body_size;
	float body_age;
	uint8_t soga_version;
	std::string soga_race_file; // Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat soga_skin_color;
	EQ2ColorFloat soga_eye_color;
	EQ2ColorFloat soga_hair_color1;
	EQ2ColorFloat soga_hair_color2;
	EQ2ColorFloat soga_hair_highlight;
	EQ2ColorFloat soga_unknown_color; //869
	uint8_t soga_unknown11[26]; // Size = "26" / >
	std::string soga_hair_file; // Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat soga_hair_type_color;
	EQ2ColorFloat soga_hair_type_highlight_color;
	std::string soga_face_file; // Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat soga_hair_face_color;
	EQ2ColorFloat soga_hair_face_highlight_color;
	std::string soga_wing_file; // Type = "EQ2_16Bit_String" / >
	EQ2ColorFloat soga_wing_color1;
	EQ2ColorFloat soga_wing_color2;
	std::string soga_chest_file;
	EQ2ColorFloat soga_shirt_color;
	EQ2ColorFloat soga_unknown_chest_color;
	std::string soga_legs_file;
	EQ2ColorFloat soga_pants_color;
	EQ2ColorFloat soga_unknown_legs_color;
	EQ2ColorFloat soga_unknown12;
	float soga_eyes2[3]; // Size = "3" / >
	float soga_ears[3]; // Size = "3" / >
	float soga_eye_brows[3]; // Size = "3" / >
	float soga_cheeks[3]; // Size = "3" / >
	float soga_lips[3]; // Size = "3" / >
	float soga_chin[3]; // Size = "3" / >
	float soga_nose[3]; // Size = "3" / >
	float soga_body_size;
	float soga_body_age;

private:
	void RegisterElements() {
		RegisterUInt8(unknown0);
		RegisterUInt32(unknown1);
		RegisterUInt32(account_id);
		RegisterUInt8(unknown3);
		RegisterUInt32(server_id);
		Register16String(name);
		RegisterUInt8(race);
		RegisterUInt8(gender);
		RegisterUInt8(deity);
		RegisterUInt8(_class);
		RegisterUInt8(level);
		RegisterUInt8(starting_zone);
		RegisterUInt8(version);
		if (GetVersion() >= 57080)
			RegisterUInt16(unknown10);
		Register16String(race_file);
		RegisterEQ2ColorFloat(skin_color);
		if (GetVersion() < 57080)
			RegisterEQ2ColorFloat(skin_color2);// not in 57080
		RegisterEQ2ColorFloat(eye_color);
		RegisterEQ2ColorFloat(hair_color1);
		RegisterEQ2ColorFloat(hair_color2);
		if (GetVersion() >= 869)
			RegisterEQ2ColorFloat(hair_highlight);
		uint8_t& Unknown8 = unknown8[0]; // Size = "26" / >
		RegisterUInt8(Unknown8)->SetCount(26);
		Register16String(hair_file);
		RegisterEQ2ColorFloat(hair_type_color);
		RegisterEQ2ColorFloat(hair_type_highlight_color);
		Register16String(face_file);
		RegisterEQ2ColorFloat(hair_face_color);
		RegisterEQ2ColorFloat(hair_face_highlight_color);
		Register16String(wing_file);
		RegisterEQ2ColorFloat(wing_color1);
		RegisterEQ2ColorFloat(wing_color2);
		Register16String(chest_file);
		RegisterEQ2ColorFloat(shirt_color);
		RegisterEQ2ColorFloat(unknown_chest_color);
		Register16String(legs_file);
		RegisterEQ2ColorFloat(pants_color);
		RegisterEQ2ColorFloat(unknown_legs_color);
		RegisterEQ2ColorFloat(unknown9);
		float& Eyes2 = eyes2[0];
		RegisterFloat(Eyes2)->SetCount(3);
		float& Ears = ears[0];
		RegisterFloat(Ears)->SetCount(3);
		float& Eye_brow = eye_brows[0];
		RegisterFloat(Eye_brow)->SetCount(3);
		float& Cheeks = cheeks[0];
		RegisterFloat(Cheeks)->SetCount(3);
		float& Lips = lips[0];
		RegisterFloat(Lips)->SetCount(3);
		float& Chin = chin[0];
		RegisterFloat(Chin)->SetCount(3);
		float& Nose = nose[0];
		RegisterFloat(Nose)->SetCount(3);
		RegisterFloat(body_size);
		RegisterFloat(body_age);
		RegisterUInt8(soga_version);
		Register16String(soga_race_file);
		RegisterEQ2ColorFloat(soga_skin_color);
		RegisterEQ2ColorFloat(soga_eye_color);
		RegisterEQ2ColorFloat(soga_hair_color1);
		RegisterEQ2ColorFloat(soga_hair_color2);
		RegisterEQ2ColorFloat(soga_hair_highlight);
		if (GetVersion() >= 869)
			RegisterEQ2ColorFloat(soga_unknown_color);
		uint8_t& Soga_unknown11 = soga_unknown11[0]; // Size = "26" / >
		RegisterUInt8(Soga_unknown11)->SetCount(26);
		Register16String(soga_hair_file);
		RegisterEQ2ColorFloat(soga_hair_type_color);
		RegisterEQ2ColorFloat(soga_hair_type_highlight_color);
		Register16String(soga_face_file);
		RegisterEQ2ColorFloat(soga_hair_face_color);
		RegisterEQ2ColorFloat(soga_hair_face_highlight_color);
		Register16String(soga_wing_file);
		RegisterEQ2ColorFloat(soga_wing_color1);
		RegisterEQ2ColorFloat(soga_wing_color2);
		Register16String(soga_chest_file);
		RegisterEQ2ColorFloat(soga_shirt_color);
		RegisterEQ2ColorFloat(soga_unknown_chest_color);
		Register16String(soga_legs_file);
		RegisterEQ2ColorFloat(soga_pants_color);
		RegisterEQ2ColorFloat(soga_unknown_legs_color);
		RegisterEQ2ColorFloat(soga_unknown12);
		float& Soga_eyes2 = soga_eyes2[0];
		RegisterFloat(Soga_eyes2)->SetCount(3);
		float& Soga_ears = soga_ears[0];
		RegisterFloat(Soga_ears)->SetCount(3);
		float& Soga_eye_brows = soga_eye_brows[0];
		RegisterFloat(Soga_eye_brows)->SetCount(3);
		float& Soga_cheeks = soga_cheeks[0];
		RegisterFloat(Soga_cheeks)->SetCount(3);
		float& Soga_lips = soga_lips[0];
		RegisterFloat(Soga_lips)->SetCount(3);
		float& Soga_chin = soga_chin[0];
		RegisterFloat(Soga_chin)->SetCount(3);
		float& Soga_nose = soga_nose[0];
		RegisterFloat(Soga_nose)->SetCount(3);
		RegisterFloat(soga_body_size);
		RegisterFloat(soga_body_age);
	}

};

/*

<Struct Name="CreateCharacter" ClientVersion="1" OpcodeName="OP_CreateCharacterRequestMsg">
<Data ElementName="unknown0" Type="int8" />
<Data ElementName="unknown1" Type="int32" />
<Data ElementName="account_id" Type="int32"  />
<Data ElementName="unknown3" Type="int8" />
<Data ElementName="server_id" Type="int32" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
<Data ElementName="race" Type="int8" />
<Data ElementName="gender" Type="int8" />
<Data ElementName="deity" Type="int8" />
<Data ElementName="class" Type="int8" />
<Data ElementName="level" Type="int8" />
<Data ElementName="starting_zone" Type="int8" />
<Data ElementName="version" Type="int8" />
<Data ElementName="race_file" Type="EQ2_16Bit_String" />
<Data ElementName="skin_color" Type="EQ2_Color" />
<Data ElementName="skin_color2" Type="EQ2_Color" />
<Data ElementName="eye_color" Type="EQ2_Color" />
<Data ElementName="hair_color1" Type="EQ2_Color" />
<Data ElementName="hair_color2" Type="EQ2_Color" />
<Data ElementName="unknown8" Type="int8" Size="26" />
<Data ElementName="hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="face_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown9" Type="EQ2_Color" Size="1" />
<Data ElementName="eyes2" Type="float" Size="3" />
<Data ElementName="ears" Type="float" Size="3" />
<Data ElementName="eye_brows" Type="float" Size="3" />
<Data ElementName="cheeks" Type="float" Size="3" />
<Data ElementName="lips" Type="float" Size="3" />
<Data ElementName="chin" Type="float" Size="3" />
<Data ElementName="nose" Type="float" Size="3" />
<Data ElementName="body_size" Type="float" Size="1" />
<Data ElementName="body_age" Type="float" Size="1" />
<Data ElementName="soga_version" Type="int8" />
<Data ElementName="soga_race_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color"  />
<Data ElementName="soga_hair_color2" Type="EQ2_Color"  />
<Data ElementName="soga_hair_highlight" Type="EQ2_Color" />
<Data ElementName="soga_unknown11" Type="int8" Size="26" />
<Data ElementName="soga_hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_face_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown12" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_eyes2" Type="float" Size="3" />
<Data ElementName="soga_ears" Type="float" Size="3" />
<Data ElementName="soga_eye_brows" Type="float" Size="3" />
<Data ElementName="soga_cheeks" Type="float" Size="3" />
<Data ElementName="soga_lips" Type="float" Size="3" />
<Data ElementName="soga_chin" Type="float" Size="3" />
<Data ElementName="soga_nose" Type="float" Size="3" />
<Data ElementName="soga_body_size" Type="float" Size="1" />
<Data ElementName="soga_body_age" Type="float" Size="1" />
</Struct>
<Struct Name="CreateCharacter" ClientVersion="869" OpcodeName="OP_CreateCharacterRequestMsg">
<Data ElementName="unknown0" Type="int8" />
<Data ElementName="unknown1" Type="int32" />
<Data ElementName="account_id" Type="int32"  />
<Data ElementName="unknown3" Type="int8" />
<Data ElementName="server_id" Type="int32" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
<Data ElementName="race" Type="int8" />
<Data ElementName="gender" Type="int8" />
<Data ElementName="deity" Type="int8" />
<Data ElementName="class" Type="int8" />
<Data ElementName="level" Type="int8" />
<Data ElementName="starting_zone" Type="int8" />
<Data ElementName="version" Type="int8" />
<Data ElementName="race_file" Type="EQ2_16Bit_String" />
<Data ElementName="skin_color" Type="EQ2_Color" Size="1" />
<Data ElementName="skin_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="eye_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_highlight" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown8" Type="int8" Size="26" />
<Data ElementName="hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="face_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown9" Type="EQ2_Color" Size="1" />
<Data ElementName="eyes2" Type="float" Size="3" />
<Data ElementName="ears" Type="float" Size="3" />
<Data ElementName="eye_brows" Type="float" Size="3" />
<Data ElementName="cheeks" Type="float" Size="3" />
<Data ElementName="lips" Type="float" Size="3" />
<Data ElementName="chin" Type="float" Size="3" />
<Data ElementName="nose" Type="float" Size="3" />
<Data ElementName="body_size" Type="float" Size="1" />
<Data ElementName="body_age" Type="float" Size="1" />
<Data ElementName="soga_version" Type="int8" />
<Data ElementName="soga_race_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color" />
<Data ElementName="soga_hair_color2" Type="EQ2_Color" />
<Data ElementName="soga_hair_highlight" Type="EQ2_Color" />
<Data ElementName="soga_unknown_color1" Type="EQ2_Color" />
<Data ElementName="soga_unknown11" Type="int8" Size="26" />
<Data ElementName="soga_hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_face_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown12" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_eyes2" Type="float" Size="3" />
<Data ElementName="soga_ears" Type="float" Size="3" />
<Data ElementName="soga_eye_brows" Type="float" Size="3" />
<Data ElementName="soga_cheeks" Type="float" Size="3" />
<Data ElementName="soga_lips" Type="float" Size="3" />
<Data ElementName="soga_chin" Type="float" Size="3" />
<Data ElementName="soga_nose" Type="float" Size="3" />
<Data ElementName="soga_body_size" Type="float" Size="1" />
<Data ElementName="soga_body_age" Type="float" Size="1" />
</Struct>
<Struct Name="CreateCharacter" ClientVersion="1096" OpcodeName="OP_CreateCharacterRequestMsg">
<Data ElementName="unknown0" Type="int8" />
<Data ElementName="unknown1" Type="int32" />
<Data ElementName="account_id" Type="int32"  />
<Data ElementName="unknown3" Type="int8" />
<Data ElementName="server_id" Type="int32" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
<Data ElementName="race" Type="int8" />
<Data ElementName="gender" Type="int8" />
<Data ElementName="deity" Type="int8" />
<Data ElementName="class" Type="int8" />
<Data ElementName="level" Type="int8" />
<Data ElementName="starting_zone" Type="int8" />
<Data ElementName="version" Type="int8" />
<Data ElementName="race_file" Type="EQ2_16Bit_String" />
<Data ElementName="skin_color" Type="EQ2_Color" />
<Data ElementName="skin_color2" Type="EQ2_Color" />
<Data ElementName="eye_color" Type="EQ2_Color" />
<Data ElementName="hair_color1" Type="EQ2_Color"  />
<Data ElementName="hair_color2" Type="EQ2_Color"  />
<Data ElementName="hair_highlight" Type="EQ2_Color" />
<Data ElementName="unknown8" Type="int8" Size="26" />
<Data ElementName="hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="face_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown9" Type="EQ2_Color" Size="1" />

<Data ElementName="eyes2" Type="float" Size="3" />
<Data ElementName="ears" Type="float" Size="3" />
<Data ElementName="eye_brows" Type="float" Size="3" />
<Data ElementName="cheeks" Type="float" Size="3" />
<Data ElementName="lips" Type="float" Size="3" />
<Data ElementName="chin" Type="float" Size="3" />
<Data ElementName="nose" Type="float" Size="3" />
<Data ElementName="body_size" Type="float" Size="1" />
<Data ElementName="body_age" Type="float" Size="1" />

<Data ElementName="soga_version" Type="int8" />
<Data ElementName="soga_race_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color"  />
<Data ElementName="soga_hair_color2" Type="EQ2_Color"  />
<Data ElementName="soga_hair_highlight" Type="EQ2_Color" />
<Data ElementName="soga_unknown_color" Type="EQ2_Color" />
<Data ElementName="soga_unknown11" Type="int8" Size="26" />
<Data ElementName="soga_hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_face_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown12" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_eyes2" Type="float" Size="3" />
<Data ElementName="soga_ears" Type="float" Size="3" />
<Data ElementName="soga_eye_brows" Type="float" Size="3" />
<Data ElementName="soga_cheeks" Type="float" Size="3" />
<Data ElementName="soga_lips" Type="float" Size="3" />
<Data ElementName="soga_chin" Type="float" Size="3" />
<Data ElementName="soga_nose" Type="float" Size="3" />
<Data ElementName="soga_body_size" Type="float" Size="1" />
<Data ElementName="soga_body_age" Type="float" Size="1" />
</Struct>
<Struct Name="CreateCharacter" ClientVersion="57080" OpcodeName="OP_CreateCharacterRequestMsg">
<Data ElementName="unknown0" Type="int8" />
<Data ElementName="unknown1" Type="int32" />
<Data ElementName="account_id" Type="int32"  />
<Data ElementName="unknown3" Type="int8" />
<Data ElementName="server_id" Type="int32" />
<Data ElementName="name" Type="EQ2_16Bit_String" />
<Data ElementName="race" Type="int8" />
<Data ElementName="gender" Type="int8" />
<Data ElementName="deity" Type="int8" />
<Data ElementName="class" Type="int8" />
<Data ElementName="level" Type="int8" />
<Data ElementName="starting_zone" Type="int8" />
<Data ElementName="version" Type="int8" />
<Data ElementName="unknown10" Type="int16" />
<Data ElementName="race_file" Type="EQ2_16Bit_String" />
<Data ElementName="skin_color" Type="EQ2_Color" />
<Data ElementName="eye_color" Type="EQ2_Color" />
<Data ElementName="unknown_skin_color2" Type="EQ2_Color" />
<Data ElementName="hair_color1" Type="EQ2_Color"  />
<Data ElementName="hair_color2" Type="EQ2_Color"  />
<Data ElementName="hair_highlight" Type="EQ2_Color" />
<Data ElementName="unknown8" Type="int8" Size="26" />
<Data ElementName="hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="face_file" Type="EQ2_16Bit_String" />
<Data ElementName="hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="unknown9" Type="EQ2_Color" Size="1" />

<Data ElementName="eyes2" Type="float" Size="3" />
<Data ElementName="ears" Type="float" Size="3" />
<Data ElementName="eye_brows" Type="float" Size="3" />
<Data ElementName="cheeks" Type="float" Size="3" />
<Data ElementName="lips" Type="float" Size="3" />
<Data ElementName="chin" Type="float" Size="3" />
<Data ElementName="nose" Type="float" Size="3" />
<Data ElementName="body_size" Type="float" Size="1" />
<Data ElementName="body_age" Type="float" Size="1" />

<Data ElementName="soga_version" Type="int8" />
<Data ElementName="soga_race_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_skin_color" Type="EQ2_Color" />
<Data ElementName="soga_eye_color" Type="EQ2_Color" />
<Data ElementName="soga_hair_color1" Type="EQ2_Color"  />
<Data ElementName="soga_hair_color2" Type="EQ2_Color"  />
<Data ElementName="soga_hair_highlight" Type="EQ2_Color" />
<Data ElementName="soga_unknown_color" Type="EQ2_Color" />
<Data ElementName="soga_unknown11" Type="int8" Size="26" />
<Data ElementName="soga_hair_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_type_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_type_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_face_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_hair_face_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_hair_face_highlight_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_wing_color1" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_wing_color2" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_chest_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_shirt_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_chest_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_legs_file" Type="EQ2_16Bit_String" />
<Data ElementName="soga_pants_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown_legs_color" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_unknown12" Type="EQ2_Color" Size="1" />
<Data ElementName="soga_eyes2" Type="float" Size="3" />
<Data ElementName="soga_ears" Type="float" Size="3" />
<Data ElementName="soga_eye_brows" Type="float" Size="3" />
<Data ElementName="soga_cheeks" Type="float" Size="3" />
<Data ElementName="soga_lips" Type="float" Size="3" />
<Data ElementName="soga_chin" Type="float" Size="3" />
<Data ElementName="soga_nose" Type="float" Size="3" />
<Data ElementName="soga_body_size" Type="float" Size="1" />
<Data ElementName="soga_body_age" Type="float" Size="1" />
</Struct>

*/