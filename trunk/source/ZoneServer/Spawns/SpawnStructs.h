#pragma once

#include <stdint.h>
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketEncodedData.h"
#include "../../common/Packets/EQ2Packets/Substruct_NetAppearance.h"

//Position state bitflags
const uint32_t POS_STATE_FLYMODE = 1;
const uint32_t POS_STATE_FLYMODE_NO_COLLIDE = 1 << 1;
//The lava widget in najena's has this, play with it?
const uint32_t POS_STATE_WATER = 1 << 2;
const uint32_t POS_STATE_NO_MOVEMENT = 1 << 3;
const uint32_t POS_STATE_NO_TURNING = 1 << 4;
const uint32_t POS_STATE_GLIDE = 1 << 5;
const uint32_t POS_STATE_SITTING = 1 << 6;
const uint32_t POS_STATE_ON_GROUND = 1 << 7;
const uint32_t POS_STATE_ON_TRANSPORT = 1 << 8;
const uint32_t POS_STATE_CROUCHING = 1 << 9;
//Not sure what this is but *most* npcs have it
const uint32_t POS_STATE_UNKNOWN = 1 << 14;
const uint32_t POS_STATE_DISABLE_GRAVITY = 1 << 15;

struct vec3 {
	float x;
	float y;
	float z;
};

///<summary>Packet struct containing the spawns position information</summary>
struct SpawnPositionStruct {
	SpawnPositionStruct();

	//We need the client unless we figure out a way to resend the same spawn ID
	void InsertSpawnData(const std::shared_ptr<class Client>& client, const std::shared_ptr<class Spawn>& spawn);

	uint32_t positionState; //confirmed
	uint32_t positionState2; //confirmed
	float desiredHeading; //confirmed
	float desiredHeadingVelocity;
	float desiredRoll;
	float desiredRollVelocity;
	float desiredPitch; //confirmed
	float desiredPitchVelocity;
	float unusedUnknown;
	uint32_t grid_id; //confirmed
	float x; //confirmed
	float y; //confirmed
	float z; //confirmed
	uint8_t movementMode; //confirmed
	uint8_t unkByte;
	int16_t unk72;
	float heading; //confirmed
	float pitch; //confirmed
	float roll; //confirmed
	float destLocX; //confirmed
	float destLocY; //confirmed
	float destLocZ; //confirmed
	float destLocX2; //confirmed
	float destLocY2; //confirmed
	float destLocZ2; //confirmed
	//I think collision radius is like the base scale
	float collisionRadius;
	//I think this is a "temp" scale but in reality mostly permanent
	float size;
	float sizeRatio;
	uint32_t faceActorID; //confirmed although not sure what it does yet
	float baseLocX;
	float baseLocY;
	float baseLocZ;
	float desiredForwardSpeed;
	float speedModifier; //confirmed
	float airSpeed; //confirmed
	float unkSpeed3;
	float desiredVertSpeed;
	float desiredStrafeSpeed;
	float velocityX; //confirmed
	float velocityY; //confirmed
	float velocityZ; //confirmed
	float swimmingSpeedMultiplier; //confirmed
	float sizeUnknown;
	float actorStopRange; //semi confirmed but not sure what it does yet
	float unk70;
};

struct CompressedSpawnPositionData {
	CompressedSpawnPositionData() {
		memset(this, 0, sizeof(*this));
	}
	//The following are only used in packets, they do not represents values we use for calculations
	uint16_t alignment_filler;
	int16_t velocityX_compressed;
	int16_t velocityY_compressed;
	int16_t velocityZ_compressed;
	int16_t heading_compressed;
	int16_t desiredHeading_compressed;
	int16_t desiredForwardSpeed_compressed;
	int16_t desiredHeadingVelocity_compressed;
	int16_t speedModifier_compressed;
	int16_t airSpeed_compressed;
	int16_t swimmingSpeedMultiplier_compressed;
	int16_t desiredStrafeSpeed_compressed;
	int16_t desiredVertSpeed_compressed;
	int16_t unkSpeed3_compressed;
	int16_t desiredRoll_compressed;
	int16_t desiredRollVelocity_compressed;
	int16_t desiredPitch_compressed;
	int16_t desiredPitchVelocity_compressed;
	int16_t baseLocX_compressed;
	int16_t baseLocY_compressed;
	int16_t baseLocZ_compressed;
	int16_t pitch_compressed;
	int16_t collisionRadius_compressed;
	int16_t size_compressed;
	int16_t actorStopRange_compressed;
	int16_t roll_compressed;
	int16_t unk70_compressed;
	int16_t x_compressed;
	int16_t y_compressed;
	int16_t z_compressed;
	int16_t destX_compressed;
	int16_t destY_compressed;
	int16_t destZ_compressed;
	int16_t destX2_compressed;
	int16_t destY2_compressed;
	int16_t destZ2_compressed;
	int16_t sizeUnknown_compressed;
};

class Substruct_SpawnPosition : public PacketEncodedData, public SpawnPositionStruct, protected CompressedSpawnPositionData  {
public:
	Substruct_SpawnPosition(uint32_t version) : PacketEncodedData(version) {	
		RegisterElements();
	}

	inline int16_t CompressFloat(const float& f, const float& factor) {
		return static_cast<int16_t>(f * factor);
	}

	void CompressData();

	inline float ExpandFloat(const int16_t& i, const float& factor) {
		return static_cast<float>(i / factor);
	}

	void DecompressData();

	void RegisterElements() override;

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override;
	bool ReadElement(const unsigned char* buf, uint32_t& offset, uint32_t bufsize) override;
};

///<summary>Packet struct containing the "viewer specific" variables which change depending on the client</summary>
struct SpawnVisualizationStruct {
	uint32_t	unknown;
	uint8_t		considerDifficulty;
	uint8_t		locked_no_loot;
	//in old code this was npc_con
	int8_t		factionCon;
	uint8_t		quest_flag;
	uint8_t		merc_icon;
	//npc_hate in old code
	bool        bShowAggro;
	uint8_t		pet_threat;
	uint8_t		name_quest_icon;
	uint32_t	unknowna2; //quest related, maybe id
	uint8_t		unknownb[3];
	bool    	bShowHandFlag;
	uint32_t    unknown2b;
	uint32_t    unknown4;
	uint8_t		unknown2[6];
	uint8_t		unknown2a[3];
	uint8_t     pvp_difficulty;
	uint8_t     tag_icon;
	uint8_t     tag_number;
	uint8_t     unknown3;
	uint16_t    vis_flags;

	SpawnVisualizationStruct() {
		memset(this, 0, sizeof(SpawnVisualizationStruct));
	}

	uint32_t CalculateCRC();
	void DetermineForClient(const std::shared_ptr<class Client>& client, const std::shared_ptr<class Spawn>& spawn);

private:
	void SetVisualFlags(const std::shared_ptr<class Client>& client, const std::shared_ptr<class Spawn>& spawn, const std::shared_ptr<class Entity>& player);
	void SetConLevel(const std::shared_ptr<class Spawn>& spawn, const std::shared_ptr<class Entity>& player);

	static const uint16_t VIS_FLAG_GROUPED_WITH_PLAYER = 1 << 0;
	static const uint16_t VIS_FLAG_SHOW_COMMAND_ICON = 1 << 1;
	static const uint16_t VIS_FLAG_TARGETABLE = 1 << 2;
	static const uint16_t VIS_FLAG_RED_OUTLINE = 1 << 3;
	static const uint16_t VIS_FLAG_DISPLAY_NAME = 1 << 4;
	static const uint16_t VIS_FLAG_SHOW_LEVEL = 1 << 5;
	static const uint16_t VIS_FLAG_ATTACKABLE = 1 << 6;
	static const uint16_t VIS_FLAG_HAS_AA_EXP = 1 << 8;

};

class Substruct_SpawnVisualization : public SpawnVisualizationStruct, public PacketEncodedData {
public:
	Substruct_SpawnVisualization(uint32_t version) : PacketEncodedData(version) {
		RegisterElements();
	}

	void RegisterElements();
};

///<summary>Packet sub struct containing spell information</summary>
class Substruct_TargetSpellInfo : public PacketSubstruct {
public:
	Substruct_TargetSpellInfo(uint32_t ver = 0) : PacketSubstruct(ver), 
		spell_id(-1), spell_icon(0), spell_triggercount(0), spell_icon_backdrop(0), unknown(0) {}

	void RegisterElements() override {
		RegisterInt32(spell_id);
		RegisterInt16(spell_icon);
		RegisterInt16(spell_triggercount);
		RegisterUInt16(spell_icon_backdrop);
		if (version >= 60055) {
			RegisterUInt16(unknown);
		}
	}

	int32_t	spell_id; //sent as -spell_id
	int16_t spell_icon; //sent as -icon_id
	int16_t	spell_triggercount; //sent like a normal value
	uint16_t	spell_icon_backdrop; //sent as (0x8000 | backdrop_id)
	uint16_t	unknown;
};

///<summary>Packet struct containing the spawns general information</summary>
struct SpawnInfoStruct {
	uint32_t						model_type;
	uint32_t						soga_model_type;
	uint32_t						equipment_types[24];
	uint32_t                        textures_slot_type;
	uint32_t						mount_adornment_type;
	uint32_t                        mount_armor_type;
	uint32_t						hair_type_id;
	uint32_t						facial_hair_type_id;
	uint32_t						wing_type_id;
	uint32_t                        tail_type_id;
	uint32_t                        soga_wing_type_id;
	uint32_t						chest_type_id;
	uint32_t                        soga_chest_type_id;
	uint32_t						legs_type_id;
	uint32_t                        soga_legs_type_id;
	uint32_t						back_slot_type_id;
	uint32_t						soga_hair_type_id;
	uint32_t						soga_facial_hair_type_id;
	uint32_t						mount_type;
	uint32_t						combat_voice;
	uint32_t						emote_voice;
	uint8_t 						unknown17[4];
	uint8_t							visual_flag;
	uint8_t							interaction_flag;
	uint8_t							flag3;
	uint8_t                         flag4;
	//BG Color, Emblem, Emblem Color, Edge, Edge Color, pattern/color (unknown order)
	uint8_t                         heraldry[7];
	uint8_t                         align_byte;
	uint8_t                         soga_heraldry[7];
	uint8_t                         align_byte2;
	uint32_t						action_state;
	uint32_t						visual_state;
	uint32_t						mood_state;
	uint32_t						emote_state;
	uint32_t                        primary_slot_state;//only visual states that have weapon in their name seem to have worked for these so far, ex: 28240 drinal_trail_weapon_poison
	uint32_t                        secondary_slot_state;
	uint32_t                        ranged_slot_state;
	uint32_t                        spell_visuals[8];
	uint8_t                         spell_visual_levels[8];
	uint32_t						target_id;
	uint32_t						follow_target;
	uint32_t						size_unknown;
	uint8_t							unknown3b[7];
	//uint8_t						spawn_type;
	//uint8_t						icon;
	//uint32_t						activity_status;
	//The first 4 bytes above were merged into EntityFlags, last 2 bytes of activityStatus into unknownpk1
	uint32_t                        entityFlags;
	uint16_t                        unknownpkShort;
	uint32_t                        unknownpk1;
	uint8_t							unknown600554[10];
	uint32_t						hp_percent; //this is sent as 100 ^ percentage remaining
	uint32_t						power_percent;
	int16_t							cast_percentage;
	int16_t                         unknown3c2;
	uint32_t                        cast_unknown;
	uint8_t							threat_level_secondary; // i think this is the threat percentage of the person 2nd on the hate list (shows up for the #1 hated's threat window for that spawn)
	uint8_t							unknown600553c;
	uint16_t						orig_level;  // This is the original level and should not change when the level value changes when mentoring
	uint16_t						level;
	uint8_t							unknown5;
	uint8_t						    heroic_flag;
	uint8_t    				    	heatLevel; //for heatvision (infravision)
	uint8_t							race;
	uint8_t							gender;
	uint8_t							adv_class;
	uint8_t							difficulty;
	uint8_t							show_archtype_icon;
	uint8_t							unknown21[2];
	uint16_t						unknownz5;
	uint8_t							unknown6[6];
	uint32_t						unknown6b;
	uint8_t							unknown18[68];
	uint8_t                         unknown2a[3];
	uint8_t                         unknown2b;
	//TODO: This contains much of the target spell effects struct I'm sure, align it for older versions
	uint8_t                         unknown3[288];
	uint32_t                        unknown13;
	uint16_t                        unknown14;
	uint16_t                        unknown15[4];
	uint8_t                         unknown19[2];
	uint32_t                        activity_timer;
	uint8_t                         unknown20[4];
	uint32_t                        unknown20a[3];
	uint8_t                         unknown13b;
	uint8_t                         body_age;
	float                           size_mod; // if set from 1.0 to -1.0 you shrink so small you can't see yourself
	uint32_t                        unknown67633;
	uint8_t                         unknown67633b;
	union {
		SpawnMorphSliders               sliders;
		int8_t                         sliderBytes[26];
	};

	union {
		SpawnMorphSliders               sogaSliders;
		int8_t                         sogaSliderBytes[26];
	};
	EQ2Color                        torch_color;

	EQ2Color						equipment_colors[24];
	EQ2Color						hair_type_color;
	EQ2Color						hair_face_color;
	EQ2Color                        textures_slot_color;
	EQ2Color						mount_adornment_color;
	EQ2Color                        mount_armor_color;
	EQ2Color                        tail_color1;
	EQ2Color                        tail_color2;
	EQ2Color						wing_color1;
	EQ2Color                        soga_wing_color1;
	EQ2Color                        chest_type_color;
	EQ2Color                        legs_type_color;
	EQ2Color                        back_slot_type_color;
	EQ2Color						unknown10[3];
	EQ2Color						equipment_highlights[24];
	EQ2Color                        textures_slot_highlight;
	EQ2Color						mount_adornment_highlight;
	EQ2Color                        mount_armor_highlight;
	EQ2Color						hair_type_highlight_color;
	EQ2Color						hair_face_highlight_color;
	EQ2Color						wing_color2;
	EQ2Color                        soga_wing_color2;
	EQ2Color						chest_type_highlight;
	EQ2Color                        legs_type_highlight;
	EQ2Color                        back_slot_type_highlight;
	EQ2Color						soga_hair_type_color;
	EQ2Color						soga_hair_type_highlight_color;
	EQ2Color						soga_hair_face_color;
	EQ2Color						soga_hair_face_highlight_color;
	EQ2Color						skin_color;
	EQ2Color						eye_color;
	EQ2Color						soga_eye_color;
	EQ2Color						soga_skin_color;
	EQ2Color						model_color;
	EQ2Color						soga_model_color;
	EQ2Color						mount_color;
	EQ2Color						mount_saddle_color;
	EQ2Color						hair_color1;
	EQ2Color						hair_color2;
	EQ2Color						hair_highlight;
	EQ2Color						soga_hair_color1;
	EQ2Color						soga_hair_color2;
	EQ2Color						soga_hair_highlight;

	SpawnInfoStruct();

protected:
	//Used for legacy clients
	uint16_t equipment_types_int16[24];
};

class Substruct_SpawnInfo : public SpawnInfoStruct, public PacketEncodedData {
public:
	Substruct_SpawnInfo(uint32_t version) : PacketEncodedData(version) {
		for (int i = 0; i < 30; i++) {
			spell_effects[i].ResetVersion(version);
		}

		RegisterElements();
	}

	Substruct_TargetSpellInfo spell_effects[30];

	//We need to syncronize the data for equipment types pre int32 versions since this is a special case
	void PostRead() override {
		PacketSubstruct::PostRead();
		if (version < 57080) {
			for (int i = 0; i < 24; i++) {
				equipment_types[i] = equipment_types_int16[i];
			}
		}
	}

	void PreWrite() override;

	void PostWrite() override {
		hp_percent ^= 100;
		
		if (version < 1188) {
			entityFlags = entity_flags_pre_write;
		}
	}

	void RegisterElements();

private:
	//Legacy clients
	uint16_t spell_visuals_do_not_set[8];
	uint32_t entity_flags_pre_write;
};

struct SpawnTitleStruct {
	SpawnTitleStruct() : isPlayer(false), unknown1(0) {}

	std::string name;
	uint8_t unknown1;
	bool isPlayer;
	std::string last_name;
	std::string suffix_title;
	std::string prefix_title;
	std::string pvp_title;
	std::string guild;
};

class Substruct_SpawnTitleStruct : public SpawnTitleStruct, public PacketSubstruct {
public:
	Substruct_SpawnTitleStruct(uint32_t version) : PacketSubstruct(version) {
		RegisterElements();
	}

	void RegisterElements() override {
		std::string& name = SpawnTitleStruct::name;
		Register16String(name);
		RegisterUInt8(unknown1);
		RegisterBool(isPlayer);
		Register16String(last_name);
		Register16String(suffix_title);
		Register16String(prefix_title);
		Register16String(pvp_title);
		Register16String(guild);
	}

	~Substruct_SpawnTitleStruct() = default;
};

class Substruct_Lerp : public PacketSubstruct {
public:

	struct Substruct_LerpWayPoint : public PacketSubstruct {
		float x;
		float y;
		float z;
		float deltaTime;

		Substruct_LerpWayPoint(uint32_t p_version = 0) : PacketSubstruct(p_version, true),
		x(0.f),
		y(0.f),
		z(0.f),
		deltaTime(0.f){
		}

		void RegisterElements() {
			RegisterFloat(x);
			RegisterFloat(y);
			RegisterFloat(z);
			RegisterFloat(deltaTime);
		}
	};

	Substruct_Lerp(uint32_t p_version = 0) : PacketSubstruct(p_version) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt32(version);
		RegisterUInt32(lerpType);
		PacketUInt32* s = RegisterUInt32(num_waypoint);
		s->SetMyArray(RegisterArray(waypoint_array, Substruct_LerpWayPoint));
	}

	uint32_t version;
	uint32_t lerpType;
	uint32_t num_waypoint;
	std::vector<Substruct_LerpWayPoint> waypoint_array;
};

class Substruct_Matrix3 : public PacketSubstruct {
public:

	Substruct_Matrix3(uint32_t p_version = 0) : PacketSubstruct(p_version) {
		RegisterElements();
	}

	void RegisterElements() {
		float& M = this->planes[0].x;
		RegisterFloat(M)->SetCount(9);
	}

	vec3 planes[3];
};

class Substruct_Position : public PacketSubstruct {
public:

	Substruct_Position(uint32_t p_version = 0) : PacketSubstruct(p_version), rotMatrix(p_version) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterFloat(yaw);
		RegisterFloat(pitch);
		RegisterFloat(roll);
		RegisterSubstruct(rotMatrix);
		RegisterFloat(scale);
		RegisterFloat(x);
		RegisterFloat(y);
		RegisterFloat(z);
	}

	Substruct_Matrix3 rotMatrix;
	float pitch;
	float yaw;
	float roll;
	float scale;
	float x;
	float y;
	float z;
};

class Substruct_SpawnFooter : public PacketSubstruct {
public:
	Substruct_SpawnFooter(uint32_t p_version) : PacketSubstruct(p_version), titleStruct(p_version), lerp1(p_version), lerp2(p_version) {
		has_lerp = false;
		spawn_type = 0;
		RegisterElements();
	}

	void RegisterElements() override {
		RegisterSubstruct(titleStruct);
		auto e = RegisterBool(has_lerp);
		RegisterSubstruct(lerp1)->SetIsVariableSet(e);
		RegisterSubstruct(lerp2)->SetIsVariableSet(e);
		RegisterUInt8(spawn_type);
	}

	Substruct_SpawnTitleStruct titleStruct;
	uint8_t spawn_type;
	bool has_lerp;
	Substruct_Lerp lerp1;
	Substruct_Lerp lerp2;
};

class Substruct_WidgetFooter : public PacketSubstruct {
public:
	Substruct_WidgetFooter(uint32_t p_version) : PacketSubstruct(p_version),
	widgetID(0),
	widgetX(0.f),
	widgetY(0.f),
	widgetZ(0.f)
	{
		RegisterElements();
	}

	void RegisterElements() override {
		RegisterUInt32(widgetID);
		RegisterFloat(widgetX);
		RegisterFloat(widgetY);
		RegisterFloat(widgetZ);
	}

	uint32_t widgetID;
	float widgetX;
	float widgetY;
	float widgetZ;
};

class Substruct_SignFooter : public PacketSubstruct {
public:
	Substruct_SignFooter(uint32_t p_version) : PacketSubstruct(p_version),
		distance(0.f),
		language(0),
		show(true),
		unkStringArray(0)
	{
		RegisterElements();
	}

	void RegisterElements() override {
		Register16String(title);
		Register16String(description);
		RegisterFloat(distance);
		RegisterBool(show);
		if (GetVersion() > 283) {
			RegisterUInt8(language);
			if (GetVersion() > 60069) {
				auto arrSize = RegisterUInt8(unkStringArrayCount);
				arrSize->SetMyArray(RegisterArray(unkStringArray, Substruct_SignFooterArray));
			}
		}
	}

	class Substruct_SignFooterArray : public PacketSubstruct {
	public:
		Substruct_SignFooterArray(uint32_t p_version = 0) : PacketSubstruct(p_version, true) {
			RegisterElements();
		}

		void RegisterElements() override {
			Register16String(unkString);
		}

		std::string unkString;
	};

	std::string title;
	std::string description;
	float distance;
	uint8_t language;
	bool show;
	uint8_t unkStringArrayCount;
	std::vector<Substruct_SignFooterArray> unkStringArray;
	
};