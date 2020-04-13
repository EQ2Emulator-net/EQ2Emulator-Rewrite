#pragma once

#include "SpawnStructs.h"
#include <string.h>
#include <atomic>
#include <memory>
#include "Widget.h"
#include "Sign.h"

//EntityFlagValues
const uint32_t EntityFlagAttackable = 1; //in combat maybe?
const uint32_t EntityFlagStaticObject = 1 << 2;
const uint32_t EntityFlagAfk = 1 << 16; // Check if afk added in 1188 or if it was in icon previously
const uint32_t EntityFlagRoleplaying = 1 << 17;
const uint32_t EntityFlagAnonymous = 1 << 18;
const uint32_t EntityFlagLinkdead = 1 << 19;
const uint32_t EntityFlagCamping = 1 << 20;
const uint32_t EntityFlagLFG = 1 << 21;
const uint32_t EntityFlagLFW = 1 << 22;
const uint32_t EntityFlagSolid = 1 << 23;
const uint32_t EntityFlagMentoring = 1 << 24;
const uint32_t EntityFlagImmunityGained = 1 << 25;
const uint32_t EntityFlagImmunityRemaining = 1 << 26;

class ZoneServer;

class Spawn : std::enable_shared_from_this<Spawn> {
public:
	Spawn();
	~Spawn();

	float GetX() const { return m_posStruct.x; }
	float GetY() const { return m_posStruct.y; }
	float GetZ() const { return m_posStruct.z; }
	uint32_t GetID() const { return m_spawnID; }
	std::string GetName() const { return m_titleStruct.name; }

	ZoneServer* GetZone() const { return m_zone; }
	void SetZone(ZoneServer* zone) { m_zone = zone; }

	const SpawnPositionStruct* GetPosStruct() const;
	const SpawnVisualizationStruct* GetVisStruct() const;
	const SpawnInfoStruct* GetInfoStruct() const;
	const SpawnTitleStruct* GetTitleStruct() const;

	struct UpdateFlags {
		bool m_posChanged : 1;
		bool m_visChanged : 1;
		bool m_infoChanged : 1;
		bool m_titleChanged : 1;
	};

	UpdateFlags PopUpdateFlags();

private:
	static uint32_t GetNextID();

	ZoneServer* m_zone;

	SpawnPositionStruct m_posStruct;
	SpawnVisualizationStruct m_visStruct;
	SpawnInfoStruct m_infoStruct;
	SpawnTitleStruct m_titleStruct;

	union {
		UpdateFlags m_updateFlags;
		uint8_t m_updateFlagsByte;
	};

	std::unique_ptr<Sign> signData;
	std::unique_ptr<Widget> widgetData;
	uint32_t m_spawnID;

public:
	/* I put the template functions down here so they aren't cluttering up the rest of the class */
	template <class Field, class Value>
	void Set(Field* field, Value value) {
		*field = static_cast<Field>(value);
	}

	template <class Field>
	void Set(Field* field, const char* value) {
		strcpy(field, value);
	}

	template <class Field, class Value>
	void SetPos(Field* field, Value value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_posChanged = true;
	}

	template <class Field, class Value>
	void SetVis(Field* field, Value value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_visChanged = true;
	}

	template <class Field, class Value>
	void SetInfo(Field* field, Value value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_infoChanged = true;
	}

	template <class Field, class Value>
	void SetTitle(Field* field, Value value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_titleChanged = true;
	}

	template <class Field>
	void SetPos(Field* field, char* value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_posChanged = true;
	}

	template <class Field>
	void SetVis(Field* field, char* value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_visChanged = true;
	}

	template <class Field>
	void SetInfo(Field* field, char* value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_infoChanged = true;
	}

	template <class Field>
	void SetTitle(Field* field, char* value, bool setUpdateFlags = true) {
		Set(field, value);

		if (setUpdateFlags)
			m_updateFlags.m_titleChanged = true;
	}


	void SetVisUnknown(uint32_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.unknown, value, updateFlags);
	}
	void SetLockedNoLoot(uint8_t new_val, bool updateFlags = true){
		SetVis(&m_visStruct.locked_no_loot, new_val, updateFlags);
	}
	void SetMercIcon(uint8_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.merc_icon, value, updateFlags);
	}
	void SetVisUnknownA(uint8_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.unknowna, value, updateFlags);
	}
	void SetVisUnknownA2(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetVis(&m_visStruct.unknowna2[index], value, updateFlags);
	}
	void SetVisFlags(uint8_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.vis_flags, value, updateFlags);
	}
	void SetVisUnknownB(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetVis(&m_visStruct.unknownb[index], value, updateFlags);
	}
	void SetHandFlag(uint8_t new_val, bool updateFlags = true){
		SetVis(&m_visStruct.hand_flag, new_val, updateFlags);
	}
	/*void SetVisUnknown2(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetVis(&m_visStruct.unknown2[index], value, updateFlags);
	}*/
	void SetVisUnknown2A(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetVis(&m_visStruct.unknown2a[index], value, updateFlags);
	}

	void SetModelType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.model_type, value, updateFlags);
	}
	void SetSogaModelType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_model_type, value, updateFlags);
	}
	void SetEquipType(uint32_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.equipment_types[index], value, updateFlags);
	}
	void SetUnknown7A(uint16_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.unknown7a[index], value, updateFlags);
	}
	void SetUnknown7AA(uint16_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.unknown7aa[index], value, updateFlags);
	}
	void SetHairType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_type_id, value, updateFlags);
	}
	void SetFacialHairType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.facial_hair_type_id, value, updateFlags);
	}
	void SetWingType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.wing_type_id, value, updateFlags);
	}
	void SetChestType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.chest_type_id, value, updateFlags);
	}
	void SetLegsType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.legs_type_id, value, updateFlags);
	}
	void SetSogaHairType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_type_id, value, updateFlags);
	}
	void SetSogaFacialHairType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_facial_hair_type_id, value, updateFlags);
	}
	void SetMountType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.mount_type, value, updateFlags);
	}
	void SetCombatVoice(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.combat_voice, value, updateFlags);
	}
	void SetEmoteVoice(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.emote_voice, value, updateFlags);
	}
	void SetInfoVisualFlags(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.visual_flag, value, updateFlags);
	}
	void SetInteractionFlag(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.interaction_flag, value, updateFlags);
	}
	void SetEquipColors(EQ2Color value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.equipment_colors[index], value, updateFlags);
	}
	void SetHairColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_type_color, value, updateFlags);
	}
	void SetFacialHairColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_face_color, value, updateFlags);
	}
	void SetWingColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.wing_color1, value, updateFlags);
	}
	void SetEquipHighlightColor(EQ2Color value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.equipment_highlights[index], value, updateFlags);
	}
	void SetHairHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_type_highlight_color, value, updateFlags);
	}
	void SetFacialHairHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_face_highlight_color, value, updateFlags);
	}
	void SetWingHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.wing_color2, value, updateFlags);
	}
	void SetSogaHairColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_type_color, value, updateFlags);
	}
	void SetSogaHairHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_type_highlight_color, value, updateFlags);
	}
	void SetSogaFacialHairColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_face_color, value, updateFlags);
	}
	void SetSogaFacialHairHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_face_highlight_color, value, updateFlags);
	}
	void SetSkinColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.skin_color, value, updateFlags);
	}
	void SetEyeColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.eye_color, value, updateFlags);
	}
	void SetSogaEyeColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_eye_color, value, updateFlags);
	}
	void SetSogaSkinColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_skin_color, value, updateFlags);
	}
	void SetSkullType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.skull[index], value, updateFlags);
	}
	void SetEyeType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.eyes[index], value, updateFlags);
	}
	void SetEarType(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.ears[index], value, updateFlags);
	}
	void SetEyeBrowType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.eyebrow[index], value, updateFlags);
	}
	void SetCheekType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.cheeks[index], value, updateFlags);
	}
	void SetLipType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.mouth[index], value, updateFlags);
	}
	void SetChinType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.chin[index], value, updateFlags);
	}
	void SetNoseType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.nose[index], value, updateFlags);
	}
	void SetBodySize(int8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.bodyscale, value, updateFlags);
	}
	void SetBodySizeUnknown(int8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sliders.bumpscale, value, updateFlags);
	}
	void SetSogaSkullType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.skull[index], value, updateFlags);
	}
	void SetSogaEyeType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.eyes[index], value, updateFlags);
	}
	void SetSogaEarType(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.ears[index], value, updateFlags);
	}
	void SetSogaEyeBrowType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.eyebrow[index], value, updateFlags);
	}
	void SetSogaCheekType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.cheeks[index], value, updateFlags);
	}
	void SetSogaLipType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.mouth[index], value, updateFlags);
	}
	void SetSogaChinType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.chin[index], value, updateFlags);
	}
	void SetSogaNoseType(int8_t value, uint8_t index, bool updateFlags = true) {
		SetInfo(&m_infoStruct.sogaSliders.nose[index], value, updateFlags);
	}
	void SetMountColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.mount_color, value, updateFlags);
	}
	void SetMountSaddleColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.mount_saddle_color, value, updateFlags);
	}
	void SetHairColor1(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_color1, value, updateFlags);
	}
	void SetHairColor2(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_color2, value, updateFlags);
	}
	void SetHairHighlight(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_highlight, value, updateFlags);
	}
	void SetSogaHairColor1(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_color1, value, updateFlags);
	}
	void SetSogaHairColor2(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_color2, value, updateFlags);
	}
	void SetSogaHairHighlight(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_hair_highlight, value, updateFlags);
	}
	void SetActionState(uint16_t state, bool updateFlags = true){
		SetInfo(&m_infoStruct.action_state, state, updateFlags);
	}
	void SetVisualState(uint16_t state, bool updateFlags = true){
		SetInfo(&m_infoStruct.visual_state, state, updateFlags);
	}
	void SetMoodState(uint16_t state, bool updateFlags = true){
		SetInfo(&m_infoStruct.mood_state, state, updateFlags);
	}
	void SetEmoteState(uint8_t new_val, bool updateFlags = true){
		SetInfo(&m_infoStruct.emote_state, new_val, updateFlags);
	}
	void SetEntityFlags(uint32_t flags, bool updateFlags = true) {
		SetInfo(&m_infoStruct.entityFlags, updateFlags);
	}
	void SetLevel(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.level, value, updateFlags);
	}
	void SetHeroicFlag(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.heroic_flag, value, updateFlags);
	}
	void SetRace(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.race, value, updateFlags);
	}
	void SetGender(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.gender, value, updateFlags);
	}
	void SetAdventureClass(uint8_t new_class, bool updateFlags = true) {
		SetInfo(&m_infoStruct.adv_class, new_class, updateFlags);
	}
	void SetDifficulty(uint8_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.difficulty, value, updateFlags);
	}

	void SetGridID(uint32_t grid, bool updateFlags = true) {
		SetPos(&m_posStruct.grid_id, grid, updateFlags);
	}
	void SetX(float x, bool updateFlags = true){
		SetPos(&m_posStruct.x, x, updateFlags);
	}
	void SetY(float y, bool updateFlags = true){
		SetPos(&m_posStruct.y, y, updateFlags);
	}
	void SetZ(float z, bool updateFlags = true){
		SetPos(&m_posStruct.z, z, updateFlags);
	}
	void SetLocation(float x, float y, float z, bool updateFlags = true) {
		SetX(x, false);
		SetY(y, false);
		SetZ(z, updateFlags);
	}
	void SetHeading(float heading, bool updateFlags = true){
		m_posStruct.heading = heading;
		SetPos(&m_posStruct.desiredHeading, heading, updateFlags);
	}
	void SetSpeed(float speed, bool updateFlags = true) {
		SetPos(&m_posStruct.desiredForwardSpeed, speed, updateFlags);
	}
	void SetState(uint32_t state, bool updateFlags = true) {
		SetPos(&m_posStruct.positionState, state, updateFlags);
	}
	void SetDestX(float x, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocX, x, updateFlags);
	}
	void SetDestY(float y, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocY, y, updateFlags);
	}
	void SetDestZ(float z, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocZ, updateFlags);
	}
	void SetDestX2(float x, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocX2, x, updateFlags);
	}
	void SetDestY2(float y, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocY2, y, updateFlags);
	}
	void SetDestZ2(float z, bool updateFlags = true) {
		SetPos(&m_posStruct.destLocZ2, z, updateFlags);
	}
	void SetMovementMode(uint8_t value, bool updateFlags = true) {
		SetPos(&m_posStruct.movementMode, value, updateFlags);
	}
	void SetPitch(float pitch, bool updateFlags = true){
		m_posStruct.pitch = pitch;
		SetPos(&m_posStruct.desiredPitch, pitch, updateFlags);
	}
	void SetCollisionRadius(uint32_t radius, bool updateFlags = true){
		SetPos(&m_posStruct.collisionRadius, radius, updateFlags);
	}
	void SetSize(float new_size, bool updateFlags = true) {
		SetPos(&m_posStruct.size, new_size, updateFlags);
	}
	void SetSizeRatio(float value, bool updateFlags = true) {
		SetPos(&m_posStruct.sizeRatio, value, updateFlags);
	}
	void SetSizeMultiplierRatio(float value, bool updateFlags = true) {
		SetPos(&m_posStruct.sizeMultiplierRatio, value, updateFlags);
	}
	void SetRoll(float roll, bool updateFlags = true){
		m_posStruct.roll = roll;
		SetPos(&m_posStruct.desiredRoll, roll, updateFlags);
	}

	void SetName(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.name, value, updateFlags);
	}
	void SetUnknown1(uint8_t value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.unknown1, value, updateFlags);
	}
	void SetIsPlayer(uint8_t value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.isPlayer, value, updateFlags);
	}
	void SetLastName(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.last_name, value, updateFlags);
	}
	void SetSuffixTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.suffix_title, value, updateFlags);
	}
	void SetPrefixTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.prefix_title, value, updateFlags);
	}
	void SetPvPTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.pvp_title, value, updateFlags);
	}
	void SetGuild(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.guild, value, updateFlags);
	}
	void SetSpawnPositionData(const SpawnPositionStruct& pos) {
		static_cast<SpawnPositionStruct&>(m_posStruct) = pos;
		m_updateFlags.m_posChanged = true;
	}
};