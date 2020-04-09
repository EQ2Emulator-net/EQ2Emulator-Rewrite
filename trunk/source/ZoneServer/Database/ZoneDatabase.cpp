#include "stdafx.h"

#include "ZoneDatabase.h"
#include "../../common/log.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../ZoneServer/ZoneServer.h"
#include "../ZoneServer/ZoneOperator.h"
#include "../Spawns/Entity.h"

extern ZoneOperator z;

ZoneDatabase::ZoneDatabase() {

}

ZoneDatabase::~ZoneDatabase() {
}

bool ZoneDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

bool ZoneDatabase::LoadOpcodes() {
	DatabaseResult result;
	bool success;
	uint32_t count = 0;

	success = Select(&result, "SELECT `name`, `version_range1`, `version_range2`, `opcode` FROM `opcodes`");
	if (!success)
		return false;

	while (result.Next()) {
		OpcodeManager::GetGlobal()->RegisterVersionOpcode(result.GetString(0), result.GetUInt32(1), result.GetUInt32(2), result.GetUInt16(3));
		count++;
	}

	LogDebug(LOG_DATABASE, 0, "--Loaded %u opcodes", count);
	return true;
}

bool ZoneDatabase::LoadZoneInfo(ZoneServer* z) {
	DatabaseResult result;
	
	bool success = Select(&result, "SELECT * FROM zones WHERE id = %u", z->GetID());
	if (!success)
		return success;

	if (result.Next()) {
		z->SetExpansionID(result.GetUInt8(1));
		z->SetName(result.GetString(2));
		z->SetFile(result.GetString(3));
		z->SetDescription(result.GetString(4));
		z->SetSafeX(result.GetFloat(5));
		z->SetSafeY(result.GetFloat(6));
		z->SetSafeZ(result.GetFloat(7));
		z->SetSafeHeading(result.GetFloat(8));
		z->SetUnderworld(result.GetFloat(9));
		z->SetEXPModifier(result.GetFloat(10));
		z->SetMinRecommended(result.GetUInt8(11));
		z->SetMaxRecomended(result.GetUInt8(12));
		z->SetZoneType(result.GetString(13));
		z->SetIsAlwaysLoaded(result.GetBool(14));
		z->SetCityZone(result.GetBool(15));
		z->SetWeatherAllowed(result.GetBool(16));
		z->SetMinStatus(result.GetUInt32(17));
		z->SetMinLevel(result.GetUInt32(18));
		z->SetMaxLevel(result.GetUInt32(19));
		z->SetStartingZone(result.GetUInt8(20));
		z->SetInstanceType(result.GetString(21));
		z->SetDefaultReenterTime(result.GetUInt32(22));
		z->SetDefaultResetTime(result.GetUInt32(23));
		z->SetDefaultLockoutTime(result.GetUInt32(124));
		z->SetForceGroupToZone(result.GetUInt16(25));
		z->SetLuaScript(result.GetString(26));
		z->SetShutdownTimer(result.GetUInt32(27));
		z->SetZoneMOTD(result.GetString(28));
		z->SetRulesetID(result.GetUInt32(29));
	}
	else
		return false;

	return true;
}

bool ZoneDatabase::LoadCharacter(uint32_t char_id, uint32_t account_id, std::shared_ptr<Entity> entity) {
	DatabaseResult result;
	bool ret = Select(&result, "SELECT `name`, `race`, `class`, `gender`, `deity`, `body_size`, `body_age`, `level`, `tradeskill_class`, `tradeskill_level`, `soga_wing_type`, `soga_chest_type`, `soga_legs_type`, `soga_hair_type`, `soga_facial_hair_type`, `soga_model_type`, `legs_type`, `chest_type`, `wing_type`, `hair_type`, `facial_hair_type`, `model_type`, `x`, `y`, `z`, `heading`, `starting_city`, DATEDIFF(curdate(), `created_date`) as accage FROM characters WHERE id = %u AND account_id = %u AND deleted = 0", char_id, account_id);
	if (!ret)
		return ret;

	if (result.Next()) {
		entity->SetName(result.GetString(0), false);
		entity->SetRace(result.GetUInt8(1), false);
		entity->SetAdventureClass(result.GetUInt8(2), false);
		entity->SetGender(result.GetUInt8(3), false);
		// Set Deity
		entity->SetBodySize(result.GetUInt8(5), false);
		entity->SetBodySizeUnknown(result.GetUInt8(6), false); // Not sure if this is actually the correct field for age
		entity->SetLevel(result.GetUInt8(7), false);
		// Set Tradeskill Class
		// Set Tradeskill Level
		// Set SOGA Wing Type
		// Set SOGA Chest Type
		// Set SOGA Legs Type
		entity->SetSogaHairType(result.GetUInt16(13), false);
		entity->SetSogaFacialHairType(result.GetUInt16(14), false);
		entity->SetSogaModelType(result.GetUInt32(15), false);
		entity->SetLegsType(result.GetUInt16(16), false);
		entity->SetChestType(result.GetUInt16(17), false);
		entity->SetWingType(result.GetUInt16(18), false);
		entity->SetHairType(result.GetUInt16(19), false);
		entity->SetFacialHairType(result.GetUInt16(20), false);
		entity->SetModelType(result.GetUInt32(21), false);
		entity->SetX(result.GetFloat(22), false);
		entity->SetY(result.GetFloat(23), false);
		entity->SetZ(result.GetFloat(24), false);
		entity->SetHeading(result.GetFloat(25), false);
		// Set Starting City
		// Set Account Age

		DatabaseResult result2;
		ret = Select(&result2, "SELECT type, red, green, blue FROM char_colors WHERE char_id = %u", char_id);
		if (!ret)
			return ret;

		while (result2.Next()) {
			std::string type = result2.GetString(0);

			if (type == "skin_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSkinColor(c, false);
			}
			else if (type == "eye_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSkinColor(c, false);
			}
			else if (type == "hair_color1") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetHairColor1(c, false);
			}
			else if (type == "hair_color2") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetHairColor2(c, false);
			}
			else if (type == "hair_highlight") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetHairHighlight(c, false);
			}
			else if (type == "hair_type_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetHairColor(c, false);
			}
			else if (type == "hair_type_highlight_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetHairHighlightColor(c, false);
			}
			else if (type == "hair_face_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetFacialHairColor(c);
			}
			else if (type == "hair_face_highlight_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetFacialHairHighlightColor(c, false);
			}
			else if (type == "wing_color1") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetWingColor(c, false);
			}
			else if (type == "wing_color2") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetWingHighlightColor(c, false);
			}
			else if (type == "shirt_color") {
				/*
				Not in struct?
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "unknown_chest_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "pants_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "unknown_legs_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "unknown9") {
				/*
				Not in struct
				EQ2Color c;
				c.unknown9.Red = result2.GetUInt8(1);
				c.unknown9.Green = result2.GetUInt8(2);
				c.unknown9.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "eye_type") {
				entity->SetEyeType(result2.GetInt8(1), 0, false);
				entity->SetEyeType(result2.GetInt8(2), 1, false);
				entity->SetEyeType(result2.GetInt8(3), 2, false);
			}
			else if (type == "ear_type") {
				entity->SetEarType(result2.GetInt8(1), 0, false);
				entity->SetEarType(result2.GetInt8(2), 1, false);
				entity->SetEarType(result2.GetInt8(3), 2, false);
			}
			else if (type == "eye_brow_type") {
				entity->SetEyeBrowType(result2.GetInt8(1), 0, false);
				entity->SetEyeBrowType(result2.GetInt8(2), 1, false);
				entity->SetEyeBrowType(result2.GetInt8(3), 2, false);
			}
			else if (type == "cheek_type") {
				entity->SetCheekType(result2.GetInt8(1), 0, false);
				entity->SetCheekType(result2.GetInt8(2), 1, false);
				entity->SetCheekType(result2.GetInt8(3), 2, false);
			}
			else if (type == "lip_type") {
				entity->SetLipType(result2.GetInt8(1), 0, false);
				entity->SetLipType(result2.GetInt8(2), 1, false);
				entity->SetLipType(result2.GetInt8(3), 2, false);
			}
			else if (type == "chin_type") {
				entity->SetChinType(result2.GetInt8(1), 0, false);
				entity->SetChinType(result2.GetInt8(2), 1, false);
				entity->SetChinType(result2.GetInt8(3), 2, false);
			}
			else if (type == "nose_type") {
				entity->SetNoseType(result2.GetInt8(1), 0, false);
				entity->SetNoseType(result2.GetInt8(2), 1, false);
				entity->SetNoseType(result2.GetInt8(3), 2, false);
			}
			else if (type == "body_size") {
				entity->SetBodySize(result2.GetInt8(1), false);
			}
			else if (type == "soga_skin_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaSkinColor(c, false);
			}
			else if (type == "soga_eye_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaEyeColor(c, false);
			}
			else if (type == "soga_hair_color1") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaHairColor1(c, false);
			}
			else if (type == "soga_hair_color2") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaHairColor2(c, false);
			}
			else if (type == "soga_hair_highlight") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaHairHighlight(c, false);
			}
			else if (type == "soga_hair_type_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaHairColor(c, false);
			}
			else if (type == "soga_hair_type_highlight_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaHairHighlightColor(c, false);
			}
			else if (type == "soga_hair_face_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaFacialHairColor(c, false);
			}
			else if (type == "soga_hair_face_highlight_color") {
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				entity->SetSogaFacialHairHighlightColor(c, false);
			}
			else if (type == "soga_wing_color1") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_wing_color2") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_shirt_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_unknown_chest_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_pants_color") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_unknown_legs_color") {
				/*
				Not in struct
				EQ2Color c;
				c.soga_unknown_legs_color.Red = result2.GetUInt8(1);
				c.soga_unknown_legs_color.Green = result2.GetUInt8(2);
				c.soga_unknown_legs_color.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_unknown13") {
				/*
				Not in struct
				EQ2Color c;
				c.Red = result2.GetUInt8(1);
				c.Green = result2.GetUInt8(2);
				c.Blue = result2.GetUInt8(3);
				*/
			}
			else if (type == "soga_eye_type") {
				entity->SetSogaEyeType(result2.GetInt8(1), 0, false);
				entity->SetSogaEyeType(result2.GetInt8(2), 1, false);
				entity->SetSogaEyeType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_ear_type") {
				entity->SetSogaEarType(result2.GetInt8(1), 0, false);
				entity->SetSogaEarType(result2.GetInt8(2), 1, false);
				entity->SetSogaEarType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_eye_brow_type") {
				entity->SetSogaEyeBrowType(result2.GetInt8(1), 0, false);
				entity->SetSogaEyeBrowType(result2.GetInt8(2), 1, false);
				entity->SetSogaEyeBrowType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_cheek_type") {
				entity->SetSogaCheekType(result2.GetInt8(1), 0, false);
				entity->SetSogaCheekType(result2.GetInt8(2), 1, false);
				entity->SetSogaCheekType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_lip_type") {
				entity->SetSogaLipType(result2.GetInt8(1), 0, false);
				entity->SetSogaLipType(result2.GetInt8(2), 1, false);
				entity->SetSogaLipType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_chin_type") {
				entity->SetSogaChinType(result2.GetInt8(1), 0, false);
				entity->SetSogaChinType(result2.GetInt8(2), 1, false);
				entity->SetSogaChinType(result2.GetInt8(3), 2, false);
			}
			else if (type == "soga_nose_type") {
				entity->SetSogaNoseType(result2.GetInt8(1), 0, false);
				entity->SetSogaNoseType(result2.GetInt8(2), 1, false);
				entity->SetSogaNoseType(result2.GetInt8(3), 2, false);
			}
		}
	}

	return ret;
}