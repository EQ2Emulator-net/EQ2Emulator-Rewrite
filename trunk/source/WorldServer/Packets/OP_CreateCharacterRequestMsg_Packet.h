#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../WorldServer/Client.h"
#include "../Database/WorldDatabase.h"

#include "OP_CreateCharacterReplyMsg_Packet.h"
#include "OP_AllCharactersDescReplyMsg_Packet.h"

extern WorldDatabase database;

class Substruct_CharacterCustomization_Asset : public PacketSubstruct {
public:
	Substruct_CharacterCustomization_Asset(uint32_t ver = 0) : PacketSubstruct(ver) {

	}

	~Substruct_CharacterCustomization_Asset() = default;

	void RegisterElements() override {
		Register16String(file);
		RegisterEQ2ColorFloat(color);
		RegisterEQ2ColorFloat(highlight);
	}

	std::string file;
	EQ2ColorFloat color;
	EQ2ColorFloat highlight;
};

//This substruct is used in at least 1 other packet
class Substruct_CharacterCustomization : public PacketSubstruct {
public:
	Substruct_CharacterCustomization(uint32_t ver = 0) : PacketSubstruct(ver), 
		unkAsset_67650(ver), hairAsset(ver), faceAsset(ver), wingAsset(ver), chestAsset(ver), legsAsset(ver) {

	}

	~Substruct_CharacterCustomization() = default;

	uint8_t version;
	std::string race_file;
	EQ2ColorFloat skin_color;
	EQ2ColorFloat model_color;
	EQ2ColorFloat eye_color;
	EQ2ColorFloat hair_color1;
	EQ2ColorFloat hair_color2;
	EQ2ColorFloat hair_highlight;
	union {
		SpawnMorphSliders sliders;
		int8_t sliderBytes[26];
	};
	union {
		float sliderFloats[26];
		SpawnFloatMorphSliders floatSliders;
	};
	Substruct_CharacterCustomization_Asset unkAsset_67650;
	Substruct_CharacterCustomization_Asset hairAsset;
	Substruct_CharacterCustomization_Asset faceAsset;
	Substruct_CharacterCustomization_Asset wingAsset;
	Substruct_CharacterCustomization_Asset tailAsset;
	Substruct_CharacterCustomization_Asset chestAsset;
	Substruct_CharacterCustomization_Asset legsAsset;

	void RegisterElements() override {
		RegisterUInt8(version);
		Register16String(race_file);
		RegisterEQ2ColorFloat(skin_color);
		if (GetVersion() < 57080) //moved in 57080
			RegisterEQ2ColorFloat(model_color);
		RegisterEQ2ColorFloat(eye_color);
		if (GetVersion() >= 57080) //moved in 57080
			RegisterEQ2ColorFloat(model_color);
		RegisterEQ2ColorFloat(hair_color1);
		RegisterEQ2ColorFloat(hair_color2);
		if (GetVersion() >= 869)
			RegisterEQ2ColorFloat(hair_highlight);
		if (GetVersion() < 67650) {
			RescopeArrayElement(sliderBytes);
			RegisterInt8(sliderBytes)->SetCount(26);
		}
		if (GetVersion() >= 67650) {
			RegisterSubstruct(unkAsset_67650);
		}
		RegisterSubstruct(hairAsset);
		RegisterSubstruct(faceAsset);
		if (GetVersion() > 283) {
			RegisterSubstruct(wingAsset);
			if (GetVersion() >= 67804) {
				RegisterSubstruct(tailAsset);
			}
		}
		RegisterSubstruct(chestAsset);
		RegisterSubstruct(legsAsset);

		RescopeArrayElement(sliderFloats);
		RegisterFloat(sliderFloats)->SetCount(26);
	}

	void PostRead() override {
		if (GetVersion() >= 67650) {
			//They got rid of the 26 byte sliders (customization version 7)
			for (int i = 0; i < 26; i++) {
				sliderBytes[i] = static_cast<int8_t>(sliderFloats[i] * 127.f);
			}
		}
	}
};

class OP_CreateCharacterRequestMsg_Packet : public EQ2Packet {
public:
	OP_CreateCharacterRequestMsg_Packet(uint32_t version)
		: EQ2Packet(version), customization(version), soga_customization(version) {
		starting_zone = 0;
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
	uint16_t starting_zone;
	uint8_t unknown57080a; 
	uint8_t unknown57080b;
	uint8_t unknown67650;

	Substruct_CharacterCustomization customization;
	Substruct_CharacterCustomization soga_customization;

	std::string unk67650_string;

private:
	void RegisterElements() {
		if (GetVersion() > 283) {
			RegisterUInt8(unknown0);
			RegisterUInt8(unknown3);
			RegisterUInt32(unknown1);
		}
		RegisterUInt32(account_id);
		RegisterUInt32(server_id);
		Register16String(name);
		RegisterUInt8(race);
		RegisterUInt8(gender);
		RegisterUInt8(deity);
		RegisterUInt8(_class);
		RegisterUInt8(level);
		if (GetVersion() > 283) {
			uint8_t& starting_zone = reinterpret_cast<uint8_t&>(this->starting_zone);
			RegisterUInt8(starting_zone);
		}
		else {
			RegisterUInt16(starting_zone);
		}
		if (GetVersion() >= 57080) {
			RegisterUInt8(unknown57080a);
			RegisterUInt8(unknown57080b);
			if (GetVersion() >= 67650) {
				RegisterUInt8(unknown67650);
			}
		}
		
		RegisterSubstruct(customization);
		if (GetVersion() <= 283) {
			return;
		}
		RegisterSubstruct(soga_customization);
		if (GetVersion() >= 67650) {
			Register16String(unk67650_string);
		}
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