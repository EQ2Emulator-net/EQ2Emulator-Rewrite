#include "stdafx.h"

#include "SpawnStructs.h"
#include "Spawn.h"
#include "../../common/CRC16.h"
#include "../ZoneServer/Client.h"
#include "../Controllers/PlayerController.h"
#include "Entity.h"


uint32_t SpawnVisualizationStruct::CalculateCRC() {
	//Not sure why they named this function CRC16 when it returns a 32 but whatever
	return CRC16(reinterpret_cast<const unsigned char*>(this), sizeof(*this), 0xFFFFFFFF);
}

void SpawnVisualizationStruct::SetVisualFlags(const std::shared_ptr<Client>& client, const std::shared_ptr<Spawn>& spawn, 
	const std::shared_ptr<class Entity>& player)
{
	//Determine vis flags for this spawn
	vis_flags = 0;
	if (spawn->IsTargetable()) {
		vis_flags |= VIS_FLAG_TARGETABLE;
	}
	if (spawn->ShouldShowLevel()) {
		vis_flags |= VIS_FLAG_SHOW_LEVEL;
	}
	if (spawn->ShouldShowCommandIcon()) {
		vis_flags |= VIS_FLAG_SHOW_COMMAND_ICON;
	}
	if (spawn == player) {
		//OR if they're grouped!!
		vis_flags |= VIS_FLAG_GROUPED_WITH_PLAYER;
	}
	if (spawn->ShouldShowName()) {
		vis_flags |= VIS_FLAG_DISPLAY_NAME;
	}
	if (spawn->IsAttackable()) {
		vis_flags |= VIS_FLAG_ATTACKABLE;
	}
	if (client->bDevMode) {
		vis_flags |= VIS_FLAG_DISPLAY_NAME | VIS_FLAG_SHOW_COMMAND_ICON | VIS_FLAG_TARGETABLE;
	}
}

void SpawnVisualizationStruct::DetermineForClient(const std::shared_ptr<Client>& client, const std::shared_ptr<Spawn>& spawn) {
	auto player = std::dynamic_pointer_cast<Entity>(client->GetController()->GetControlled());

	if (!player) {
		LogError(LOG_CLIENT, 0, "Tried to make a spawn visual packet for a client with no player?");
		return;
	}

	SetVisualFlags(client, spawn, player);
	if (vis_flags & VIS_FLAG_SHOW_LEVEL) {
		considerDifficulty = static_cast<uint8_t>(Spawn::GetConsiderDifficulty(player->GetAdventureLevel(), spawn->GetAdventureLevel()));
	}
	else {
		considerDifficulty = static_cast<uint8_t>(EConsiderDifficulty::WHITE);
	}
	bShowHandFlag = spawn->IsInteractable();
}

void Substruct_SpawnVisualization::RegisterElements() {
	if (version >= 60038) {
		RegisterUInt32(unknown);
	}
	RegisterUInt8(considerDifficulty);
	RegisterUInt8(locked_no_loot);
	RegisterInt8(factionCon);
	RegisterUInt8(quest_flag);
	if (version >= 1119) {
		RegisterUInt8(merc_icon);
	}
	if (version >= 864) {
		RegisterBool(bShowAggro);
	}
	if (version >= 1096 || version < 1142) {
		RegisterUInt8(pet_threat);
	}
	if ((version >= 1119 && version < 1142) || version >= 1188) {
		RegisterUInt8(name_quest_icon);
	}
	if (version >= 1188) {
		RegisterUInt32(unknowna2);
	}
	RegisterUInt16(vis_flags);
	if (version >= 1142 && version < 1188) {
		RegisterUInt8(name_quest_icon);
	}
	uint8_t& unknownb = this->unknownb[0];
	if (version < 1142) {
		RegisterUInt8(unknownb)->SetCount(3);
	}
	else {
		RegisterUInt8(unknownb);
		RegisterUInt8(pvp_difficulty);
	}
	RegisterBool(bShowHandFlag);
	
	if (version > 864) {
		RegisterUInt32(unknown4);
	}

	RescopeArrayElement(unknown2);

	if (version < 60038 && ((version >= 864 && version < 936) || version >= 1188)) {
		RegisterUInt8(unknown2)->SetCount(4);
	}
	else if (version >= 936 && version < 1142) {
		RegisterUInt8(unknown2);
	}
	else if (version > 1142 && version < 1188) {
		RegisterUInt8(unknown2)->SetCount(3);
	}

	if (version >= 1096) {
		RegisterUInt8(tag_icon);
		RegisterUInt8(tag_number);
		RegisterUInt8(unknown3);
	}
}

SpawnPositionStruct::SpawnPositionStruct() {
	memset(this, 0, sizeof(*this));
	//sizeUnknown = -1.f;
	faceActorID = 0xFFFFFFFF;
}

void SpawnPositionStruct::InsertSpawnData(const std::shared_ptr<Client>& client, const std::shared_ptr<Spawn>& spawn) {
	*this = *spawn->GetPosStruct();
	if (auto& widget = spawn->GetWidgetData()) {
		if (!widget->GetIncludeHeading()) {
			heading = 0.f;
			desiredHeading = 0.f;
		}
		if (!widget->GetIncludeLocation()) {
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}
	}
}

//Many elements in this packet are compressed from a float (4 bytes) to a short (2 bytes). Handle those conversions here
void Substruct_SpawnPosition::CompressData() {
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
	collisionRadius_compressed = CompressFloat(collisionRadius, 32);
	speedModifier_compressed = CompressFloat(speedModifier, 32);
	airSpeed_compressed = CompressFloat(airSpeed, 32);
	unkSpeed3_compressed = CompressFloat(unkSpeed3, 32);
	desiredVertSpeed_compressed = CompressFloat(desiredVertSpeed, 32);
	desiredStrafeSpeed_compressed = CompressFloat(desiredStrafeSpeed, 32);
	swimmingSpeedMultiplier_compressed = CompressFloat(swimmingSpeedMultiplier, 32);
	desiredForwardSpeed_compressed = CompressFloat(desiredForwardSpeed, 256);
	desiredRoll_compressed = CompressFloat(desiredRoll, 256);
	desiredRollVelocity_compressed = CompressFloat(desiredRollVelocity, 256);
	size_compressed = CompressFloat(size, 32);
	size_ratio_compressed = CompressFloat(sizeRatio, 32);
	if (version > 910) {
		//Looks like size moved after 910 back 2 bytes where collisionRadius was prior and they thought this element was size
		//Not sure what this is but has a massive compression ratio so can only be low number values
		sizeUnknown_compressed = CompressFloat(sizeUnknown, 16384);
	}
	unk70_compressed = CompressFloat(unk70, 256);
	actorStopRange_compressed = CompressFloat(actorStopRange, 32);
}

void Substruct_SpawnPosition::DecompressData() {
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
	airSpeed = ExpandFloat(airSpeed_compressed, 32);
	unkSpeed3 = ExpandFloat(unkSpeed3_compressed, 32);
	desiredVertSpeed = ExpandFloat(desiredVertSpeed_compressed, 32);
	desiredStrafeSpeed = ExpandFloat(desiredStrafeSpeed_compressed, 32);
	swimmingSpeedMultiplier = ExpandFloat(swimmingSpeedMultiplier_compressed, 32);
	desiredForwardSpeed = ExpandFloat(desiredForwardSpeed_compressed, 256);
	desiredRoll = ExpandFloat(desiredRoll_compressed, 256);
	desiredRollVelocity = ExpandFloat(desiredRollVelocity_compressed, 256);
	if (GetVersion() <= 910) {
		size = ExpandFloat(size_compressed, 32);
		sizeUnknown = ExpandFloat(sizeUnknown_compressed, 16384);
		sizeRatio = ExpandFloat(size_ratio_compressed, 32);
	}
	unk70 = ExpandFloat(unk70_compressed, 256);
	actorStopRange = ExpandFloat(actorStopRange_compressed, 32);
}

void Substruct_SpawnPosition::RegisterElements() {
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
		RegisterUInt32(positionState2);
		if (GetVersion() < 936) {
			RegisterUInt8(movementMode);
			RegisterUInt8(unkByte);
		}
		RegisterFloat(destLocX);
		RegisterFloat(destLocY);
		RegisterFloat(destLocZ);
		RegisterFloat(destLocX2);
		RegisterFloat(destLocY2);
		RegisterFloat(destLocZ2);
		if (GetVersion() >= 936) {
			RegisterUInt8(movementMode);
			RegisterUInt8(unkByte);
		}
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
	if (GetVersion() >= 1119) {
		RegisterInt16(desiredHeadingVelocity_compressed);
	}
	RegisterInt16(speedModifier_compressed);
	if (GetVersion() >= 1096) {
		RegisterInt16(airSpeed_compressed);
	}
	RegisterInt16(swimmingSpeedMultiplier_compressed);
	RegisterInt16(desiredStrafeSpeed_compressed);
	if (GetVersion() >= 1119) {
		RegisterInt16(desiredVertSpeed_compressed);
		RegisterInt16(unkSpeed3_compressed);
	}
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
	RegisterInt16(collisionRadius_compressed);

	if (GetVersion() >= 936) {
		RegisterInt16(sizeUnknown_compressed);
	}
	else {
		RegisterInt16(size_compressed);
	}

	if (GetVersion() > 283) {
		if (GetVersion() < 918) {
			RegisterInt16(size_ratio_compressed);
			RegisterInt16(sizeUnknown_compressed);
		}
		else {
			RegisterFloat(size);
			RegisterFloat(sizeRatio);
		}
	}
	RegisterUInt32(faceActorID);
	RegisterInt16(actorStopRange_compressed);
	RegisterInt16(roll_compressed);
	RegisterInt16(unk70_compressed);
	if ((GetVersion() >= 936 && GetVersion() < 1096) || GetVersion() >= 1142)
		RegisterInt16(unk72);
}

void Substruct_SpawnPosition::WriteElement(unsigned char* outbuf, uint32_t& offset) {
	//These values are converted in the spawn packet to a different scale before being compressed (they can go negative)
	float tmpHeading = heading;
	float tmpDesiredHeading = desiredHeading;

	heading -= 180.f;
	desiredHeading -= 180.f;

	CompressData();
	PacketEncodedData::WriteElement(outbuf, offset);

	heading = tmpHeading;
	desiredHeading = tmpDesiredHeading;
}

void Substruct_SpawnPosition::PostRead() {
	DecompressData();

	//Convert the heading values to something we use
	heading += 180.f;
	desiredHeading += 180.f;
}

void Substruct_SpawnInfo::RegisterElements() {
	auto& orig_level = reinterpret_cast<uint8_t&>(this->orig_level);
	auto& level = reinterpret_cast<uint8_t&>(this->level);

	if (version < 60038) {
		RegisterUInt32(hp_percent);
		RegisterUInt32(power_percent);

		RescopeArrayElement(unknown3);
		RescopeArrayElement(spell_effects);
		RegisterSubstruct(spell_effects)->SetCount(30);
		RegisterUInt32(target_id);
		RegisterUInt32(follow_target);
		RegisterUInt32(id_unknown);
		RegisterUInt32(pet_owner_id);

		if (version < 936) {
			//These were all moved further in the packet
			RegisterUInt8(adv_class);
			RegisterUInt8(orig_level);
			RegisterUInt8(level);
			RegisterUInt8(difficulty);
			RegisterUInt8(unknown5);
			RescopeToReference(heroic_flag, uint8_t);
			RegisterUInt8(heroic_flag);
		}
		RegisterUInt32(entityFlags);
		RegisterUInt32(entityFlags2);
		RescopeArrayElement(unknown6);
		if (version < 936) {
			RegisterUInt8(archetype);
			RegisterUInt8(unknown6)->SetCount(3);
			RegisterUInt8(heatLevel);
		}
		else {
			if (version >= 954) {
				RegisterUInt8(archetype);
			}
			RegisterUInt8(orig_level);
			RegisterUInt8(level);
			RegisterUInt8(unknown5);
			RegisterUInt8(heroic_flag);
			RegisterUInt32(activity_timer);
			RegisterUInt8(heatLevel);
		}
		RegisterEQ2Color(torch_color);
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
	if (version < 60038) {
		RegisterEQ2Color(skin_color);
		RegisterEQ2Color(eye_color);
		if (version >= 864) {
			RegisterEQ2Color(model_color);
		}
		if (version > 283) {
			RegisterEQ2Color(soga_eye_color);
			RegisterEQ2Color(soga_skin_color);
			if (version >= 864) {	
				RegisterEQ2Color(soga_model_color);
			}
		}
	}
	if (version < 57080) {
		uint16_t& equipment_types = reinterpret_cast<uint16_t&>(this->equipment_types_int16[0]);
		RegisterUInt16(equipment_types)->SetCount(24);
	}
	else {
		RescopeArrayElement(equipment_types);
		RegisterUInt32(equipment_types)->SetCount(24);
	}

	if (version >= 1161) {
		if (version >= 57080) {
			RegisterUInt32(mount_adornment_type);
			RegisterUInt32(mount_armor_type);
		}
		else {
			RescopeToReference(mount_adornment_type, uint16_t);
			RegisterUInt16(mount_adornment_type);
			RescopeToReference(mount_armor_type, uint16_t);
			RegisterUInt16(mount_armor_type);
		}
	}

	if (version < 57080) {
		RescopeToReference(textures_slot_type, uint16_t);
		RegisterUInt16(textures_slot_type);
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
		RegisterUInt32(textures_slot_type);
		RegisterUInt32(hair_type_id);
		RegisterUInt32(facial_hair_type_id);
		RegisterUInt32(wing_type_id);
		if (version >= 67804) {
			RegisterUInt32(tail_type_id);
		}
		RegisterUInt32(chest_type_id);
		RegisterUInt32(legs_type_id);
	}

	if (version >= 996) {
		if (version < 57080) {
			RescopeToReference(back_slot_type_id, uint16_t);
			RegisterUInt16(back_slot_type_id);
		}
		else {
			RegisterUInt32(back_slot_type_id);
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

	if (version >= 60038) {
		RegisterUInt32(mount_type);
		RegisterUInt32(combat_voice);
		RegisterUInt32(emote_voice);
		RegisterUInt32(ammo_type);
		RegisterUInt8(visual_flag);
		RegisterUInt8(interaction_flag);
		RegisterUInt8(flag3);
		RegisterUInt8(flag4);
		RescopeArrayElement(heraldry);
		RegisterUInt8(heraldry)->SetCount(7);
		RegisterUInt8(align_byte);
		RescopeArrayElement(soga_heraldry);
		RegisterUInt8(soga_heraldry)->SetCount(7);
		RegisterUInt8(align_byte2);
	}

	int32_t colorCount = (version > 283 ? 24 : 21);
	RescopeArrayElement(equipment_colors);
	RegisterEQ2Color(equipment_colors)->SetCount(colorCount);
	if (version >= 1161) {
		RegisterEQ2Color(mount_adornment_color);
		RegisterEQ2Color(mount_armor_color);
	}
	if (GetVersion() > 283) {
		RegisterEQ2Color(textures_slot_color);
	}
	RegisterEQ2Color(hair_type_color);
	RegisterEQ2Color(hair_face_color);
	if (version > 283) {
		RegisterEQ2Color(wing_color1);
		if (version >= 67804) {
			RegisterEQ2Color(tail_color1);
		}
	}

	RegisterEQ2Color(chest_type_color);
	RegisterEQ2Color(legs_type_color);

	if (version >= 996) {
		RegisterEQ2Color(back_slot_type_color);
	}

	RescopeArrayElement(equipment_highlights);
	RegisterEQ2Color(equipment_highlights)->SetCount(24);
	if (version >= 1161) {
		RegisterEQ2Color(mount_adornment_highlight);
		RegisterEQ2Color(mount_armor_highlight);
	}
	if (version > 283) {
		RegisterEQ2Color(textures_slot_highlight);
	}
	RegisterEQ2Color(hair_type_highlight_color);
	RegisterEQ2Color(hair_face_highlight_color);
	RegisterEQ2Color(wing_color2);
	if (version >= 67804) {
		RegisterEQ2Color(tail_color2);
	}

	RegisterEQ2Color(chest_type_highlight);
	RegisterEQ2Color(legs_type_highlight);

	if (version >= 996) {
		RegisterEQ2Color(back_slot_type_highlight);
	}

	if (version > 283) {
		RegisterEQ2Color(soga_hair_type_color);
		RegisterEQ2Color(soga_hair_type_highlight_color);
		RegisterEQ2Color(soga_hair_face_color);
		RegisterEQ2Color(soga_hair_face_highlight_color);

		if (version >= 60038) {
			RegisterEQ2Color(skin_color);
			RegisterEQ2Color(eye_color);
			RegisterEQ2Color(model_color);
			RegisterEQ2Color(soga_eye_color);
			RegisterEQ2Color(soga_skin_color);
			RegisterEQ2Color(soga_model_color);
		}
	}

	RescopeArrayElement(sliderBytes);
	RegisterInt8(sliderBytes)->SetCount(26);

	if (version > 283) {
		RescopeArrayElement(sogaSliderBytes);
		RegisterInt8(sogaSliderBytes)->SetCount(26);
	}

	if (version < 60038) {
		RescopeArrayElement(heraldry);
		RegisterUInt8(heraldry)->SetCount(7);
		if (version < 1119) {
			static uint8_t alignFiller = 0;
			RegisterUInt8(alignFiller);
		}
		else {
			RescopeArrayElement(soga_heraldry);
			RegisterUInt8(soga_heraldry)->SetCount(7);
		}

		if (version < 57080) {
			RescopeToReference(mount_type, uint16_t);
			RegisterUInt16(mount_type);
		}
		else {
			RegisterUInt32(mount_type);
		}
	}

	RegisterEQ2Color(mount_color);
	RegisterEQ2Color(mount_saddle_color);

	//8 bytes popped up here, since the soga heraldry also popped up recently before this
	//thinking it was probably soga mount stuff..but not needed so didn't last long before removal
	if (version >= 1161 && version < 1188) {
		static uint64_t soga_mount_info = 0;
		RegisterUInt64(soga_mount_info);
	}

	RegisterEQ2Color(hair_color1);
	RegisterEQ2Color(hair_color2);
	RegisterEQ2Color(hair_highlight);
	if (version > 283) {
		RegisterEQ2Color(soga_hair_color1);
		RegisterEQ2Color(soga_hair_color2);
		RegisterEQ2Color(soga_hair_highlight);
		if (version >= 67804) {
			//This version needed filler to align NetAppearance to 4 bytes, this may go away at some point
			static uint8_t s_alignFiller[2] = { 0, 0 };
			uint8_t& alignFiller = s_alignFiller[0];
			RegisterUInt8(alignFiller)->SetCount(2);
		}
	}
	if (version < 60038) {
		if (version < 57080) {
			uint16_t& combat_voice = reinterpret_cast<uint16_t&>(this->combat_voice);
			RegisterUInt16(combat_voice);
			uint16_t& emote_voice = reinterpret_cast<uint16_t&>(this->emote_voice);
			RegisterUInt16(emote_voice);
			if (version >= 839) {
				uint16_t& ammo_type = reinterpret_cast<uint16_t&>(this->ammo_type);
				RegisterUInt16(ammo_type);
			}
			if (version >= 864 && version < 1119) {
				uint16_t& unknown17 = reinterpret_cast<uint16_t&>(this->unknown17);
				RegisterUInt16(unknown17);
			}
		}
		else {
			RegisterUInt32(combat_voice);
			RegisterUInt32(emote_voice);
			RegisterUInt32(ammo_type);
		}

		RegisterUInt8(visual_flag);
		RegisterUInt8(interaction_flag);

		if (version >= 839) {
			RegisterUInt8(flag3);
			RegisterUInt8(flag4);
			RegisterFloat(size_mod);

			if (version > 1096 && version < 1119) {
				static uint16_t zalignFiller = 0;
				RegisterUInt16(zalignFiller);
			}

			if (version >= 57080) {
				RescopeArrayElement(spell_visuals);
				RegisterUInt32(spell_visuals)->SetCount(8);		
			}
			else {
				uint16_t& spell_visuals = spell_visuals_do_not_set[0];
				RegisterUInt16(spell_visuals)->SetCount(8);
			}

			RescopeArrayElement(spell_visual_levels);
			RegisterUInt8(spell_visual_levels)->SetCount(8);
		}
		else {
			RescopeArrayElement(unknown18);
			//This probably has the above elements (spell visuals) and then something else that was removed or needs to be combined into an above unknown but need to confirm
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
		if (version >= 839) {
			uint16_t& emote_state = reinterpret_cast<uint16_t&>(this->emote_state);
			RegisterUInt16(emote_state);
		}
	}
	else {
		RegisterUInt32(action_state);
		RegisterUInt32(visual_state);
		RegisterUInt32(mood_state);
		RegisterUInt32(emote_state);
	}

	if (version >= 60038) {
		RegisterUInt32(primary_slot_state);
		RegisterUInt32(secondary_slot_state);
		RegisterUInt32(ranged_slot_state);
		RescopeArrayElement(spell_visuals);
		RegisterUInt32(spell_visuals)->SetCount(8);
		RescopeArrayElement(spell_visual_levels);
		RegisterUInt8(spell_visual_levels)->SetCount(8);
		RescopeArrayElement(spell_effects);
		RegisterSubstruct(spell_effects)->SetCount(30);
		RegisterUInt32(target_id);
		RegisterUInt32(follow_target);
		RegisterUInt32(id_unknown);
		RegisterUInt32(pet_owner_id);
		RegisterUInt32(entityFlags);
		RegisterUInt32(entityFlags2);
		RegisterUInt32(activity_timer);
		RegisterUInt32(cast_unknown);
		RegisterUInt32(hp_percent);
		RegisterUInt32(power_percent);
		RegisterFloat(size_mod);
		RegisterInt16(cast_percentage);
		RegisterInt16(unknown3c2);
		if (GetVersion() >= 67633) {
			RegisterUInt32(unknown67633);
		}
		RegisterUInt8(threat_level_secondary);
		RegisterUInt8(orig_level);
		RegisterUInt8(level);
		RegisterUInt8(unknown5);
		RegisterUInt8(heroic_flag);
		RegisterUInt8(heatLevel);
		RegisterEQ2Color(torch_color);
	}

	RegisterUInt8(race);
	RegisterUInt8(gender);

	if (version >= 936) {
		RegisterUInt8(adv_class);
		RegisterUInt8(difficulty);

		if (version >= 60038) {
			RegisterUInt8(show_archtype_icon);
			RescopeArrayElement(unknown21);
			RegisterUInt8(unknown21)->SetCount(2);
			//RegisterUInt32(activity_timer);
			//RescopeArrayElement(unknown18);
			//RegisterUInt8(unknown18)->SetCount(68);
		}
		else {
			RescopeArrayElement(unknown20);
			if (version >= 945 && version < 953) {
				//One of these bytes appears to have moved higher in the packet after this version
				RegisterUInt8(unknown20)->SetCount(4);
			}
			else {
				if (version >= 1096) {
					RescopeArrayElement(unknown20a);
					if (version >= 57080) {
						RegisterUInt8(show_archtype_icon);
						RegisterUInt8(unknown20a)->SetCount(7);
						RegisterUInt32(primary_slot_state);
						RegisterUInt32(secondary_slot_state);
						RegisterUInt32(ranged_slot_state);
					}
					else if (version >= 1188) {
						RegisterUInt8(show_archtype_icon);
						RegisterUInt8(unknown20a)->SetCount(7);
						RescopeToReference(primary_slot_state, uint16_t);
						RescopeToReference(secondary_slot_state, uint16_t);
						RescopeToReference(ranged_slot_state, uint16_t);
						RegisterUInt16(primary_slot_state);
						RegisterUInt16(secondary_slot_state);
						RegisterUInt16(ranged_slot_state);
					}
					else {
						RegisterUInt8(show_archtype_icon);
						RegisterUInt8(unknown20a)->SetCount(5);
					}
				}
				if (version < 58618) {
					RegisterUInt8(unknown20);
				}
				else {
					RegisterUInt8(unknown20)->SetCount(5);
				}
			}
		}
	}
}

SpawnInfoStruct::SpawnInfoStruct() {
	memset(this, 0, sizeof(*this));
	power_percent = 100;
	hp_percent = 100;
	heatLevel = 0xff;//temp until we can set it properly
	size_mod = 1.0f;

	target_id = -1;
	follow_target = -1;
	id_unknown = -1;
	pet_owner_id = -1;
	cast_unknown = -1;
	cast_percentage = -1;
	unknown3c2 = -1;
	show_archtype_icon = 0xff;
}

void Substruct_SpawnInfo::PreWrite() {
	PacketSubstruct::PreWrite();
	if (version < 57080) {
		for (int i = 0; i < 24; i++) {
			equipment_types_int16[i] = static_cast<uint16_t>(equipment_types[i]);
		}
		for (int i = 0; i < 8; i++) {
			spell_visuals_do_not_set[i] = static_cast<uint16_t>(spell_visuals[i]);
		}

		if (version < 1188) {
			//Some entity flags moved over one when they added mercs, shift those flags back one
			entity_flags_pre_write = entityFlags;
			//Save the corpse and npc flag because those didn't move
			uint32_t tmp = entityFlags & (EntityFlagCorpse | EntityIsNpc);
			//Drop corpse, npc and mercenary
			entityFlags &= ~(EntityFlagCorpse | EntityIsNpc | EntityIsMercenary);
			//Shift remaining
			entityFlags >>= 1;
			//Add back corpse/npc
			entityFlags |= tmp;
		}
	}

}