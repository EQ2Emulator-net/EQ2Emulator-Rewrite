#include "stdafx.h"

#include "WorldDatabase.h"
#include "../../common/log.h"
#include "../../common/timer.h"

#include "../Packets/OP_AllCharactersDescReplyMsg_Packet.h"
#include "../Packets/OP_AllCharactersDescRequestMsg_Packet.h"
#include "../Packets/OP_AllWSDescRequestMsg_Packet.h"
#include "../Packets/OP_CreateCharacterRequestMsg_Packet.h"
#include "../Packets/OP_CreateCharacterReplyMsg_Packet.h" // needed for the defines

#include "../../common/Classes.h"
#include "../WorldServer/WorldServer.h"
#include "../WorldServer/CharacterList.h"
#include "../WorldServer/Character.h"

#ifdef _WIN32
	#include <WS2tcpip.h>
#else
	#include <arpa/inet.h>
#endif

extern WorldServer g_worldServer;
extern Classes classes;
extern CharacterList g_characterList;

WorldDatabase::WorldDatabase() {

}

WorldDatabase::~WorldDatabase() {
}

bool WorldDatabase::Start() {
	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	return false;
}

bool WorldDatabase::GetAccount(Client* client, std::string user, std::string pass) {
	DatabaseResult result;
	bool success;

	string esc_user = Escape(user.c_str());
	string esc_pass = Escape(pass.c_str());

	success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s' AND passwd = SHA2('%s', 512)", esc_user.c_str(), esc_pass.c_str());
	if (success) {
		success = result.Next();
		if (success) {
			uint32_t id = result.GetUInt32(0);
			client->SetAccount(id);
			UpdateAccountIPAddress(id, client->GetIP());
			UpdateAccountClientVersion(id, client->GetVersion());
			client->SetAllowedClasses(result.GetUInt32(6));
			client->SetAllowedRaces(result.GetUInt32(7));
			client->SetCharacterSlots(result.GetUInt8(8));
		}
		// if user and pass check failed
		else {
			// if auto account creation is enabled
			if (g_worldServer.GetAutoAccountCreation()) {
				// see if there is already an account with this username
				success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s'", esc_user.c_str());

				if (success)
					success = result.Next();

				// account found which means bad password so return false
				if (success) {
					success = false;
				}
				// no account found so create one
				else {
					in_addr ip_addr;
					ip_addr.s_addr = client->GetIP();
					QueryResult result = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID, 
						"INSERT INTO account(`name`, `passwd`, `ip_address`, `last_client_version`) VALUES ('%s', SHA2('%s', 512), '%s', %u)",
						esc_user.c_str(), esc_pass.c_str(), inet_ntoa(ip_addr), client->GetVersion());
					if (result) {
						client->SetAccount(static_cast<uint32_t>(result.last_insert_id));
						success = true;
					}
				}
					
			}
			// No auto account so return false
			else {
				success = false;
			}
		}
	}

	return success;
}

bool WorldDatabase::UpdateAccountIPAddress(uint32_t account, uint32_t address) {
	in_addr ip_addr;
	ip_addr.s_addr = address;
	return Query("UPDATE `account` SET `ip_address` = '%s' WHERE `id` = %u", inet_ntoa(ip_addr), account);
}

bool WorldDatabase::UpdateAccountClientVersion(uint32_t account, uint32_t version) {
	return Query("UPDATE `account` SET `last_client_version` = %u WHERE `id` = %u", version, account);
}

bool WorldDatabase::LoadCharacterAppearances(uint32_t account, OP_AllCharactersDescReplyMsg_Packet* packet, uint8_t max_adv_level, uint8_t max_ts_level) {
	bool ret;
	DatabaseResult result;
	DatabaseResult result2;

	ret = Select(&result, "SELECT id, server_id, name, race, class, gender, deity, body_size, body_age, current_zone_id, level, tradeskill_class, tradeskill_level, soga_wing_type, soga_chest_type, soga_legs_type, soga_hair_type, soga_facial_hair_type, soga_model_type, legs_type, chest_type, wing_type, hair_type, facial_hair_type, model_type, tail_type, unix_timestamp(created_date), unix_timestamp(last_played) FROM characters WHERE account_id = %u AND deleted = 0", account);
	if (!ret)
		return ret;

	while (result.Next()) {
		packet->CharacterList.emplace_back(packet->GetVersion());
		auto& c = packet->CharacterList.back();
		c.account_id = account;

		c.charid = result.GetUInt32(0);
		c.server_id = result.GetUInt32(1);
		c.name = result.GetString(2);
		c.race = result.GetUInt8(3);
		c._class = result.GetUInt8(4);
		c.gender = result.GetUInt8(5);
		//c.deity = result.GetUInt8(6);
		Substruct_NetAppearance& app = c.appearance;
		app.sliders.bodyscale = result.GetUInt8(7);
		app.sliders.bumpscale = result.GetUInt8(8);
		uint32_t zone_id = result.GetUInt32(9);
		ret = Select(&result2, "SELECT name, description FROM zones WHERE id = %u", zone_id);
		if (!ret)
			return ret;
		if (result2.Next()) {
			c.zone = result2.IsNull(0) ? " " : result2.GetString(0);
			c.zonedesc = result2.IsNull(1) ? " " : result2.GetString(1);
			c.zonename2 = " ";
		}
		c.level = result.GetUInt32(10);
		c.tradeskill_class = result.GetUInt8(11);
		c.tradeskill_level = result.GetUInt32(12);

		if (c.level >= max_adv_level)
			packet->VeteranAdventureBonus++;
		if (c.tradeskill_level >= max_ts_level)
			packet->VeteranTradeskillBonus++;

		/* SOGA Appearances */
		//c.soga_wing_type = result.GetUInt16(13);
		//c.soga_cheek_type = result.GetUInt16(14);
		//c.soga_legs_type = result.GetUInt16(15);

		
		app.soga_hair.type = result.GetUInt32(16);
		app.soga_hair_face.type = result.GetUInt32(17);
		app.soga_race_type = result.GetUInt32(18);

		/* NORMAL Appearances */
		app.pants.type = result.GetUInt32(19);
		app.shirt.type = result.GetUInt32(20);
		app.wings.type = result.GetUInt32(21);
		app.hair.type = result.GetUInt32(22);
		app.hair_face.type = result.GetUInt32(23);
		app.race_type = result.GetUInt32(24);
		app.tail.type = result.GetUInt32(25);

		if (!result.IsNull(25))
			c.created_date = result.GetUInt32(26);
		if (!result.IsNull(26))
			c.last_played = result.GetUInt32(27);


		// TODO char_colors table
		ret = Select(&result2, "SELECT type, red, green, blue FROM char_colors WHERE char_id = %u", c.charid);
		if (!ret)
			return ret;

		while (result2.Next()) {
			std::string type = result2.GetString(0);

			if (type == "skin_color") {
				app.skin_color.Red = result2.GetUInt8(1);
				app.skin_color.Green = result2.GetUInt8(2);
				app.skin_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "eye_color") {
				app.eye_color.Red = result2.GetUInt8(1);
				app.eye_color.Green = result2.GetUInt8(2);
				app.eye_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "model_color") {
				app.model_color.Red = result2.GetUInt8(1);
				app.model_color.Green = result2.GetUInt8(2);
				app.model_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_color1") {
				app.hair_color1.Red = result2.GetUInt8(1);
				app.hair_color1.Green = result2.GetUInt8(2);
				app.hair_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_color2") {
				app.hair_color2.Red = result2.GetUInt8(1);
				app.hair_color2.Green = result2.GetUInt8(2);
				app.hair_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_highlight") {
				app.hair_scatter.Red = result2.GetUInt8(1);
				app.hair_scatter.Green = result2.GetUInt8(2);
				app.hair_scatter.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_type_color") {
				app.hair.color.Red = result2.GetUInt8(1);
				app.hair.color.Green = result2.GetUInt8(2);
				app.hair.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_type_highlight_color") {
				app.hair.highlight.Red = result2.GetUInt8(1);
				app.hair.highlight.Green = result2.GetUInt8(2);
				app.hair.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_face_color") {
				app.hair_face.color.Red = result2.GetUInt8(1);
				app.hair_face.color.Green = result2.GetUInt8(2);
				app.hair_face.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_face_highlight_color") {
				app.hair_face.highlight.Red = result2.GetUInt8(1);
				app.hair_face.highlight.Green = result2.GetUInt8(2);
				app.hair_face.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "wing_color1") {
				app.wings.color.Red = result2.GetUInt8(1);
				app.wings.color.Green = result2.GetUInt8(2);
				app.wings.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "wing_color2") {
				app.wings.highlight.Red = result2.GetUInt8(1);
				app.wings.highlight.Green = result2.GetUInt8(2);
				app.wings.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "tail_color1") {
				app.tail.color.Red = result2.GetUInt8(1);
				app.tail.color.Green = result2.GetUInt8(2);
				app.tail.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "tail_color2") {
				app.tail.highlight.Red = result2.GetUInt8(1);
				app.tail.highlight.Green = result2.GetUInt8(2);
				app.tail.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "shirt_color") {
				app.shirt.color.Red = result2.GetUInt8(1);
				app.shirt.color.Green = result2.GetUInt8(2);
				app.shirt.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown_chest_color") {
				app.shirt.highlight.Red = result2.GetUInt8(1);
				app.shirt.highlight.Green = result2.GetUInt8(2);
				app.shirt.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "pants_color") {
				app.pants.color.Red = result2.GetUInt8(1);
				app.pants.color.Green = result2.GetUInt8(2);
				app.pants.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown_legs_color") {
				app.pants.highlight.Red = result2.GetUInt8(1);
				app.pants.highlight.Green = result2.GetUInt8(2);
				app.pants.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown9") {
				app.sliders.skull[0] = result2.GetUInt8(1);
				app.sliders.skull[1] = result2.GetUInt8(2);
				app.sliders.skull[2] = result2.GetUInt8(3);
			}
			else if (type == "eye_type") {
				app.sliders.eyes[0] = result2.GetUInt8(1);
				app.sliders.eyes[1] = result2.GetUInt8(2);
				app.sliders.eyes[2] = result2.GetUInt8(3);
			}
			else if (type == "ear_type") {
				app.sliders.ears[0] = result2.GetUInt8(1);
				app.sliders.ears[1] = result2.GetUInt8(2);
				app.sliders.ears[2] = result2.GetUInt8(3);
			}
			else if (type == "eye_brow_type") {
				app.sliders.eyebrow[0] = result2.GetUInt8(1);
				app.sliders.eyebrow[1] = result2.GetUInt8(2);
				app.sliders.eyebrow[2] = result2.GetUInt8(3);
			}
			else if (type == "cheek_type") {
				app.sliders.cheeks[0] = result2.GetUInt8(1);
				app.sliders.cheeks[1] = result2.GetUInt8(2);
				app.sliders.cheeks[2] = result2.GetUInt8(3);
			}
			else if (type == "lip_type") {
				app.sliders.mouth[0] = result2.GetUInt8(1);
				app.sliders.mouth[1] = result2.GetUInt8(2);
				app.sliders.mouth[2] = result2.GetUInt8(3);
			}
			else if (type == "chin_type") {
				app.sliders.chin[0] = result2.GetUInt8(1);
				app.sliders.chin[1] = result2.GetUInt8(2);
				app.sliders.chin[2] = result2.GetUInt8(3);
			}
			else if (type == "nose_type") {
				app.sliders.nose[0] = result2.GetUInt8(1);
				app.sliders.nose[1] = result2.GetUInt8(2);
				app.sliders.nose[2] = result2.GetUInt8(3);
			}
			else if (type == "body_size") {
				app.sliders.bodyscale = result2.GetInt8(1);
			}
			else if (type == "soga_skin_color") {
				app.soga_skin_color.Red = result2.GetUInt8(1);
				app.soga_skin_color.Green = result2.GetUInt8(2);
				app.soga_skin_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_eye_color") {
				app.soga_eye_color.Red = result2.GetUInt8(1);
				app.soga_eye_color.Green = result2.GetUInt8(2);
				app.soga_eye_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_model_color") {
				app.soga_model_color.Red = result2.GetUInt8(1);
				app.soga_model_color.Green = result2.GetUInt8(2);
				app.soga_model_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_color1") {
				app.soga_hair_color1.Red = result2.GetUInt8(1);
				app.soga_hair_color1.Green = result2.GetUInt8(2);
				app.soga_hair_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_color2") {
				app.soga_hair_color2.Red = result2.GetUInt8(1);
				app.soga_hair_color2.Green = result2.GetUInt8(2);
				app.soga_hair_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_highlight") {
				app.soga_hair.highlight.Red = result2.GetUInt8(1);
				app.soga_hair.highlight.Green = result2.GetUInt8(2);
				app.soga_hair.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_type_color") {
				app.soga_hair.color.Red = result2.GetUInt8(1);
				app.soga_hair.color.Green = result2.GetUInt8(2);
				app.soga_hair.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_type_highlight_color") {
				app.soga_hair.highlight.Red = result2.GetUInt8(1);
				app.soga_hair.highlight.Green = result2.GetUInt8(2);
				app.soga_hair.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_face_color") {
				app.soga_hair_face.color.Red = result2.GetUInt8(1);
				app.soga_hair_face.color.Green = result2.GetUInt8(2);
				app.soga_hair_face.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_face_highlight_color") {
				app.soga_hair_face.highlight.Red = result2.GetUInt8(1);
				app.soga_hair_face.highlight.Green = result2.GetUInt8(2);
				app.soga_hair_face.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_wing_color1") {
				app.wings.color.Red = result2.GetUInt8(1);
				app.wings.color.Green = result2.GetUInt8(2);
				app.wings.color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_wing_color2") {
				app.wings.highlight.Red = result2.GetUInt8(1);
				app.wings.highlight.Green = result2.GetUInt8(2);
				app.wings.highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_unknown13") {
				app.sogaSliders.skull[0] = result2.GetUInt8(1);
				app.sogaSliders.skull[1] = result2.GetUInt8(2);
				app.sogaSliders.skull[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_eye_type") {
				app.sogaSliders.eyes[0] = result2.GetUInt8(1);
				app.sogaSliders.eyes[1] = result2.GetUInt8(2);
				app.sogaSliders.eyes[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_ear_type") {
				app.sogaSliders.ears[0] = result2.GetUInt8(1);
				app.sogaSliders.ears[1] = result2.GetUInt8(2);
				app.sogaSliders.ears[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_eye_brow_type") {
				app.sogaSliders.eyebrow[0] = result2.GetUInt8(1);
				app.sogaSliders.eyebrow[1] = result2.GetUInt8(2);
				app.sogaSliders.eyebrow[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_cheek_type") {
				app.sogaSliders.cheeks[0] = result2.GetUInt8(1);
				app.sogaSliders.cheeks[1] = result2.GetUInt8(2);
				app.sogaSliders.cheeks[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_lip_type") {
				app.sogaSliders.mouth[0] = result2.GetUInt8(1);
				app.sogaSliders.mouth[1] = result2.GetUInt8(2);
				app.sogaSliders.mouth[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_chin_type") {
				app.sogaSliders.chin[0] = result2.GetUInt8(1);
				app.sogaSliders.chin[1] = result2.GetUInt8(2);
				app.sogaSliders.chin[2] = result2.GetUInt8(3);
			}
			else if (type == "soga_nose_type") {
				app.sogaSliders.nose[0] = result2.GetUInt8(1);
				app.sogaSliders.nose[1] = result2.GetUInt8(2);
				app.sogaSliders.nose[2] = result2.GetUInt8(3);
			}
		}

		ret = Select(&result2, "SELECT ci.slot, ia.equip_type, ia.red, ia.green, ia.blue, ia.highlight_red, ia.highlight_green, ia.highlight_blue FROM character_items ci INNER JOIN item_appearances ia ON ci.item_id = ia.item_id WHERE ci.type = 'EQUIPPED' AND ci.char_id = %u ORDER BY ci.slot ASC", c.charid);
		if (!ret)
			return ret;

		while (result2.Next()) {
			uint8_t slot = result2.GetUInt8(0);
			app.equip[slot].type = result2.GetUInt16(1);
			app.equip[slot].color.Red = result2.GetUInt8(2);
			app.equip[slot].color.Green = result2.GetUInt8(3);
			app.equip[slot].color.Blue = result2.GetUInt8(4);
			app.equip[slot].highlight.Red = result2.GetUInt8(5);
			app.equip[slot].highlight.Green = result2.GetUInt8(6);
			app.equip[slot].highlight.Blue = result2.GetUInt8(7);
		}
		
		ret = Select(&result2, "SELECT s.name FROM config_world s INNER JOIN characters c WHERE s.id = c.server_id AND c.id = %u", c.charid);
		if (!ret)
			return ret;

		if (result2.Next())
			c.server_name = result2.GetString(0);
	}

	packet->NumCharacters = (uint8_t)packet->CharacterList.size();

	return ret;
}

bool WorldDatabase::DeleteCharacter(uint32_t account_id, uint32_t char_id, std::string name) {
	QueryResult result = QueryWithFetchedResult(QUERY_RESULT_FLAG_AFFECTED_ROWS, 
		"UPDATE characters SET deleted = 1 WHERE id = %u AND account_id = %u AND name = '%s'", char_id, account_id, name.c_str());
	return (result && result.affected_rows == 1);
}

bool WorldDatabase::SaveClientLog(const std::string& type, const std::string& message, uint32_t version) {
	string type_esc = Escape(type);
	string message_esc = Escape(message);
	bool ret = Query("INSERT INTO log_messages (log_type, message, client_data_version, log_time) VALUES ('%s', '%s', %u, %u)", type_esc.c_str(), message_esc.c_str(), version, Timer::GetUnixTimeStamp());
	return ret;
}

uint32_t WorldDatabase::CreateCharacter(uint32_t account_id, OP_CreateCharacterRequestMsg_Packet* packet) {
	const char* create_char =
		"INSERT INTO characters (account_id, server_id, name, race, class, gender, deity, body_size, body_age, soga_wing_type, soga_chest_type,"
		"soga_legs_type, soga_hair_type, soga_model_type, legs_type, chest_type, wing_type, hair_type, model_type, facial_hair_type, "
		"soga_facial_hair_type, created_date, last_saved, admin_status, tail_type) "
		"VALUES (%i, %i, '%s', %i, %i, %i, %i, %f, %f, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, now(), unix_timestamp(), NULL, %u)";
	 
	const Substruct_CharacterCustomization& custom = packet->customization;
	const Substruct_CharacterCustomization& soga = packet->soga_customization;

	std::array<uint32_t, 13> ids = GetCreateAppearanceIDs(custom, soga);

	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_LAST_INSERT_ID, create_char,
		account_id,
		packet->server_id,
		packet->name.c_str(),
		packet->race,
		packet->_class,
		packet->gender,
		packet->deity,
		custom.floatSliders.bodyscale,
		custom.floatSliders.bumpscale,
		ids[0],
		ids[1],
		ids[2],
		ids[3],
		ids[4],
		ids[5],
		ids[6],
		ids[7],
		ids[8],
		ids[9],
		ids[10],
		ids[11],
		ids[12]
		);

	if (!res) {
		LogError(LOG_DATABASE, 0, "Error in SaveCharacter query!");
		return 0;
	}

	uint32_t char_id = static_cast<uint32_t>(res.last_insert_id);

	auto character = std::make_shared<Character>();
	CharacterBasicInfo& bi = character->basicInfo;
	bi.accountID = account_id;
	bi.advClass = packet->_class;
	bi.characterID = char_id;
	bi.characterName = packet->name;
	bi.race = packet->race;
	
	g_characterList.AddCharacter(character);
	
	UpdateStartingFactions(char_id, packet->starting_zone);
	UpdateStartingZone(char_id, packet->_class, packet->race, packet->starting_zone);
	// Starting here
	UpdateStartingItems(char_id, packet->_class, packet->race);
	UpdateStartingSkills(char_id, packet->_class, packet->race);
	UpdateStartingSpells(char_id, packet->_class, packet->race);
	UpdateStartingSkillbar(char_id, packet->_class, packet->race);
	UpdateStartingTitles(char_id, packet->_class, packet->race, packet->gender);
	InsertCharacterStats(char_id, packet->_class, packet->race);
	// and ending here these functions still need to be implemented

	//AddNewPlayerToServerGuild(loginID, char_id);

	SaveCharacterColors(char_id, "skin_color", custom.skin_color);
	SaveCharacterColors(char_id, "model_color", custom.model_color);
	SaveCharacterColors(char_id, "eye_color", custom.eye_color);
	SaveCharacterColors(char_id, "hair_color1", custom.hair_color1);
	SaveCharacterColors(char_id, "hair_color2", custom.hair_color2);
	SaveCharacterColors(char_id, "hair_highlight", custom.hair_highlight);
	SaveCharacterColors(char_id, "hair_type_color", custom.hairAsset.color);
	SaveCharacterColors(char_id, "hair_type_highlight_color", custom.hairAsset.highlight);
	SaveCharacterColors(char_id, "hair_face_color", custom.faceAsset.color);
	SaveCharacterColors(char_id, "hair_face_highlight_color", custom.faceAsset.highlight);
	SaveCharacterColors(char_id, "wing_color1", custom.wingAsset.color);
	SaveCharacterColors(char_id, "wing_color2", custom.wingAsset.highlight);
	SaveCharacterColors(char_id, "shirt_color", custom.chestAsset.color);
	SaveCharacterColors(char_id, "unknown_chest_color", custom.chestAsset.highlight);
	SaveCharacterColors(char_id, "pants_color", custom.legsAsset.color);
	SaveCharacterColors(char_id, "unknown_legs_color", custom.legsAsset.highlight);
	SaveCharacterColors(char_id, "unknown9", custom.sliders.skull);
	SaveCharacterColors(char_id, "eye_type", custom.sliders.eyes);
	SaveCharacterColors(char_id, "ear_type", custom.sliders.ears);
	SaveCharacterColors(char_id, "eye_brow_type", custom.sliders.eyebrow);
	SaveCharacterColors(char_id, "cheek_type", custom.sliders.cheeks);
	SaveCharacterColors(char_id, "lip_type", custom.sliders.mouth);
	SaveCharacterColors(char_id, "chin_type", custom.sliders.chin);
	SaveCharacterColors(char_id, "nose_type", custom.sliders.nose);
	SaveCharacterFloats(char_id, "body_size", custom.sliders.bodyscale, 0, 0);
	SaveCharacterColors(char_id, "tail_color1", custom.tailAsset.color);
	SaveCharacterColors(char_id, "tail_color2", custom.tailAsset.highlight);

	SaveCharacterColors(char_id, "soga_skin_color", soga.skin_color);
	SaveCharacterColors(char_id, "soga_model_color", soga.model_color);
	SaveCharacterColors(char_id, "soga_eye_color", soga.eye_color);
	SaveCharacterColors(char_id, "soga_hair_color1", soga.hair_color1);
	SaveCharacterColors(char_id, "soga_hair_color2", soga.hair_color2);
	SaveCharacterColors(char_id, "soga_hair_highlight", soga.hair_highlight);
	SaveCharacterColors(char_id, "soga_hair_type_color", soga.hairAsset.color);
	SaveCharacterColors(char_id, "soga_hair_type_highlight_color", soga.hairAsset.highlight);
	SaveCharacterColors(char_id, "soga_hair_face_color", soga.faceAsset.color);
	SaveCharacterColors(char_id, "soga_hair_face_highlight_color", soga.faceAsset.highlight);
	SaveCharacterColors(char_id, "soga_wing_color1", soga.wingAsset.color);
	SaveCharacterColors(char_id, "soga_wing_color2", soga.wingAsset.highlight);
	SaveCharacterColors(char_id, "soga_shirt_color", soga.chestAsset.color);
	SaveCharacterColors(char_id, "soga_unknown_chest_color", soga.chestAsset.highlight);
	SaveCharacterColors(char_id, "soga_pants_color", soga.legsAsset.color);
	SaveCharacterColors(char_id, "soga_unknown_legs_color", soga.legsAsset.highlight);
	SaveCharacterColors(char_id, "soga_unknown13", soga.sliders.skull);
	SaveCharacterColors(char_id, "soga_eye_type", soga.sliders.eyes);
	SaveCharacterColors(char_id, "soga_ear_type", soga.sliders.ears);
	SaveCharacterColors(char_id, "soga_eye_brow_type", soga.sliders.eyebrow);
	SaveCharacterColors(char_id, "soga_cheek_type", soga.sliders.cheeks);
	SaveCharacterColors(char_id, "soga_lip_type", soga.sliders.mouth);
	SaveCharacterColors(char_id, "soga_chin_type", soga.sliders.chin);
	SaveCharacterColors(char_id, "soga_nose_type", soga.sliders.nose);

	return char_id;
}

//13 asset ids stored in the character table from char create, add more as needed in order
//TODO: should probably just cache these
std::array<uint32_t, 13> WorldDatabase::GetCreateAppearanceIDs(const Substruct_CharacterCustomization& custom, const Substruct_CharacterCustomization& soga) {
	std::array<uint32_t, 13> ret;
	ret.fill(0);

	std::array<std::string, 13> lookupRef;

	lookupRef[0] = soga.wingAsset.file;
	lookupRef[1] = soga.chestAsset.file;
	lookupRef[2] = soga.legsAsset.file;
	lookupRef[3] = soga.hairAsset.file;
	lookupRef[4] = soga.race_file;
	lookupRef[5] = custom.legsAsset.file;
	lookupRef[6] = custom.chestAsset.file;
	lookupRef[7] = custom.wingAsset.file;
	lookupRef[8] = custom.hairAsset.file;
	lookupRef[9] = custom.race_file;
	lookupRef[10] = custom.faceAsset.file;
	lookupRef[11] = soga.faceAsset.file;
	lookupRef[12] = custom.tailAsset.file;

	std::ostringstream ss;

	ss << "SELECT appearance_id, `name` FROM appearances WHERE `name` IN (";
	
	bool bFirst = true;

	for (auto& itr : lookupRef) {
		if (itr.empty()) continue;
		else if (bFirst) bFirst = false;
		else ss << ',';

		ss << '\'' << Escape(itr) << '\'';
	}

	ss << ");";

	DatabaseResult result;

	if (!Select(&result, "%s", ss.str().c_str())) {
		//Error
		return ret;
	}

	while (result.Next()) {
		for (int i = 0; i < lookupRef.size(); i++) {
			if (lookupRef[i] == result.GetString(1)) {
				ret[i] = result.GetUInt32(0);
			}
		}
	}

	return ret;
}

void WorldDatabase::UpdateStartingFactions(uint32_t char_id, uint16_t choice) {
	Query("INSERT INTO character_factions (char_id, faction_id, faction_level) SELECT %u, faction_id, value FROM starting_factions WHERE starting_city = %u", char_id, choice);
}

void WorldDatabase::UpdateStartingZone(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint16_t choice) {
	DatabaseResult result;
	bool success;

	
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default zone for race: %i, class: %i for char_id: %u (choice: %i)", race_id, class_id, char_id, choice);

	// first, check to see if there is a starting_zones record for this race/class/choice combo (now using extended Archetype/BaseClass/Class combos
	success = Select(&result, "SELECT `name` FROM starting_zones sz, zones z WHERE sz.zone_id = z.id AND class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255) AND choice IN (%i, 255)",
		classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id, choice);

	// TODO: verify client version so clients do not crash trying to enter zones they do not own (paks)
	if (success && result.Next()) {
		string zone_name = result.GetString(0);

		success = Query("UPDATE characters c, zones z, starting_zones sz SET c.current_zone_id = z.id, c.x = z.safe_x, c.y = z.safe_y, c.z = z.safe_z, c.starting_city = %i WHERE z.id = sz.zone_id AND sz.class_id IN (%i, %i, %i, 255) AND sz.race_id IN (%i, 255) AND sz.choice IN (%i, 255) AND c.id = %u",
			choice, classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id, choice, char_id);

		if (!success) {
			//LogWrite(PLAYER__ERROR, 0, "Player", "Error in UpdateStartingZone custom starting_zones, query: '%s': %s", query.GetQuery(), query.GetError());
			return;
		}

		//if (AffectedRows() > 0) {
			//LogWrite(PLAYER__DEBUG, 0, "Player", "Setting New Character Starting Zone to '%s' FROM starting_zones table.", zone_name.c_str());
			return;
		//}
	}
	else {
		// there was no matching starting_zone value, so use default 'choice' starting city
		success = Query("UPDATE characters c, zones z SET c.current_zone_id = z.id, c.x = z.safe_x, c.y = z.safe_y, c.z = z.safe_z, c.starting_city = %i WHERE z.start_zone = %i and c.id = %u",
			choice, choice, char_id);

		if (!success) {
			//LogWrite(PLAYER__ERROR, 0, "Player", "Error in UpdateStartingZone player choice, query: '%s': %s", query.GetQuery(), query.GetError());
			return;
		}

		//if (AffectedRows() > 0) {
			//LogWrite(PLAYER__DEBUG, 0, "Player", "Setting New Character Starting Zone to '%s' FROM player choice.", GetStartingZoneName(choice).c_str());
			return;
		//}
	}

	// if we are here, it's a bad thing. zone tables have no start_city values to match client 'choice', so throw the player into zone according to R_World::DefaultStartingZoneID rule.
	// shout a few warnings so the admin fixes this asap!
	uint16_t default_zone_id = 1; // rule_manager.GetGlobalRule(R_World, DefaultStartingZoneID)->GetInt16();

	//LogWrite(WORLD__WARNING, 0, "World", "No Starting City defined for player choice: %i! BAD! BAD! BAD! Defaulting player to zone %i.", choice, default_zone_id);

	success = Query("UPDATE characters c, zones z SET c.current_zone_id = z.id, c.x = z.safe_x, c.y = z.safe_y, c.z = z.safe_z, c.heading = z.safe_heading, c.starting_city = 1 WHERE z.id = %i and c.id = %u", default_zone_id, char_id);

	if (!success) {
		//LogWrite(PLAYER__ERROR, 0, "Player", "Error in UpdateStartingZone default zone %i, query: '%s': %s", default_zone_id, query.GetQuery(), query.GetError());
		return;
	}

	//if (AffectedRows() > 0) {
		//LogWrite(PLAYER__DEBUG, 0, "Player", "Setting New Character Starting Zone to '%s' due to no valid options!", GetZoneName(1)->c_str());
	//}

	return;
}

void WorldDatabase::SaveCharacterColors(uint32_t char_id, const char* type, EQ2ColorFloat color) {
	uint8_t red = (uint8_t)(color.Red * 255);
	uint8_t green = (uint8_t)(color.Green * 255);
	uint8_t blue = (uint8_t)(color.Blue * 255);
	if (!Query("INSERT INTO char_colors (char_id, type, red, green, blue) VALUES (%u, '%s', %u, %u, %u)", char_id, type, red, green, blue)) {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in SaveCharacterColors query: %s", GetError());
	}
}

void WorldDatabase::SaveCharacterColors(uint32_t char_id, const char* type, const int8_t* color) {
	if (!Query("INSERT INTO char_colors (char_id, signed_value, type, red, green, blue) VALUES (%u, %u, '%s', %i, %i, %i)", char_id, 1, type, color[0], color[1], color[2])) {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in SaveCharacterColors query: %s", GetError());
	}
}

void WorldDatabase::SaveCharacterFloats(uint32_t char_id, const char* type, float float1, float float2, float float3) {
	if (!Query("INSERT INTO char_colors (char_id, type, red, green, blue, signed_value) VALUES (%u, '%s', %i, %i, %i, 1)", char_id, type, (int8_t)(float1 * 100), (int8_t)(float2 * 100), (int8_t)(float3 * 100))) {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in SaveCharacterFloats query: %s", GetError());
	}
}

void WorldDatabase::UpdateStartingItems(uint32_t char_id, uint8_t class_id, uint8_t race_id, bool base_class) {
	DatabaseResult result;

	struct StartingItem {
		std::string type;
		uint32_t id;
		std::string creator;
		uint8_t condition;
		uint8_t attuned;
		uint8_t count;
	};

	//success = Select(&result, "SELECT `type`, item_id, creator, condition_, attuned, count FROM starting_items WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255) ORDER BY id", classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id);

	/*
	SELECT si.`type`, si.item_id, si.creator, si.condition_, si.attuned, si.`count`, i.item_type FROM starting_items si
	INNER JOIN items i
	ON si.item_id = i.id
	WHERE si.class_id IN (%u, %u, %u, 255) AND si.race_id IN (%u, 255) ORDER BY si.id
	*/

	/*
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default items for race: %i, class: %i for char_id: %u", race_id, class_id, char_id);
	Query query;
	Query query2;
	vector<Item*> items;
	vector<Item*> bags;
	map<int32, int8> total_slots;
	map<int32, int8> slots_left;
	map<int8, bool> equip_slots;
	map<Item*, StartingItem> item_list;
	int32 item_id = 0;
	Item* item = 0;
	StartingItem* starting_item = 0;
	//first get a list of the starting items for the character
	MYSQL_RES* result = 0;
	
	result = query2.RunQuery2(Q_SELECT, "SELECT `type`, item_id, creator, condition_, attuned, count FROM starting_items WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255) ORDER BY id", classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id);
	if (result && mysql_num_rows(result) > 0) {
		MYSQL_ROW row;
		while (result && (row = mysql_fetch_row(result))) {
			item_id = atoul(row[1]);
			item = master_item_list.GetItem(item_id);
			if (item) {
				starting_item = &(item_list[item]);
				starting_item->type = (row[0]) ? string(row[0]) : "";
				starting_item->item_id = atoul(row[1]);
				starting_item->creator = (row[2]) ? string(row[2]) : "";
				starting_item->condition = atoi(row[3]);
				starting_item->attuned = atoi(row[4]);
				starting_item->count = atoi(row[5]);
				item = master_item_list.GetItem(starting_item->item_id);
				if (item) {
					if (bags.size() < NUM_INV_SLOTS && item->IsBag() && item->details.num_slots > 0)
						bags.push_back(item);
					else
						items.push_back(item);
				}
			}
		}
	}
	slots_left[0] = NUM_INV_SLOTS;
	//next create the bags in the inventory
	for (int8 i = 0; i < bags.size(); i++) {
		item = bags[i];
		query.RunQuery2(Q_INSERT, "insert into character_items (char_id, type, slot, item_id, creator, condition_, attuned, bag_id, count) values(%u, '%s', %i, %u, '%s', %i, %i, %u, %i)",
			char_id, item_list[item].type.c_str(), i, item_list[item].item_id, getSafeEscapeString(item_list[item].creator.c_str()).c_str(), item_list[item].condition, item_list[item].attuned, 0, item_list[item].count);
		slots_left[query.GetLastInsertedID()] = item->details.num_slots;
		total_slots[query.GetLastInsertedID()] = item->details.num_slots;
		slots_left[0]--;
	}
	map<int32, int8>::iterator itr;
	int32 inv_slot = 0;
	int8  slot = 0;
	//finally process the rest of the items, placing them in the first available slot
	for (int32 x = 0; x < items.size(); x++) {
		item = items[x];
		if (item_list[item].type.find("NOT") < 0xFFFFFFFF) { // NOT-EQUIPPED Items
			for (itr = slots_left.begin(); itr != slots_left.end(); itr++) {
				if (itr->second > 0) {
					if (itr->first == 0 && slots_left.size() > 1) //we want items to go into bags first, then inventory after bags are full
						continue;
					inv_slot = itr->first;
					slot = total_slots[itr->first] - itr->second;
					itr->second--;
					if (itr->second == 0)
						slots_left.erase(itr);
					break;
				}
			}
			query.RunQuery2(Q_INSERT, "insert into character_items (char_id, type, slot, item_id, creator, condition_, attuned, bag_id, count) values(%u, '%s', %i, %u, '%s', %i, %i, %u, %i)",
				char_id, item_list[item].type.c_str(), slot, item_list[item].item_id, getSafeEscapeString(item_list[item].creator.c_str()).c_str(), item_list[item].condition, item_list[item].attuned, inv_slot, item_list[item].count);
		}
		else { //EQUIPPED Items
			for (int8 i = 0; i < item->slot_data.size(); i++) {
				if (equip_slots.count(item->slot_data[i]) == 0) {
					equip_slots[item->slot_data[i]] = true;
					query.RunQuery2(Q_INSERT, "insert into character_items (char_id, type, slot, item_id, creator, condition_, attuned, bag_id, count) values(%u, '%s', %i, %u, '%s', %i, %i, %u, %i)",
						char_id, item_list[item].type.c_str(), item->slot_data[i], item_list[item].item_id, getSafeEscapeString(item_list[item].creator.c_str()).c_str(), item_list[item].condition, item_list[item].attuned, 0, item_list[item].count);
					break;
				}
			}
		}
	}
	*/
}

void WorldDatabase::UpdateStartingSkills(uint32_t char_id, uint8_t class_id, uint8_t race_id) {
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default skills for race: %i, class: %i for char_id: %u", race_id, class_id, char_id);
	Query("INSERT IGNORE INTO character_skills (char_id, skill_id, current_val, max_val) SELECT %u, skill_id, current_val, max_val FROM starting_skills WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255)",
		char_id, classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id);
}

void WorldDatabase::UpdateStartingSpells(uint32_t char_id, uint8_t class_id, uint8_t race_id) {
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default spells for race: %i, class: %i for char_id: %u", race_id, class_id, char_id);
	Query("INSERT IGNORE INTO character_spells (char_id, spell_id, tier, knowledge_slot) SELECT %u, spell_id, tier, knowledge_slot FROM starting_spells WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255)",
		char_id, classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id);
}

void WorldDatabase::UpdateStartingSkillbar(uint32_t char_id, uint8_t class_id, uint8_t race_id) {
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default skillbar for race: %i, class: %i for char_id: %u", race_id, class_id, char_id);
	Query("INSERT IGNORE INTO character_skillbar (char_id, type, hotbar, spell_id, slot, text_val) SELECT %u, type, hotbar, spell_id, slot, text_val FROM starting_skillbar WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255)",
		char_id, classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id);
}

void WorldDatabase::UpdateStartingTitles(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint8_t gender_id) {
	//LogWrite(PLAYER__DEBUG, 0, "Player", "Adding default titles for race: %i, class: %i, gender: %i for char_id: %u", race_id, class_id, gender_id, char_id);
	Query("INSERT IGNORE INTO character_titles (char_id, title_id) SELECT %u, title_id FROM  starting_titles WHERE class_id IN (%i, %i, %i, 255) AND race_id IN (%i, 255) and gender_id IN (%i, 255)",
		char_id, classes.GetBaseClass(class_id), classes.GetSecondaryBaseClass(class_id), class_id, race_id, gender_id);
}

bool WorldDatabase::InsertCharacterStats(uint32_t character_id, uint8_t class_id, uint8_t race_id) {

	/* Blank record */
	Query("INSERT INTO `character_details` (`char_id`) VALUES (%u)", character_id);

	/* Using the class id and race id */
	QueryResult res = QueryWithFetchedResult(QUERY_RESULT_FLAG_AFFECTED_ROWS,
		"UPDATE character_details c, starting_details s SET c.max_hp = s.max_hp, c.hp = s.max_hp, c.max_power = s.max_power, c.power = s.max_power, c.str = s.str, c.sta = s.sta, c.agi = s.agi, c.wis = s.wis, c.intel = s.intel,c.heat = s.heat, c.cold = s.cold, c.magic = s.magic, c.mental = s.mental, c.divine = s.divine, c.disease = s.disease, c.poison = s.poison, c.coin_copper = s.coin_copper, c.coin_silver = s.coin_silver, c.coin_gold = s.coin_gold, c.coin_plat = s.coin_plat, c.status_points = s.status_points WHERE s.race_id = %d AND class_id = %d AND char_id = %u", race_id, class_id, character_id);
	if (res && res.affected_rows)
		return true;

	/* Using the class id and race id = 255 */
	res = QueryWithFetchedResult(QUERY_RESULT_FLAG_AFFECTED_ROWS,
		"UPDATE character_details c, starting_details s SET c.max_hp = s.max_hp, c.hp = s.max_hp, c.max_power = s.max_power, c.power = s.max_power, c.str = s.str, c.sta = s.sta, c.agi = s.agi, c.wis = s.wis, c.intel = s.intel,c.heat = s.heat, c.cold = s.cold, c.magic = s.magic, c.mental = s.mental, c.divine = s.divine, c.disease = s.disease, c.poison = s.poison, c.coin_copper = s.coin_copper, c.coin_silver = s.coin_silver, c.coin_gold = s.coin_gold, c.coin_plat = s.coin_plat, c.status_points = s.status_points WHERE s.race_id = 255 AND class_id = %d AND char_id = %u", class_id, character_id);
	if (res && res.affected_rows)
		return true;

	/* Using class id = 255 and the race id */
	res = QueryWithFetchedResult(QUERY_RESULT_FLAG_AFFECTED_ROWS,
	"UPDATE character_details c, starting_details s SET c.max_hp = s.max_hp, c.hp = s.max_hp, c.max_power = s.max_power, c.power = s.max_power, c.str = s.str, c.sta = s.sta, c.agi = s.agi, c.wis = s.wis, c.intel = s.intel,c.heat = s.heat, c.cold = s.cold, c.magic = s.magic, c.mental = s.mental, c.divine = s.divine, c.disease = s.disease, c.poison = s.poison, c.coin_copper = s.coin_copper, c.coin_silver = s.coin_silver, c.coin_gold = s.coin_gold, c.coin_plat = s.coin_plat, c.status_points = s.status_points WHERE s.race_id = %d AND class_id = 255 AND char_id = %u", race_id, character_id);
	if (res && res.affected_rows)
		return true;

	/* Using class id = 255 and race id = 255 */
	res = QueryWithFetchedResult(QUERY_RESULT_FLAG_AFFECTED_ROWS,
	"UPDATE character_details c, starting_details s SET c.max_hp = s.max_hp, c.hp = s.max_hp, c.max_power = s.max_power, c.power = s.max_power, c.str = s.str, c.sta = s.sta, c.agi = s.agi, c.wis = s.wis, c.intel = s.intel,c.heat = s.heat, c.cold = s.cold, c.magic = s.magic, c.mental = s.mental, c.divine = s.divine, c.disease = s.disease, c.poison = s.poison, c.coin_copper = s.coin_copper, c.coin_silver = s.coin_silver, c.coin_gold = s.coin_gold, c.coin_plat = s.coin_plat, c.status_points = s.status_points WHERE s.race_id = 255 AND class_id = 255 AND char_id = %u", character_id);
	if (res && res.affected_rows)
		return true;

	return false;
}

uint8_t WorldDatabase::CheckNameFilter(const char* name) {
	// the minimum 4 is enforced by the client too
	if (!name || strlen(name) < 4 || strlen(name) > 15) // Even 20 char length is long...
		return BADNAMELENGTH_REPLY;
	unsigned char* checkname = (unsigned char*)name;
	for (uint8_t i = 0; i < strlen(name); i++) {
		if (!alpha_check(checkname[i]))
			return NAMEINVALID_REPLY;
	}

	DatabaseResult result;
	bool success;

	LogDebug(LOG_WORLD, 0, "Name check on: %s", name);
	success = Select(&result, "SELECT count(*) FROM characters WHERE name='%s' AND deleted = 0", name);
	if (success && result.Next()) {
		if (result.GetUInt32(0) > 0)
			return NAMETAKEN_REPLY;
	}
	else {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in CheckNameFilter (name exist check) (Name query '%s': %s", query.GetQuery(), query.GetError());
	}

	LogDebug(LOG_WORLD, 0, "Name check on: %s (Bots table)", name);
	success = Select(&result, "SELECT count(*) FROM bots WHERE name='%s'", name);
	if (success && result.Next()) {
		if (result.GetUInt32(0) > 0)
			return NAMETAKEN_REPLY;
	}
	else {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in CheckNameFilter (name exist check, bot table) (Name query '%s': %s", query3.GetQuery(), query3.GetError());
	}

	
	success = Select(&result, "SELECT count(*) FROM name_filter WHERE '%s' like name", name);
	if (success && result.Next()) {
		if (result.GetUInt32(0) > 0)
			return NAMEFILTER_REPLY;
		else
			return CREATESUCCESS_REPLY;
	}
	else {
		//LogWrite(WORLD__ERROR, 0, "World", "Error in CheckNameFilter (name_filter check) query '%s': %s", query.GetQuery(), query.GetError());
	}

	return UNKNOWNERROR_REPLY;
}

bool WorldDatabase::LoadServerVariables(WorldServer* s) {
	bool success = true;
	DatabaseResult result;
	/*
	success = Select(&result, "SELECT variable_name, variable_value FROM variables");
	if (!success)
		return success;

	while (result.Next()) {
		std::string var = result.GetString(0);
		std::string var_val = result.GetString(1);
		if (var == "server_name") {
			s->SetName(var_val);
		}
		else if (var == "max_characters_per_account") {
			s->SetMaxCharactersPerAccount(static_cast<uint8_t>(atoul(var_val.c_str())));
		}
		else if (var == "allowed_races") {
			s->SetAllowedRaces(atoul(var_val.c_str()));
		}
		else if (var == "allowed_classes") {
			s->SetAllowedClasses(atoul(var_val.c_str()));
		}
		else if (var == "max_level") {
			s->SetMaxLevel(static_cast<uint8_t>(atoul(var_val.c_str())));
		}
	}
	*/
	return success;
}

bool WorldDatabase::LoadServerConfig(WorldServer* s) {
	bool success = true;
	DatabaseResult result;

	success = Select(&result, "SELECT * FROM config_world WHERE id = %u", s->GetID());
	if (!success)
		return success;

	if (result.Next()) {
		s->SetName(result.GetString(1));
		s->SetLocked(result.GetBool(2));
		s->SetCharactersSlotsPerAccount(result.GetUInt8(3));
		s->SetMaxAdvLevel(result.GetUInt8(4));
		s->SetMaxTSLevel(result.GetUInt8(5));
		s->SetAllowedRaces(result.GetUInt32(6));
		s->SetAllowedClasses(result.GetUInt32(7));
		s->SetAutoAccountCreation(result.GetBool(8));
	}

	success = Select(&result, "SELECT * FROM character_create_equipment WHERE type = 'Normal'");
	if (!success)
		return success;

	while (result.Next()) {
		uint8_t class_id = result.GetUInt8(2);

		OP_LoginReplyMsg_Packet::ClassItem::StartingItem si;
		si.SlotID = result.GetUInt8(3);
		si.ModelID = result.GetUInt32(4);
		si.UseColor = result.GetUInt8(5);
		si.UseHighlightColor = result.GetUInt8(6);
		si.ModelColor.Red = result.GetUInt8(7);
		si.ModelColor.Green = result.GetUInt8(8);
		si.ModelColor.Blue = result.GetUInt8(9);
		si.ModelHighlightColor.Red = result.GetUInt8(10);
		si.ModelHighlightColor.Green = result.GetUInt8(11);
		si.ModelHighlightColor.Blue = result.GetUInt8(12);

		s->NormalEquipment[class_id].push_back(si);
	}

	success = Select(&result, "SELECT * FROM character_create_equipment WHERE type = 'Level 90'");
	if (!success)
		return success;

	while (result.Next()) {
		uint8_t class_id = result.GetUInt8(2);

		OP_LoginReplyMsg_Packet::ClassItem::StartingItem si;
		si.SlotID = result.GetUInt8(3);
		si.ModelID = result.GetUInt32(4);
		si.UseColor = result.GetUInt8(5);
		si.UseHighlightColor = result.GetUInt8(6);
		si.ModelColor.Red = result.GetUInt8(7);
		si.ModelColor.Green = result.GetUInt8(8);
		si.ModelColor.Blue = result.GetUInt8(9);
		si.ModelHighlightColor.Red = result.GetUInt8(10);
		si.ModelHighlightColor.Green = result.GetUInt8(11);
		si.ModelHighlightColor.Blue = result.GetUInt8(12);

		s->HeroicEquipment[class_id].push_back(si);
	}

	success = Select(&result, "SELECT * FROM character_create_equipment WHERE type = 'Time Locked'");
	if (!success)
		return success;

	while (result.Next()) {
		uint8_t class_id = result.GetUInt8(2);

		OP_LoginReplyMsg_Packet::ClassItem::StartingItem si;
		si.SlotID = result.GetUInt8(3);
		si.ModelID = result.GetUInt32(4);
		si.UseColor = result.GetUInt8(5);
		si.UseHighlightColor = result.GetUInt8(6);
		si.ModelColor.Red = result.GetUInt8(7);
		si.ModelColor.Green = result.GetUInt8(8);
		si.ModelColor.Blue = result.GetUInt8(9);
		si.ModelHighlightColor.Red = result.GetUInt8(10);
		si.ModelHighlightColor.Green = result.GetUInt8(11);
		si.ModelHighlightColor.Blue = result.GetUInt8(12);

		s->TLEquipment[class_id].push_back(si);
	}

	return success;
}

uint32_t WorldDatabase::GetZoneIDForCharacter(uint32_t char_id) {
	DatabaseResult result;
	uint32_t ret = 0;

	if (Select(&result, "SELECT current_zone_id FROM characters WHERE id = %u", char_id)) {
		if (result.Next())
			ret = result.GetUInt32(0);
	}

	return ret;
}

bool WorldDatabase::LoadCharacterList(CharacterList& charList) {
	DatabaseResult result;

	if (!Select(&result, "SELECT id, account_id, name, race, class, tradeskill_class, level, tradeskill_level FROM characters;")) {
		return false;
	}

	while (result.Next()) {
		auto character = std::make_shared<Character>();

		int i = 0;

		CharacterBasicInfo& bi = character->basicInfo;
		bi.characterID = result.GetUInt32(i++);
		bi.accountID = result.GetUInt32(i++);
		bi.characterName = result.GetString(i++);
		bi.race = result.GetUInt8(i++);
		bi.advClass = result.GetUInt8(i++);
		bi.tradeskillClass = result.GetUInt8(i++);
		bi.advLevel = result.GetUInt8(i++);
		bi.tradeskillLevel = result.GetUInt8(i++);

		charList.AddCharacter(character);
	}

	return true;
}

bool WorldDatabase::LoadNextItemUniqueIDRange(std::pair<uint32_t,uint32_t>& out) {
	DatabaseResult result;

	static uint32_t rangeStart = 1;
	
	for (;;) {
		uint32_t rangeEnd = rangeStart + 9999;

		if (!Select(&result, "SELECT MAX(id) FROM character_items WHERE id BETWEEN %u AND %u", rangeStart, rangeEnd)) {
			LogError(LOG_DATABASE, 0, "Error loading the next item unique id!");
			return false;
		}

		result.Next();

		rangeStart += 10000;

		if (result.IsNull(0)) {
			out = std::make_pair(rangeStart - 10000, rangeEnd);
		}
		else {
			uint32_t next = result.GetUInt32(0) + 1;
			if (next == rangeStart) {
				//This range is completely full, check the next one
				continue;
			}

			out = std::make_pair(next, rangeEnd);
		}

		break;
	}

	return true;
}

bool WorldDatabase::SetServerStartTime() {
	bool ret = Query("INSERT INTO variables (variable_name, variable_value, comment) VALUES ('world_start_time', UTC_TIMESTAMP(), 'The time the server started') ON DUPLICATE KEY UPDATE variable_value=UTC_TIMESTAMP()");
	return ret;
}

bool WorldDatabase::SetServerLastUpdateTime() {
	bool ret = Query("INSERT INTO variables (variable_name, variable_value, comment) VALUES ('world_last_update_time', UTC_TIMESTAMP(), 'The time the server last updated') ON DUPLICATE KEY UPDATE variable_value=UTC_TIMESTAMP()");
	return ret;
}