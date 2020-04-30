#pragma once

#include "SpawnStructs.h"
#include <string.h>
#include <atomic>
#include <memory>
#include "Widget.h"
#include "Sign.h"

//EntityFlagValues
const uint32_t EntityFlagAlive = 1;
const uint32_t EntityIsNpc = 1 << 1;
const uint32_t EntityIsMercenary = 1 << 2;
const uint32_t EntityFlagStaticObject = 1 << 3;
const uint32_t EntityFlagMerchant = 1 << 4;
const uint32_t EntityFlagShowLevel = 1 << 9; // This also hides/shows the health bar
const uint32_t EntityFlagInteractable = 1 << 10; //shows the hand icon
const uint32_t EntityFlagTargetable = 1 << 11;
const uint32_t EntityFlagIsTransport = 1 << 12; //guess
const uint32_t EntityFlagShowCommandIcon = 1 << 13; //Shows various icons depending on the spawn's entity commands
const uint32_t EntityFlagLootable = 1 << 14;
const uint32_t EntityFlagInCombat = 1 << 15;
const uint32_t EntityFlagAfk = 1 << 16; // Check if afk added in 1188 or if it was in icon previously
const uint32_t EntityFlagRoleplaying = 1 << 17;
const uint32_t EntityFlagAnonymous = 1 << 18;
const uint32_t EntityFlagLinkdead = 1 << 19;
const uint32_t EntityFlagCamping = 1 << 20;
const uint32_t EntityFlagLFG = 1 << 21;
const uint32_t EntityFlagLFW = 1 << 22;
const uint32_t EntityFlagSolid = 1 << 23;
const uint32_t EntityFlagMentoring = 1 << 28;
const uint32_t EntityFlagWeaponsEquipped = 1 << 29;
const uint32_t EntityFlagImmunityGained = 1 << 30;
const uint32_t EntityFlagImmunityRemaining = 1 << 31;

//Info vis flags
const uint8_t INFO_VIS_FLAG_INVIS = 1;
const uint8_t INFO_VIS_FLAG_HIDE_HOOD = 1 << 1;
const uint8_t INFO_VIS_FLAG_MOUNTED = 1 << 2;
const uint8_t INFO_VIS_FLAG_CROUCH = 1 << 3;


class ZoneServer;
class Client;

class Spawn : public std::enable_shared_from_this<Spawn> {
public:
	Spawn();
	Spawn(std::shared_ptr<Spawn> in);
	~Spawn();

	bool IsMercenary() { return (GetInfoStruct()->entityFlags & EntityIsMercenary) != 0; }
	bool IsStaticObject() { return (GetInfoStruct()->entityFlags & EntityFlagStaticObject) != 0; }
	bool IsMerchant() { return (GetInfoStruct()->entityFlags & EntityFlagMerchant) != 0; }
	bool IsShowLevel() { return (GetInfoStruct()->entityFlags & EntityFlagShowLevel) != 0; }
	bool IsInteractable() { return (GetInfoStruct()->entityFlags & EntityFlagInteractable) != 0; }
	bool IsTargetable() { return (GetInfoStruct()->entityFlags & EntityFlagTargetable) != 0; }
	bool IsTransport() { return (GetInfoStruct()->entityFlags & EntityFlagIsTransport) != 0; }
	bool IsLootable() { return (GetInfoStruct()->entityFlags & EntityFlagLootable) != 0; }
	bool IsInCombat() { return (GetInfoStruct()->entityFlags & EntityFlagInCombat) != 0; }
	bool IsAFK() { return (GetInfoStruct()->entityFlags & EntityFlagAfk) != 0; }
	bool IsRoleplaying() { return (GetInfoStruct()->entityFlags & EntityFlagRoleplaying) != 0; }
	bool IsAnonymous() { return (GetInfoStruct()->entityFlags & EntityFlagAnonymous) != 0; }
	bool IsLinkdead() { return (GetInfoStruct()->entityFlags & EntityFlagLinkdead) != 0; }
	bool IsCamping() { return (GetInfoStruct()->entityFlags & EntityFlagCamping) != 0; }
	bool IsLFG() { return (GetInfoStruct()->entityFlags & EntityFlagLFG) != 0; }
	bool IsLFW() { return (GetInfoStruct()->entityFlags & EntityFlagLFW) != 0; }
	bool IsSolid() { return (GetInfoStruct()->entityFlags & EntityFlagSolid) != 0; }
	bool IsMentoring() { return (GetInfoStruct()->entityFlags & EntityFlagMentoring) != 0; }
	bool IsWeaponsEquipped() { return (GetInfoStruct()->entityFlags & EntityFlagWeaponsEquipped) != 0; }
	bool IsImmunityGained() { return (GetInfoStruct()->entityFlags & EntityFlagImmunityGained) != 0; }
	bool IsImmunityRemaining() { return (GetInfoStruct()->entityFlags & EntityFlagImmunityRemaining) != 0; }

	float GetX() const { return m_posStruct.x; }
	float GetY() const { return m_posStruct.y; }
	float GetZ() const { return m_posStruct.z; }
	uint32_t GetID() const { return m_spawnID; }
	uint32_t GetDatabaseID() { return m_spawnDatabaseID; }
	void SetDatabaseID(uint32_t id) { m_spawnDatabaseID = id; }
	std::string GetName() const { return m_titleStruct.name; }

	std::shared_ptr<ZoneServer> GetZone() const { return m_zone.lock(); }
	void SetZone(std::shared_ptr<ZoneServer> zone) { m_zone = zone; }

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

	struct BaseViewerFlags {
		bool bAttackable : 1;
		bool bTargetable : 1;
		bool bShowLevel : 1;
		bool bShowCommandIcon : 1;
		bool bDisplayHandIcon : 1;
	};

	void Process();
	void AddClient(std::weak_ptr<Client> client);
	void RemoveClient(std::weak_ptr<Client> client);
	
	std::unique_ptr<Sign>& GetSignData() { return signData; }
	std::unique_ptr<Widget>& GetWidgetData() { return widgetData; }

private:
	static uint32_t GetNextID();

	std::weak_ptr<ZoneServer> m_zone;

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
	uint32_t m_spawnDatabaseID;

	std::vector<std::weak_ptr<Client> > m_clients;
	uint32_t movementTimestamp;

	float m_sizeOffset;
	uint32_t m_primaryCommandListID;
	uint32_t m_secondaryCommandListID;
	uint32_t m_factionID;
	// These may need to be changed
	uint32_t m_hp;
	uint32_t m_power;
	uint32_t m_savagery;
	uint32_t m_dissonance;
	uint32_t m_merchantID;
	uint32_t m_merchantType;

	uint32_t m_spawnLocationID;
	uint32_t m_spawnEntryID;
	uint32_t m_respawnTime;
	uint32_t m_expireTime;

	uint32_t m_spawnLocationPlacementID;
	float m_origX;
	float m_origY;
	float m_origZ;
	float m_origHeading;
	float m_origPitch;
	float m_origRoll;
	std::pair<int32_t, int32_t> m_currentCellCoordinates;

public:
	/* I put the template functions down here so they aren't cluttering up the rest of the class */
	template <typename Field, typename Value>
	bool Set(Field* field, const Value& value, bool checkChanged) {
		if (checkChanged) {
			Field oldVal = *field;
			*field = static_cast<Field>(value);
			return memcmp(&oldVal, field, sizeof(Field)) != 0;
		}

		*field = static_cast<Field>(value);
		return false;
	}

	template<typename Value>
	bool Set(std::string* field, const Value& value, bool checkChanged) {
		if (checkChanged) {
			bool bChanged = *field != value;
			if (bChanged) {
				*field = value;
			}
			return bChanged;
		}

		*field = value;
		return false;
	}

	template <typename Field, typename Value>
	void SetPos(Field* field, const Value& value, bool setUpdateFlags = true) {
		if (Set(field, value, setUpdateFlags))
			m_updateFlags.m_posChanged = true;
	}

	template <typename Field, typename Value>
	void SetVis(Field* field, const Value& value, bool setUpdateFlags = true) {
		if (Set(field, value, setUpdateFlags))
			m_updateFlags.m_visChanged = true;
	}

	template <typename Field, typename Value>
	void SetInfo(Field* field, const Value& value, bool setUpdateFlags = true) {
		if (Set(field, value, setUpdateFlags))
			m_updateFlags.m_infoChanged = true;
	}

	template <typename Field, typename Value>
	void SetTitle(Field* field, const Value& value, bool setUpdateFlags = true) {
		if (Set(field, value, setUpdateFlags))
			m_updateFlags.m_titleChanged = true;
	}

	void SetVisUnknown(uint32_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.unknown, value, updateFlags);
	}
	void SetLockedNoLoot(uint8_t new_val, bool updateFlags = true) {
		SetVis(&m_visStruct.locked_no_loot, new_val, updateFlags);
	}
	void SetMercIcon(uint8_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.merc_icon, value, updateFlags);
	}
	void SetVisUnknownA(uint8_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.unknowna, value, updateFlags);
	}
	void SetVisFlags(uint16_t value, bool updateFlags = true) {
		SetVis(&m_visStruct.vis_flags, value, updateFlags);
	}
	void ToggleVisFlags(uint16_t toggle, bool updateFlags = true) {
		uint16_t flags = m_visStruct.vis_flags ^ toggle;
		SetVis(&m_visStruct.vis_flags, flags, updateFlags);
	}
	void SetVisUnknownB(uint8_t value, uint8_t index, bool updateFlags = true) {
		SetVis(&m_visStruct.unknownb[index], value, updateFlags);
	}
	void SetHandFlag(uint8_t new_val, bool updateFlags = true) {
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
	void SetHairType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.hair_type_id, value, updateFlags);
	}
	void SetFacialHairType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.facial_hair_type_id, value, updateFlags);
	}
	void SetWingType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.wing_type_id, value, updateFlags);
	}
	void SetChestType(uint32_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.chest_type_id, value, updateFlags);
	}
	void SetChestColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.chest_type_color, value, updateFlags);
	}
	void SetChestHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.chest_type_highlight, value, updateFlags);
	}
	void SetLegsType(uint16_t value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.legs_type_id, value, updateFlags);
	}
	void SetLegsColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.legs_type_color, value, updateFlags);
	}
	void SetLegsHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.legs_type_highlight, value, updateFlags);
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
	void SetSogaWingColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.soga_wing_color1, value, updateFlags);
	}
	void SetSogaWingHighlightColor(EQ2Color value, bool updateFlags = true) {
		SetInfo(&m_infoStruct.wing_color2, value, updateFlags);
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
	void SetActionState(uint32_t state, bool updateFlags = true) {
		SetInfo(&m_infoStruct.action_state, state, updateFlags);
	}
	void SetVisualState(uint32_t state, bool updateFlags = true) {
		SetInfo(&m_infoStruct.visual_state, state, updateFlags);
	}
	void SetMoodState(uint32_t state, bool updateFlags = true) {
		SetInfo(&m_infoStruct.mood_state, state, updateFlags);
	}
	void SetEmoteState(uint32_t new_val, bool updateFlags = true) {
		SetInfo(&m_infoStruct.emote_state, new_val, updateFlags);
	}
	void SetEntityFlags(uint32_t flags, bool updateFlags = true) {
		SetInfo(&m_infoStruct.entityFlags, flags, updateFlags);
	}
	void ToggleEntityFlags(uint32_t toggle, bool bupdateFlags = true) {
		uint32_t flags = m_infoStruct.entityFlags;
		SetEntityFlags(flags ^ toggle, bupdateFlags);
	}
	void EnableEntityFlags(uint32_t flags, bool bUpdateFlags = true) {
		uint32_t current = m_infoStruct.entityFlags;
		SetEntityFlags(current | flags, bUpdateFlags);
	}
	void DisableEntityFlags(uint32_t flags, bool bUpdateFlags = true) {
		uint32_t current = m_infoStruct.entityFlags;
		SetEntityFlags(current & (~flags), bUpdateFlags);
	}
	void ToggleInfoVisFlags(uint8_t toggle, bool bUpdateFlags = true) {
		uint8_t flags = m_infoStruct.visual_flag ^ toggle;
		SetInfo(&m_infoStruct.visual_flag, flags, bUpdateFlags);
	}
	void EnableInfoVisFlags(uint8_t flags, bool bUpdateFlags = true) {
		uint8_t f = m_infoStruct.visual_flag | flags;
		SetInfo(&m_infoStruct.visual_flag, f, bUpdateFlags);
	}
	void DisableInfoVisFlags(uint8_t flags, bool bUpdateFlags = true) {
		uint8_t f = m_infoStruct.visual_flag & (~flags);
		SetInfo(&m_infoStruct.visual_flag, f, bUpdateFlags);
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
	void SetX(float x, bool updateFlags = true) {
		SetPos(&m_posStruct.x, x, updateFlags);
		UpdateCellCoordinates();
	}
	void SetY(float y, bool updateFlags = true) {
		SetPos(&m_posStruct.y, y, updateFlags);
	}
	void SetZ(float z, bool updateFlags = true) {
		SetPos(&m_posStruct.z, z, updateFlags);
		UpdateCellCoordinates();
	}
	void SetLocation(float x, float y, float z, bool updateFlags = true) {
		SetX(x, false);
		SetY(y, false);
		SetZ(z, updateFlags);
	}
	void SetHeading(float heading, bool updateFlags = true) {
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
	void SetPitch(float pitch, bool updateFlags = true) {
		m_posStruct.pitch = pitch;
		SetPos(&m_posStruct.desiredPitch, pitch, updateFlags);
	}
	void SetCollisionRadius(float radius, bool updateFlags = true) {
		SetPos(&m_posStruct.collisionRadius, radius, updateFlags);
	}
	void SetSize(float new_size, bool updateFlags = true) {
		SetPos(&m_posStruct.size, new_size, updateFlags);
	}
	void SetSizeRatio(float value, bool updateFlags = true) {
		SetPos(&m_posStruct.sizeRatio, value, updateFlags);
	}
	void SetRoll(float roll, bool updateFlags = true) {
		m_posStruct.roll = roll;
		SetPos(&m_posStruct.desiredRoll, roll, updateFlags);
	}
	void SetActivityTimer(uint32_t timestamp, bool updateFlags = true) {
		SetInfo(&m_infoStruct.activity_timer, timestamp, updateFlags);
	}
	void SetName(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.name, value, updateFlags);
		m_titleStruct.m_updateFlags.nameChanged = true;
	}
	void SetUnknown1(uint8_t value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.unknown1, value, updateFlags);
		m_titleStruct.m_updateFlags.unknown1Changed = true;
	}
	void SetIsPlayer(uint8_t value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.isPlayer, value, updateFlags);
		m_titleStruct.m_updateFlags.isPlayerChanged = true;
	}
	void SetLastName(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.last_name, value, updateFlags);
		m_titleStruct.m_updateFlags.lastNameChanged = true;
	}
	void SetSuffixTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.suffix_title, value, updateFlags);
		m_titleStruct.m_updateFlags.suffixChanged = true;
	}
	void SetPrefixTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.prefix_title, value, updateFlags);
		m_titleStruct.m_updateFlags.prefixChanged = true;
	}
	void SetPvPTitle(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.pvp_title, value, updateFlags);
		m_titleStruct.m_updateFlags.pvpChanged = true;
	}
	void SetGuild(std::string value, bool updateFlags = true) {
		SetTitle(&m_titleStruct.guild, value, updateFlags);
		m_titleStruct.m_updateFlags.guildChanged = true;
	}
	void SetSpawnPositionData(const SpawnPositionStruct& pos, uint32_t timestamp) {
		movementTimestamp = timestamp;
		static_cast<SpawnPositionStruct&>(m_posStruct) = pos;
		m_updateFlags.m_posChanged = true;
		UpdateCellCoordinates();
	}
	void SetAppearanceEquipmentItem(uint8_t slotID, const struct EQ2EquipmentItem& item, bool updateFlags = true);
	void SetSizeOffset(float offset) { m_sizeOffset = offset; }
	float GetSizeOffset() { return m_sizeOffset; }
	void SetPrimaryCommandListID(uint32_t id) { m_primaryCommandListID = id; }
	uint32_t GetPrimaryCommandListID() { return m_primaryCommandListID; }
	void SetSecondaryCommandListID(uint32_t id) { m_secondaryCommandListID = id; }
	uint32_t GetSecondaryCommandListID() { return m_secondaryCommandListID; }
	void SetFactionID(uint32_t id) { m_factionID = id; }
	uint32_t GetFactionID() { return m_factionID; }

	// These may need to be changed
	void SetHP(uint32_t val) { m_hp = val; }
	uint32_t GetHP() { return m_hp; }
	void SetPower(uint32_t val) { m_power = val; }
	uint32_t GetPower() { return m_power; }
	void SetSavagery(uint32_t val) { m_savagery = val; }
	uint32_t GetSavagery() { return m_savagery; }
	void SetDissonance(uint32_t val) { m_dissonance = val; }
	uint32_t GetDissonance() { return m_dissonance; }
	void SetMerchantID(uint32_t val) { m_merchantID = val; }
	uint32_t GetMerchantID() { return m_merchantID; }
	void SetMerchantType(uint32_t val) { m_merchantType = val; }
	uint32_t GetMerchantType() { return m_merchantType; }
	void SetSpawnLocationID(uint32_t val) { m_spawnLocationID = val; }
	uint32_t GetSpawnLocationID() { return m_spawnLocationID; }
	void SetSpawnEntryID(uint32_t val) { m_spawnEntryID = val; }
	uint32_t GetSpawnEntryID() { return m_spawnEntryID; }
	void SetRespawnTime(uint32_t val) { m_respawnTime = val; }
	uint32_t GetRespawnTime() { return m_respawnTime; }
	void SetExpireTime(uint32_t val) { m_expireTime = val; }
	uint32_t GetExpireTime() { return m_expireTime; }
	void SetSpawnLocationPlacementID(uint32_t val) { m_spawnLocationPlacementID = val; }
	uint32_t GetSpawnLocationPlacementID() { return m_spawnLocationPlacementID; }
	void SetOrigX(float val) { m_origX = val; }
	float GetOrigX() { return m_origX; }
	void SetOrigY(float val) { m_origY = val; }
	float GetOrigY() { return m_origY; }
	void SetOrigZ(float val) { m_origZ = val; }
	float GetOrigZ() { return m_origZ; }
	void SetOrigHeading(float val) { m_origHeading = val; }
	float GetOrigHeading() { return m_origHeading; }
	void SetOrigPitch(float val) { m_origPitch = val; }
	float GetOrigPitch() { return m_origPitch; }
	void SetOrigRoll(float val) { m_origRoll = val; }
	float GetOrigRoll() { return m_origRoll; }
	float GetHeading() { return m_posStruct.heading; }
	std::pair<int32_t, int32_t> GetCellCoordinates() { return m_currentCellCoordinates; }
	void UpdateCellCoordinates();

	static float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2, bool ignore_y = false);
	float GetDistance(float x, float y, float z, bool ignore_y = false);
	float GetDistance(const std::shared_ptr<Spawn>& spawn, bool ignore_y = false);
};