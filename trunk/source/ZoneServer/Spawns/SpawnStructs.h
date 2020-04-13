#pragma once

#include <stdint.h>
#include <string>
#include "../../common/Packets/PacketElements/PacketEQ2Color.h"
#include "../../common/Packets/PacketElements/PacketElements.h"
#include "../../common/Packets/PacketElements/PacketEncodedData.h"

///<summary>Packet struct containing the spawns position information</summary>
struct SpawnPositionStruct {
	SpawnPositionStruct() {
		memset(this, 0, sizeof(*this));
		sizeUnknown = 3.f;
	}

	uint32_t positionState;
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
	float unk20;
	float destLocX;
	float destLocY;
	float destLocZ;
	float destLocX2;
	float destLocY2;
	float destLocZ2;
	float size;
	float collisionRadius;
	float sizeRatio;
	float sizeMultiplierRatio;
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
	float unknown20;
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

	//Many elements in this packet are compressed from a float (4 bytes) to a short (2 bytes). Handle those conversions here
	void CompressData() {
		heading_compressed = CompressFloat(heading, 64);
		pitch_compressed = CompressFloat(pitch, 64);
		roll_compressed = CompressFloat(roll, 64);
		velocityX_compressed = CompressFloat(velocityX, 32);
		velocityY_compressed = CompressFloat(velocityY, 32);
		velocityZ_compressed = CompressFloat(velocityZ, 32);
		if (GetVersion() > 283) {
			baseLocX_compressed = CompressFloat(baseLocX, 16384);
			baseLocY_compressed = CompressFloat(baseLocY, 16384);
			baseLocZ_compressed = CompressFloat(baseLocZ, 16834);
		}
		desiredHeading_compressed = CompressFloat(desiredHeading, 64);
		desiredPitch_compressed = CompressFloat(desiredPitch, 64);
		desiredPitchVelocity_compressed = CompressFloat(desiredPitchVelocity, 32);
		desiredHeadingVelocity_compressed = CompressFloat(desiredHeadingVelocity, 32);
		desiredRollVelocity_compressed = CompressFloat(desiredPitchVelocity, 32);
		collisionRadius_compressed = CompressFloat(collisionRadius, 32);
		speedModifier_compressed = CompressFloat(speedModifier, 32);
		swimmingSpeedModifier_compressed = CompressFloat(speedModifier, 32);
		unkSpeed3_compressed = CompressFloat(unkSpeed3, 32);
		desiredVertSpeed_compressed = CompressFloat(desiredVertSpeed, 32);
		desiredStrafeSpeed_compressed = CompressFloat(desiredStrafeSpeed, 32);
		moveType_compressed = CompressFloat(moveType, 32);
		desiredForwardSpeed_compressed = CompressFloat(desiredForwardSpeed, 256);
		desiredRoll_compressed = CompressFloat(desiredRoll, 256);
		desiredRollVelocity_compressed = CompressFloat(desiredRollVelocity, 256);
		size_compressed = CompressFloat(size, 32);
		if (version > 910) {
			//Looks like size moved after 910 back 2 bytes where collisionRadius was prior and they thought this element was size
			//Not sure what this is but has a massive compression ratio so can only be low number values
			sizeUnknown_compressed = CompressFloat(sizeUnknown, 16384);
		}
		unk70_compressed = CompressFloat(unk70, 256);
		actorStopRange_compressed = CompressFloat(actorStopRange, 32);
	}

	inline float ExpandFloat(const int16_t& i, const float& factor) {
		return static_cast<float>(i / factor);
	}

	void DecompressData() {
		heading = ExpandFloat(heading_compressed, 64);
		pitch = ExpandFloat(pitch_compressed, 64);
		roll = ExpandFloat(roll_compressed, 64);
		velocityX = ExpandFloat(velocityX_compressed, 32);
		velocityY = ExpandFloat(velocityY_compressed, 32);
		velocityZ = ExpandFloat(velocityZ_compressed, 32);
		if (GetVersion() > 283) {
			baseLocX = ExpandFloat(baseLocX_compressed, 16384);
			baseLocY = ExpandFloat(baseLocY_compressed, 16384);
			baseLocZ = ExpandFloat(baseLocZ_compressed, 16384);
		}
		desiredHeading = ExpandFloat(desiredHeading_compressed, 64);
		desiredPitch = ExpandFloat(desiredPitch_compressed, 64);
		desiredPitchVelocity = ExpandFloat(desiredPitchVelocity_compressed, 32);
		desiredHeadingVelocity = ExpandFloat(desiredHeadingVelocity_compressed, 32);
		desiredRollVelocity = ExpandFloat(desiredRollVelocity_compressed, 32);
		collisionRadius = ExpandFloat(collisionRadius_compressed, 32);
		speedModifier = ExpandFloat(speedModifier_compressed, 32);
		swimmingSpeedModifier = ExpandFloat(swimmingSpeedModifier_compressed, 32);
		unkSpeed3 = ExpandFloat(unkSpeed3_compressed, 32);
		desiredVertSpeed = ExpandFloat(desiredVertSpeed_compressed, 32);
		desiredStrafeSpeed = ExpandFloat(desiredStrafeSpeed_compressed, 32);
		moveType = ExpandFloat(moveType_compressed, 32);
		desiredForwardSpeed = ExpandFloat(desiredForwardSpeed_compressed, 256);
		desiredRoll = ExpandFloat(desiredRoll_compressed, 256);
		desiredRollVelocity = ExpandFloat(desiredRollVelocity_compressed, 256);
		size = ExpandFloat(size_compressed, 32);
		if (version > 910) {
			sizeUnknown = ExpandFloat(sizeUnknown_compressed, 16384);
		}
		unk70 = ExpandFloat(unk70_compressed, 256);
		actorStopRange = ExpandFloat(actorStopRange_compressed, 32);
	}

	void RegisterElements() override {
		RegisterUInt32(grid_id);
		if (GetVersion() > 283) {
			RegisterFloat(x);
			RegisterFloat(y);
			RegisterFloat(z);
		}
		else {
			RegisterInt16(x_compressed);
			RegisterInt16(y_compressed);
			RegisterInt16(z_compressed);
		}
		RegisterInt16(velocityX_compressed);
		RegisterInt16(velocityY_compressed);
		RegisterInt16(velocityZ_compressed);
		RegisterInt16(heading_compressed);
		RegisterInt16(desiredHeading_compressed);
		RegisterInt16(desiredForwardSpeed_compressed);
		RegisterUInt32(positionState);
		if (GetVersion() > 283) {
			RegisterFloat(unk20);
			RegisterFloat(destLocX);
			RegisterFloat(destLocY);
			RegisterFloat(destLocZ);
			RegisterFloat(destLocX2);
			RegisterFloat(destLocY2);
			RegisterFloat(destLocZ2);
			RegisterUInt8(movementMode);
			RegisterUInt8(unkByte);
		}
		else {
			RegisterUInt8(movementMode);
			RegisterInt16(destX_compressed);
			RegisterInt16(destY_compressed);
			RegisterInt16(destZ_compressed);
			RegisterInt16(destX2_compressed);
			RegisterInt16(destY2_compressed);
			RegisterInt16(destZ2_compressed);
		}
		RegisterInt16(desiredHeadingVelocity_compressed);
		RegisterInt16(speedModifier_compressed);
		RegisterInt16(swimmingSpeedModifier_compressed);
		if (GetVersion() > 283) {
			RegisterInt16(moveType_compressed);
		}
		RegisterInt16(desiredStrafeSpeed_compressed);
		RegisterInt16(desiredVertSpeed_compressed);
		RegisterInt16(unkSpeed3_compressed);
		if (GetVersion() > 283) {
			RegisterInt16(desiredRoll_compressed);
			RegisterInt16(desiredRollVelocity_compressed);
		}
		RegisterInt16(desiredPitch_compressed);
		RegisterInt16(desiredPitchVelocity_compressed);
		if (GetVersion() > 283) {
			RegisterInt16(baseLocX_compressed);
			RegisterInt16(baseLocY_compressed);
			RegisterInt16(baseLocZ_compressed);
		}
		else {
			RegisterFloat(baseLocX);
			RegisterFloat(baseLocY);
			RegisterFloat(baseLocZ);
		}
		RegisterInt16(pitch_compressed);
		if (GetVersion() <= 910) {
			RegisterInt16(collisionRadius_compressed);
			RegisterInt16(size_compressed);
		}
		else {
			RegisterInt16(size_compressed);
			RegisterInt16(sizeUnknown_compressed);
		}
		
		if (GetVersion() > 283) {
			RegisterFloat(sizeRatio);
			RegisterFloat(sizeMultiplierRatio);
		}
		RegisterUInt32(faceActorID);
		RegisterInt16(actorStopRange_compressed);
		if (GetVersion() > 283) {
			RegisterInt16(roll_compressed);
			RegisterInt16(unk70_compressed);
			RegisterInt16(unk72);
		}
	}

	void WriteElement(unsigned char* outbuf, uint32_t& offset) override {
		CompressData();
		PacketEncodedData::WriteElement(outbuf, offset);
	}

	bool ReadElement(const unsigned char* buf, uint32_t& offset, uint32_t bufsize) override {
		bool ret = PacketEncodedData::ReadElement(buf, offset, bufsize);
		DecompressData();
		return ret;
	}
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
	uint8_t		unknownb[4];
	uint8_t		hand_flag;
	uint8_t		unknown2[8];
	uint8_t		tag1;
	uint8_t		tag2;
	uint8_t		unknown2a[3];
	uint8_t     pvp_difficulty;
	uint8_t     unknown3[3];

	SpawnVisualizationStruct() {
		memset(this, 0, sizeof(SpawnVisualizationStruct));
	}
};

class Substruct_SpawnVisualization : public SpawnVisualizationStruct, public PacketEncodedData {
public:
	Substruct_SpawnVisualization(uint32_t version) : PacketEncodedData(version) {
		RegisterElements();
	}

	void RegisterElements() {
		if (version >= 60055) {
			RegisterUInt32(unknown);
		}
		RegisterUInt8(arrow_color);
		RegisterUInt8(locked_no_loot);
		RegisterInt8(npc_con);
		RegisterUInt8(quest_flag);
		if (version >= 1188) {
			RegisterUInt8(merc_icon);
		}
		if (version >= 936) {
			RegisterUInt8(npc_hate);
		}
		if (version >= 1142) {
			RegisterUInt8(unknowna);
		}
		if (version >= 1188) {
			RegisterUInt8(name_quest_icon);
			uint8_t& unknowna2 = this->unknowna2[0];
			RegisterUInt8(unknowna2)->SetCount(4);
		}
		RegisterUInt8(vis_flags);
		uint8_t& unknownb = this->unknownb[0];
		if (version < 1142) {
			RegisterUInt8(unknownb)->SetCount(4);
		}
		else {
			RegisterUInt8(npc_aaxp);
			RegisterUInt8(unknownb);
			RegisterUInt8(pvp_difficulty);
		}
		RegisterUInt8(hand_flag);
		uint8_t& unknown2 = this->unknown2[0];
		if (version >= 60055) {
			RegisterUInt8(unknown2)->SetCount(2);
		}
		else if (version == 936 || version >= 1188) {
			RegisterUInt8(unknown2)->SetCount(6);
		}
		else if (version == 955) {
			RegisterUInt8(unknown2)->SetCount(5);
		}
		else if (version < 1188) {
			RegisterUInt8(unknown2)->SetCount(8);
		}

		if (version >= 1188) {
			RegisterUInt8(tag1);
			RegisterUInt8(tag2);
			uint8_t& unknown3 = this->unknown3[0];
			RegisterUInt8(unknown3)->SetCount(3);
		}
	}
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
	uint16_t						unknown7a[2];
	uint16_t						unknown7aa[2];
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
	float							unknown600553;
	uint8_t							unknown600553b;
	uint8_t							unknown600553c;
	uint8_t							unknown4;
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
	EQ2Color						unknown7b[2];
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
		memset(&model_type, 0, reinterpret_cast<uint64_t>(equipment_colors) - reinterpret_cast<uint64_t>(&model_type));
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
		bool ret = PacketSubstruct::ReadElement(srcbuf, offset, bufsize);

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

		return PacketSubstruct::WriteElement(outbuf, offset);
	}

	void RegisterElements() {
		if (version < 60055) {
			if (version < 1188) {
				uint8_t& hp_remaining = reinterpret_cast<uint8_t&>(this->hp_remaining);
				RegisterUInt8(hp_remaining);
				RescopeArrayElement(unknown2a);
				RegisterUInt8(unknown2a)->SetCount(3);
				uint8_t& power_percent = reinterpret_cast<uint8_t&>(this->power_percent);
				RegisterUInt8(power_percent);
				RegisterUInt8(unknown2b);
			}
			else {
				RegisterUInt32(hp_remaining);
				RegisterUInt32(power_percent);
			}
			RescopeArrayElement(unknown3);
			if (version < 936) {
				RegisterUInt8(unknown3)->SetCount(288);
			}
			else {
				if (version < 1096) {
					RegisterUInt8(unknown3)->SetCount(258);
				}
				else {
					if (version < 1188) {
						RegisterUInt8(unknown3)->SetCount(242);
					}
					else {
						RescopeArrayElement(spell_effects);
						RegisterSubstruct(spell_effects)->SetCount(30);
					}
					RegisterUInt32(target_id);
					RegisterUInt32(follow_target);
					RescopeArrayElement(unknown3b);
					RegisterUInt8(unknown3b)->SetCount(version >= 1198 ? 8 : 7);
					if (version < 1198) {
						RescopeToReference(heroic_flag, uint8_t);
						RegisterUInt8(heroic_flag);
					}
				}
			}
			if (version < 936) {
				//These were all moved further in the packet
				RegisterUInt8(adv_class);
				RegisterUInt8(unknown4);
				RegisterUInt8(level);
				RegisterUInt8(difficulty);
				RegisterUInt8(unknown5);
				RescopeToReference(heroic_flag, uint8_t);
				RegisterUInt8(heroic_flag);
			}
			RegisterUInt32(entityFlags);
			RegisterUInt16(unknownpkShort);
			if (version >= 57080) {
				RegisterUInt16(unknownz5);
			}
			RescopeArrayElement(unknown6);
			if (version < 936) {
				RegisterUInt8(unknown6)->SetCount(6);
			}
			else {
				if (version >= 955) {
					RegisterUInt8(unknown6)->SetCount(3);
				}
				else {
					RegisterUInt8(unknown6)->SetCount(2);
				}
				RegisterUInt8(unknown4);
				RegisterUInt8(level);
				RegisterUInt8(unknown5);
				if (version < 1096 || version >= 1188) {
					//Thinking this got moved by accident at some point then moved to the correct location again
					RegisterUInt16(heroic_flag);
				}
				RegisterUInt32(activity_timer);
			}
			RegisterUInt8(unknown7_b);
			RegisterUInt16(unknown7);
		}
		if (version < 57080) {
			uint16_t& model_type = reinterpret_cast<uint16_t&>(this->model_type);
			RegisterUInt16(model_type);
			if (version > 283) {
				uint16_t& soga_model_type = reinterpret_cast<uint16_t&>(this->soga_model_type);
				RegisterUInt16(soga_model_type);
			}
		}
		else {
			RegisterUInt32(model_type);
			RegisterUInt32(soga_model_type);
		}
		if (version < 60055) {
			RegisterEQ2Color(skin_color);
			RegisterEQ2Color(eye_color);
			if (version > 283) {
				RegisterEQ2Color(soga_eye_color);
				RegisterEQ2Color(soga_skin_color);
				if (version > 860) {
					RegisterEQ2Color(kunark_unknown_color1);
					RegisterEQ2Color(kunark_unknown_color2);
				}
			}
		}
		if (version < 57080) {
			uint16_t& equipment_types = reinterpret_cast<uint16_t&>(this->equipment_types_int16[0]);
			RegisterUInt16(equipment_types)->SetCount(25);
		}
		else {
			RescopeArrayElement(equipment_types);
			RegisterUInt32(equipment_types)->SetCount(25);
		}
		if (version >= 1188) {
			RescopeArrayElement(unknown7a);
			RegisterUInt16(unknown7a)->SetCount(2);
			if (version >= 57080) {
				RescopeArrayElement(unknown7aa);
				RegisterUInt16(unknown7aa)->SetCount(2);
			}
		}

		if (version < 57080) {
			RescopeToReference(hair_type_id, uint16_t);
			RegisterUInt16(hair_type_id);
			RescopeToReference(facial_hair_type_id, uint16_t);
			RegisterUInt16(facial_hair_type_id);
			if (version > 283) {
				RescopeToReference(wing_type_id, uint16_t);
				RegisterUInt16(wing_type_id);
			}
			RescopeToReference(chest_type_id, uint16_t);
			RegisterUInt16(chest_type_id);
			RescopeToReference(legs_type_id, uint16_t);
			RegisterUInt16(legs_type_id);

		}
		else {
			RegisterUInt32(hair_type_id);
			RegisterUInt32(facial_hair_type_id);
			RegisterUInt32(wing_type_id);
			RegisterUInt32(chest_type_id);
			RegisterUInt32(legs_type_id);
		}
		
		if (version >= 996) {
			if (version < 57080) {
				RescopeToReference(unknown_new_type_id, uint16_t);
				RegisterUInt16(unknown_new_type_id);
			}
			else {
				RegisterUInt32(unknown_new_type_id);
				//RegisterUInt32(unknown_new_type_id2);
			}
		}

		if (version >= 57080) {
			RegisterUInt32(soga_hair_type_id);
			RegisterUInt32(soga_facial_hair_type_id);
		}
		else if (version > 283) {
			RescopeToReference(soga_hair_type_id, uint16_t);
			RegisterUInt16(soga_hair_type_id);
			RescopeToReference(soga_facial_hair_type_id, uint16_t);
			RegisterUInt16(soga_facial_hair_type_id);
		}

		if (version >= 60055) {
			RegisterUInt32(mount_type);
			RegisterUInt32(combat_voice);
			RegisterUInt32(emote_voice);
			RescopeArrayElement(unknown17);
			RegisterUInt8(unknown17)->SetCount(4);
			RegisterUInt8(visual_flag);
			RegisterUInt8(interaction_flag);
			RescopeArrayElement(unknown60055);
			RegisterUInt8(unknown60055)->SetCount(18);
		}

		int32_t colorCount = (GetVersion() > 283 ? 25 : 21);
		RescopeArrayElement(equipment_colors);
		RegisterEQ2Color(equipment_colors)->SetCount(colorCount);
		if (version >= 1188) {
			RescopeArrayElement(unknown7b);
			RegisterEQ2Color(unknown7b)->SetCount(2);
		}
		RegisterEQ2Color(hair_type_color);
		RegisterEQ2Color(hair_face_color);
		if (version > 283) {
			RegisterEQ2Color(wing_color1);
		}
		RescopeArrayElement(unknown10);
		if (version < 996) {
			RegisterEQ2Color(unknown10)->SetCount(2);
		}
		else {
			RegisterEQ2Color(unknown10)->SetCount(3);
		}
		RescopeArrayElement(equipment_highlights);
		RegisterEQ2Color(equipment_highlights)->SetCount(25);
		if (version >= 1188) {
			RescopeArrayElement(unknown7c);
			RegisterEQ2Color(unknown7c)->SetCount(2);
		}
		RegisterEQ2Color(hair_type_highlight_color);
		RegisterEQ2Color(hair_face_highlight_color);
		RegisterEQ2Color(wing_color2);
		RescopeArrayElement(unknown11);
		if (version < 996) {
			RegisterEQ2Color(unknown11)->SetCount(2);
		}
		else {
			RegisterEQ2Color(unknown11)->SetCount(3);
		}

		if (version > 283) {
			RegisterEQ2Color(soga_hair_type_color);
			RegisterEQ2Color(soga_hair_type_highlight_color);
			RegisterEQ2Color(soga_hair_face_color);
			RegisterEQ2Color(soga_hair_face_highlight_color);

			if (version >= 60055) {
				RegisterEQ2Color(skin_color);
				RegisterEQ2Color(eye_color);
				RegisterEQ2Color(soga_eye_color);
				RegisterEQ2Color(soga_skin_color);
				RegisterEQ2Color(kunark_unknown_color1);
				RegisterEQ2Color(kunark_unknown_color2);
			}
		}

		RescopeArrayElement(sliderBytes);
		RegisterInt8(sliderBytes)->SetCount(26);
		
		if (version > 283) {
			RescopeArrayElement(sogaSliderBytes);
			RegisterInt8(sogaSliderBytes)->SetCount(26);
		}

		if (version < 1188) {
			uint16_t& mount_type = reinterpret_cast<uint16_t&>(this->mount_type);
			RegisterUInt16(mount_type);
			RegisterEQ2Color(mount_color);
			RegisterEQ2Color(mount_saddle_color);
		}
		else {
			RegisterEQ2Color(mount_color);
			RegisterEQ2Color(mount_saddle_color);
		}

		RegisterEQ2Color(hair_color1);
		RegisterEQ2Color(hair_color2);
		RegisterEQ2Color(hair_highlight);
		if (version > 283) {
			RegisterEQ2Color(soga_hair_color1);
			RegisterEQ2Color(soga_hair_color2);
			RegisterEQ2Color(soga_hair_highlight);
		}
		if (version < 60055) {
			if (version >= 1188) {
				if (version < 57080) {
					RescopeToReference(mount_type, uint16_t);
					RegisterUInt16(mount_type);
				}
				else {
					RegisterUInt32(mount_type);
				}
				RegisterEQ2Color(mount_color);
				RegisterEQ2Color(mount_saddle_color);
				RescopeArrayElement(unknown14a);
				RegisterUInt8(unknown14a)->SetCount(18);
			}
			if (version < 57080) {
				uint16_t& combat_voice = reinterpret_cast<uint16_t&>(this->combat_voice);
				RegisterUInt16(combat_voice);
				uint16_t& emote_voice = reinterpret_cast<uint16_t&>(this->emote_voice);
				RegisterUInt16(emote_voice);
			}
			else {
				RegisterUInt32(combat_voice);
				RegisterUInt32(emote_voice);
			}
			RescopeArrayElement(unknown17);
			if (version < 864 || (version >= 1188 && version < 57080)) {
				RegisterUInt8(unknown17)->SetCount(2);
			}
			else if ((version >= 1096 && version < 1188) || version >= 57080) {
				RegisterUInt8(unknown17)->SetCount(4);
			}
			RegisterUInt8(visual_flag);
			RegisterUInt8(interaction_flag);

			RescopeArrayElement(unknown18);
			if (version >= 57080) {

				RegisterUInt8(unknown18)->SetCount(46);
			}
			else if (version >= 864) {
				RegisterUInt8(unknown18)->SetCount(30);
			}
			else if (version >= 860) {
				RegisterUInt8(unknown18)->SetCount(36);
			}
			else {
				RegisterUInt8(unknown18)->SetCount(32);
			}
		}

		if (version < 57080) {
			uint16_t& action_state = reinterpret_cast<uint16_t&>(this->action_state);
			RegisterUInt16(action_state);
			uint16_t& visual_state = reinterpret_cast<uint16_t&>(this->visual_state);
			RegisterUInt16(visual_state);
			uint16_t& mood_state = reinterpret_cast<uint16_t&>(this->mood_state);
			RegisterUInt16(mood_state);
			if (version >= 860) {
				uint16_t& emote_state = reinterpret_cast<uint16_t&>(this->emote_state);
				RegisterUInt16(emote_state);
				if (version < 864) {
					RescopeArrayElement(unknown19);
					RegisterUInt8(unknown19)->SetCount(2);
				}
			}
		}
		else {
			RegisterUInt32(action_state);
			RegisterUInt32(visual_state);
			RegisterUInt32(mood_state);
			RegisterUInt32(emote_state);
		}

		if (version >= 60055) {
			RescopeArrayElement(unknown600552);
			RegisterUInt8(unknown600552)->SetCount(52);
			RescopeArrayElement(spell_effects);
			RegisterSubstruct(spell_effects)->SetCount(30);
			RegisterUInt32(target_id);
			RegisterUInt32(follow_target);
			RegisterUInt32(size_unknown);
			RescopeArrayElement(unknown3b);
			RegisterUInt8(unknown3b)->SetCount(4);
			RegisterUInt32(entityFlags);
			RegisterUInt32(unknownpk1);
			RegisterUInt32(activity_timer);
			RescopeArrayElement(unknown600554);
			RegisterUInt8(unknown600554)->SetCount(4);
			RegisterUInt32(hp_remaining);
			RegisterUInt32(power_percent);
			RegisterFloat(unknown600553);
			RegisterUInt32(size_mod);
			RegisterUInt8(unknown600553b);
			RegisterUInt8(unknown4);
			if (version >= 67633) {
				RescopeArrayElement(unknown67633);
				RegisterUInt8(unknown67633)->SetCount(3);
				RegisterUInt8(unknown67633b);
			}
			RegisterUInt8(level);
			RegisterUInt8(unknown5);
			RegisterUInt16(heroic_flag);
			RegisterUInt8(unknown7_b);
			RegisterUInt16(unknown7);
		}

		RegisterUInt8(race);
		RegisterUInt8(gender);

		if (version >= 936) {
			RegisterUInt8(adv_class);
			RegisterUInt8(difficulty);

			if (version >= 60055) {
				RescopeArrayElement(unknown21);
				RegisterUInt8(unknown21)->SetCount(3);
				//RegisterUInt32(activity_timer);
				//RescopeArrayElement(unknown18);
				//RegisterUInt8(unknown18)->SetCount(68);
			}
			else {
				RescopeArrayElement(unknown20);
				if (version == 945) {
					//One of these bytes appears to have moved higher in the packet after this version
					RegisterUInt8(unknown20)->SetCount(4);
				}
				else {
					if (version >= 1096) {
						RescopeArrayElement(unknown20a);
						if (version >= 57080) {
							RegisterUInt32(unknown20a)->SetCount(5);
						}
						else if (version >= 1188) {
							RegisterUInt32(unknown20a)->SetCount(3);
						}
						else {
							RegisterUInt32(unknown20a);
						}
					}
					if (version < 57080) {
						RegisterUInt8(unknown20)->SetCount(3);
					}
					else if (version < 58618) {
						RegisterUInt8(unknown20);
					}
					else {
						RegisterUInt8(unknown20)->SetCount(5);
					}
				}
			}
		}
	}
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