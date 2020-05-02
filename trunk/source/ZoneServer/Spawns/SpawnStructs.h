#pragma once

#include <stdint.h>
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketEncodedData.h"

///<summary>Packet struct containing the spawns position information</summary>
struct SpawnPositionStruct {
	SpawnPositionStruct();
	uint32_t positionState;
	uint32_t positionState2;
	float desiredHeading;
	float desiredHeadingVelocity;
	float desiredRoll;
	float desiredRollVelocity;
	float desiredPitch;
	float desiredPitchVelocity;
	float unusedUnknown;
	uint32_t grid_id;
	float x;
	float y;
	float z;
	uint8_t movementMode;
	uint8_t unkByte;
	int16_t unk72;
	float heading;
	float pitch;
	float roll;
	float destLocX;
	float destLocY;
	float destLocZ;
	float destLocX2;
	float destLocY2;
	float destLocZ2;
	//I think collision radius is like the base scale
	float collisionRadius;
	//I think this is a "temp" scale but in reality mostly permanent
	float size;
	float sizeRatio;
	uint32_t faceActorID;
	float baseLocX;
	float baseLocY;
	float baseLocZ;
	float desiredForwardSpeed;
	float speedModifier;
	float swimmingSpeedModifier;
	float unkSpeed3;
	float desiredVertSpeed;
	float desiredStrafeSpeed;
	float velocityX;
	float velocityY;
	float velocityZ;
	float moveType;
	float sizeUnknown;
	float actorStopRange;
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
	int16_t swimmingSpeedModifier_compressed;
	int16_t moveType_compressed;
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
	uint8_t		unknowna;
	uint8_t		name_quest_icon;
	uint32_t	unknowna2;
	uint8_t		unknownb[3];
	bool    	bShowHandFlag;
	uint32_t    unknown2b;
	uint32_t    unknown4;
	uint8_t		unknown2[6];
	uint8_t		unknown2a[3];
	uint8_t     pvp_difficulty;
	uint8_t     tag1;
	uint8_t     tag2;
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

//26 bytes
struct SpawnMorphSliders {
	int8_t skull[3];
	int8_t eyes[3];
	int8_t ears[3];
	int8_t eyebrow[3];
	int8_t cheeks[3];
	int8_t mouth[3];
	int8_t chin[3];
	int8_t nose[3];
	uint8_t bodyscale;
	uint8_t bumpscale;
};

///<summary>Packet struct containing the spawns general information</summary>
struct SpawnInfoStruct {
	uint32_t						model_type;
	uint32_t						soga_model_type;
	uint32_t						equipment_types[25];
	uint32_t						unknownType26;
	uint32_t						unknownType27;
	uint32_t						hair_type_id;
	uint32_t						facial_hair_type_id;
	uint32_t						wing_type_id;
	uint32_t						chest_type_id;
	uint32_t						legs_type_id;
	uint32_t						unknown_new_type_id;
	//uint32_t                        unknown_new_type_id2;
	uint32_t						soga_hair_type_id;
	uint32_t						soga_facial_hair_type_id;
	uint32_t						mount_type;
	uint32_t						combat_voice;
	uint32_t						emote_voice;
	uint8_t 						unknown17[4];
	uint8_t							visual_flag;
	uint8_t							interaction_flag;
	uint8_t							unknown60055[18];
	uint32_t						action_state;
	uint32_t						visual_state;
	uint32_t						mood_state;
	uint32_t						emote_state;
	uint8_t							unknown600552[52];
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
	uint32_t						hp_remaining;
	uint32_t						power_percent;
	float							unknown600553; // if set from 1.0 to -1.0 you shrink so small you can't see yourself
	uint8_t							unknown600553b;
	uint8_t							unknown600553c;
	uint8_t							orig_level;  // This is the original level and should not change when the level value changes when mentoring
	uint8_t							level;
	uint8_t							unknown5;
	uint16_t						heroic_flag;
	uint16_t    					unknown7;
	uint8_t                         unknown7_b;
	uint8_t							race;
	uint8_t							gender;
	uint8_t							adv_class;
	uint8_t							difficulty;
	uint8_t							unknown21[3];
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
	uint8_t                         unknown14a[18];
	uint8_t                         body_age;
	uint32_t                        size_mod;
	uint8_t                         unknown67633[3];
	uint8_t                         unknown67633b;
	union {
		SpawnMorphSliders               sliders;
		int8_t                         sliderBytes[26];
	};

	union {
		SpawnMorphSliders               sogaSliders;
		int8_t                         sogaSliderBytes[26];
	};

	//The above data is zeroed out
	EQ2Color						equipment_colors[25];
	EQ2Color						hair_type_color;
	EQ2Color						hair_face_color;
	EQ2Color						unknownType26Color;
	EQ2Color                        unknownType27Color;
	EQ2Color						wing_color1;
	EQ2Color                        soga_wing_color1;
	EQ2Color                        chest_type_color;
	EQ2Color                        legs_type_color;
	EQ2Color                        unknown_new_type_color;
	EQ2Color						unknown10[3];
	EQ2Color						equipment_highlights[25];
	EQ2Color						unknownType26Highlight;
	EQ2Color                        unknownType27Highlight;
	EQ2Color						hair_type_highlight_color;
	EQ2Color						hair_face_highlight_color;
	EQ2Color						wing_color2;
	EQ2Color                        soga_wing_color2;
	EQ2Color						chest_type_highlight;
	EQ2Color                        legs_type_highlight;
	EQ2Color                        unknown_new_type_highlight;
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
	EQ2Color						mount_color;
	EQ2Color						mount_saddle_color;
	EQ2Color						hair_color1;
	EQ2Color						hair_color2;
	EQ2Color						hair_highlight;
	EQ2Color						soga_hair_color1;
	EQ2Color						soga_hair_color2;
	EQ2Color						soga_hair_highlight;

	SpawnInfoStruct() {
		//Hack to zero out most of the data allowing default values for the eq2 colors
		memset(&model_type, 0, reinterpret_cast<size_t>(equipment_colors) - reinterpret_cast<size_t>(&model_type));
	}

protected:
	//Used for legacy clients
	uint16_t equipment_types_int16[25];
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
	bool ReadElement(const unsigned char* srcbuf, uint32_t& offset, uint32_t bufsize) override {
		bool ret = PacketEncodedData::ReadElement(srcbuf, offset, bufsize);

		if (version < 57080) {
			for (int i = 0; i < 25; i++) {
				equipment_types[i] = equipment_types_int16[i];
			}
		}

		return ret;
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		if (version < 57080) {
			for (int i = 0; i < 25; i++) {
				equipment_types_int16[i] = static_cast<uint16_t>(equipment_types[i]);
			}
		}

		return PacketEncodedData::WriteElement(outbuf, offset);
	}

	void RegisterElements();
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