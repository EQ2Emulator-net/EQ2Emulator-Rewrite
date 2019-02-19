#pragma once

#include <stdint.h>
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"

/* These structs should match the latest xml structs exactly, if changes are made to one the should also be made to the other */

///<summary>Packet struct containing the spawns position information</summary>
struct SpawnPositionStruct {
	uint32_t	grid_id;
	float		X;
	float		Y;
	float		Z;
	int16_t		unknown[3];
	int16_t		heading1;
	int16_t		heading2;
	uint16_t	speed;
	uint16_t	state;
	uint8_t		unknown2[6];
	float		next_X;
	float		next_Y;
	float		next_Z;
	float		X3;
	float		Y3;
	float		Z3;
	uint8_t		movement_mode;
	uint8_t		unknown3b;
	uint16_t	unknown4[3];
	uint16_t	move_type;
	uint16_t	unknown6[5];
	int16_t		pitch1;
	uint16_t	unknown7[4];
	int16_t		pitch2;
	uint16_t	collision_radius;
	uint16_t	size;
	float		size_ratio;
	float		size_multiplier_ratio;
	uint16_t	unknown10[3];
	int16_t		roll;
	uint16_t	unknown12[2];
};

///<summary>Packet struct containing the spawns visual information</summary>
struct SpawnVisualizationStruct {
	uint32_t	unknown;
	uint8_t		arrow_color;
	uint8_t		locked_no_loot;
	int8_t		npc_con;
	uint8_t		quest_flag;
	uint8_t		merc_icon;
	uint8_t		npc_hate;
	uint8_t		unknowna;
	uint8_t		name_quest_icon;
	uint8_t		unknowna2[4];
	uint8_t		vis_flags;
	uint8_t		npc_aaxp;
	uint8_t		unknownb[2];
	uint8_t		hand_flag;
	uint8_t		unkown2[2];
	uint8_t		tag1;
	uint8_t		tag2;
	uint8_t		unknown2a[3];
};

///<summary>Packet sub struct containg spell information</summary>
struct SpawnTargetSpellEffectsStruct {
	uint32_t	spell_id;
	uint16_t	spell_icon;
	uint16_t	spell_triggercount;
	uint16_t	spell_icon_backdrop;
	uint16_t	unknown;
};

///<summary>Packet struct containg the spawns general information</summary>
struct SpawnInfoStruct {
	uint32_t						model_type;
	uint32_t						soga_model_type;
	uint32_t						equipment_types[25];
	uint16_t						unknown7a[2];
	uint16_t						unknown7aa[2];
	uint16_t						hair_type_id;
	uint16_t						unknown30;
	uint16_t						facial_hair_type_id;
	uint16_t						unknown30a;
	uint16_t						wing_type_id;
	uint16_t						unknown30b;
	uint16_t						chest_type_id;
	uint16_t						unknown30c;
	uint16_t						legs_type_id;
	uint16_t						unknown_new_type_id;
	uint16_t						unknown30d;
	uint16_t						unknown30e;
	uint16_t						soga_hair_type_id;
	uint16_t						unknown30f;
	uint16_t						soga_facial_hair_type_id;
	uint16_t						unknown30g;
	uint32_t						mount_type;
	uint32_t						combat_voice;
	uint32_t						emote_voice;
	uint32_t						unknown17;
	uint8_t							visual_flag;
	uint8_t							interaction_flag;
	uint8_t							unknown60055[18];
	EQ2Color						equipment_colors[25];
	EQ2Color						unknown7b[2];
	EQ2Color						hair_type_color;
	EQ2Color						hair_face_color;
	EQ2Color						wing_color1;
	EQ2Color						unknown10[3];
	EQ2Color						equipment_highlights[25];
	EQ2Color						unknown7c[2];
	EQ2Color						hair_type_highlight_color;
	EQ2Color						hair_face_highlight_color;
	EQ2Color						wing_color2;
	EQ2Color						unknown11[3];
	EQ2Color						soga_hair_type_color;
	EQ2Color						soga_hair_type_highlight_color;
	EQ2Color						soga_hair_face_color;
	EQ2Color						soga_hair_face_highlight_color;
	EQ2Color						skin_color;
	EQ2Color						eye_color;
	EQ2Color						soga_eye_color;
	EQ2Color						soga_skin_color;
	EQ2Color						kunark_unknown_color1;
	EQ2Color						kunark_unknown_color2;
	EQ2Color						unknown12;
	int8_t							eye_type[3];
	int8_t							ear_type[3];
	int8_t							eye_brow_type[3];
	int8_t							cheek_type[3];
	int8_t							lip_type[3];
	int8_t							chin_type[3];
	int8_t							nose_type[3];
	int8_t							body_size;
	int8_t							body_size_unknown;
	uint8_t							unknown14a[3];
	int8_t							soga_eye_type[3];
	int8_t							soga_ear_type[3];
	int8_t							soga_eye_brow_type[3];
	int8_t							soga_cheek_type[3];
	int8_t							soga_lip_type[3];
	int8_t							soga_chin_type[3];
	int8_t							soga_nose_type[3];
	uint8_t							unknown14b[2];
	EQ2Color						mount_color;
	EQ2Color						mount_saddle_color;
	EQ2Color						hair_color1;
	EQ2Color						hair_color2;
	EQ2Color						hair_highlight;
	EQ2Color						soga_hair_color1;
	EQ2Color						soga_hair_color2;
	EQ2Color						soga_hair_highlight;
	uint32_t						action_state;
	uint32_t						visual_state;
	uint32_t						mood_state;
	uint32_t						emote_state;
	uint8_t							unknown600552[52];
	SpawnTargetSpellEffectsStruct	spell_effects[30];
	uint32_t						target_id;
	uint32_t						follow_target;
	uint32_t						size_unknown;
	uint8_t							unknown3b[4];
	uint8_t							spawn_type;
	uint8_t							icon;
	uint32_t						activity_status;
	uint8_t							unknown600554[10];
	uint32_t						hp_remaining;
	uint32_t						power_percent;
	uint8_t							unknown600553[9];
	uint8_t							unknown4;
	uint8_t							level;
	uint8_t							unknown5;
	uint8_t							heroic_flag;
	uint32_t						unknown7;
	uint8_t							race;
	uint8_t							gender;
	uint8_t							adv_class;
	uint8_t							difficulty;
	uint8_t							unknown21[3];
	uint16_t						unknownz5;
	uint8_t							unknown6;
	uint32_t						unknown6b;
	uint8_t							unknown18[68];
};

struct SpawnTitleStruct {
	std::string name;
	std::string last_name;
	std::string suffix_title;
	std::string prefix_title;
	std::string sub_title;
};