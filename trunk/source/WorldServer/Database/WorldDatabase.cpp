#include "stdafx.h"

#include "WorldDatabase.h"
#include "../../common/log.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../Packets/OP_AllCharactersDescReplyMsg_Packet.h"

#include "../Packets/OP_AllCharactersDescRequestMsg_Packet.h"
#include "../Packets/OP_AllWSDescRequestMsg_Packet.h"

#ifdef _WIN32
	#include <WS2tcpip.h>
#else
	#include <arpa/inet.h>
#endif

static void DatabaseQueryError(Database *db) {
	LogError(LOG_DATABASE, 0, "Error running MySQL query: %s", db->GetError());
}

WorldDatabase::WorldDatabase() {

}

WorldDatabase::~WorldDatabase() {
	Disconnect();
}

bool WorldDatabase::Start() {
	DatabaseCallbacks callbacks;
	callbacks.query_error = DatabaseQueryError;
	SetCallbacks(&callbacks);

	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	LogError(LOG_DATABASE, 0, "Error connecting to MySQL: %s", GetError());
	return false;
}

void WorldDatabase::Stop() {
	Disconnect();
}

bool WorldDatabase::LoadOpcodes() {
	DatabaseResult result;
	bool success;
	uint32_t count = 0;

	success = Select(&result, "SELECT `name`, `version_range1`, `version_range2`, `opcode` FROM `opcodes`");
	if (!success)
		return false;

	while (result.Next()) {
		OpcodeManager::GetGlobal()->RegisterVersionOpcode(result.GetString(0), result.GetUInt16(1), result.GetUInt16(2), result.GetUInt16(3));
		count++;
	}

	LogDebug(LOG_DATABASE, 0, "Loaded %u opcodes", count);
	return true;
}

bool WorldDatabase::GetAccount(Client* client, std::string user, std::string pass) {
	DatabaseResult result;
	bool success;

	char* esc_user = Escape(user.c_str());
	char* esc_pass = Escape(pass.c_str());

	success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s' AND passwd = md5('%s')", esc_user, esc_pass);
	if (success) {
		success = result.Next();
		if (success) {
			uint32_t id = result.GetUInt32(0);
			client->SetAccount(id);
			UpdateAccountIPAddress(id, client->GetIP());
			UpdateAccountClientVersion(id, client->GetVersion());
		}
		// if user and pass check failed
		else {
			// if auto account creation is enabled
			if (true) {
				// see if there is already an account with this username
				success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s'", esc_user);
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
					success = Query("INSERT INTO account(`name`, `passwd`, `ip_address`, `last_client_version`) VALUES ('%s', md5('%s'), '%s', %u)", esc_user, esc_pass, inet_ntoa(ip_addr), client->GetVersion());
					if (success) 
						client->SetAccount(LastInsertID());
				}
					
			}
			// No auto account so return false
			else {
				success = false;
			}
		}
	}

	free(esc_user);
	free(esc_pass);
	return success;
}

bool WorldDatabase::UpdateAccountIPAddress(uint32_t account, uint32_t address) {
	in_addr ip_addr;
	ip_addr.s_addr = address;
	return Query("UPDATE `account` SET `ip_address` = '%s' WHERE `id` = %u", inet_ntoa(ip_addr), account);
}

bool WorldDatabase::UpdateAccountClientVersion(uint32_t account, uint16_t version) {
	return Query("UPDATE `account` SET `last_client_version` = %u WHERE `id` = %u", version, account);
}

bool WorldDatabase::LoadCharacters(uint32_t account, OP_AllCharactersDescReplyMsg_Packet* packet) {
	bool ret;
	DatabaseResult result;
	DatabaseResult result2;

	ret = Select(&result, "SELECT id, name, race, class, gender, deity, body_size, body_age, current_zone_id, level, tradeskill_class, tradeskill_level, soga_wing_type, soga_chest_type, soga_legs_type, soga_hair_type, soga_facial_hair_type, soga_model_type, legs_type, chest_type, wing_type, hair_type, facial_hair_type, model_type, unix_timestamp(created_date), unix_timestamp(last_played) FROM characters WHERE account_id = %u AND deleted = 0", account);
	if (!ret)
		return ret;

	while (result.Next()) {
		OP_AllCharactersDescReplyMsg_Packet::CharacterListEntry c;
		c.account_id = account;
		c.server_id = 1;

		if (packet->GetVersion() >= 887)
			c.version = 6;
		else
			c.version = 5;

		c.charid = result.GetUInt32(0);
		c.name = result.GetString(1);
		c.race = result.GetUInt8(2);
		c._class = result.GetUInt8(3);
		c.gender = result.GetUInt8(4);
		//c.deity = result.GetUInt8(5);
		c.body_size = result.GetUInt8(6);
		//c.body_age = result.GetUInt8(7);
		uint32_t zone_id = result.GetUInt32(8);
		ret = Select(&result2, "SELECT name, description FROM zones WHERE id = %u", zone_id);
		if (!ret)
			return ret;
		if (result2.Next()) {
			c.zone = result2.IsNull(0) ? " " : result2.GetString(0);
			c.zonedesc = result2.IsNull(1) ? " " : result2.GetString(1);
			c.zonename2 = " ";
		}
		c.level = result.GetUInt32(9);
		c.tradeskill_class = result.GetUInt8(10);
		c.tradeskill_level = result.GetUInt32(11);

		/* SOGA Appearances */
		//c.soga_wing_type = result.GetUInt16(12);
		//c.soga_cheek_type = result.GetUInt16(13);
		//c.soga_legs_type = result.GetUInt16(14);
		c.soga_hair_type = result.GetUInt16(15);
		c.soga_hair_face_type = result.GetUInt16(16);
		c.soga_race_type = result.GetUInt16(17);

		/* NORMAL Appearances */
		c.legs_type = result.GetUInt16(18);
		c.chest_type = result.GetUInt16(19);
		c.wing_type = result.GetUInt16(20);
		c.hair_type = result.GetUInt16(21);
		c.hair_face_type = result.GetUInt16(22);
		c.race_type = result.GetUInt16(23);

		if (!result.IsNull(24))
			c.created_date = result.GetUInt32(24);
		if (!result.IsNull(25))
			c.last_played = result.GetUInt32(25);


		// TODO char_colors table
		ret = Select(&result2, "SELECT type, red, green, blue FROM char_colors WHERE char_id = %u", c.charid);
		if (!ret)
			return ret;

		while (result2.Next()) {
			std::string type = result2.GetString(0);

			if (type == "skin_color") {
				c.skin_color.Red = result2.GetUInt8(1);
				c.skin_color.Green = result2.GetUInt8(2);
				c.skin_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "eye_color") {
				c.eye_color.Red = result2.GetUInt8(1);
				c.eye_color.Green = result2.GetUInt8(2);
				c.eye_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_color1") {
				c.hair_color1.Red = result2.GetUInt8(1);
				c.hair_color1.Green = result2.GetUInt8(2);
				c.hair_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_color2") {
				c.hair_color2.Red = result2.GetUInt8(1);
				c.hair_color2.Green = result2.GetUInt8(2);
				c.hair_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_highlight") {
				// ??? - not in the structs?
				//c.hair_highlight_color.Red = result2.GetUInt8(1);
				//c.hair_highlight_color.Green = result2.GetUInt8(2);
				//c.hair_highlight_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_type_color") {
				c.hair_type_color.Red = result2.GetUInt8(1);
				c.hair_type_color.Green = result2.GetUInt8(2);
				c.hair_type_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_type_highlight_color") {
				c.hair_type_highlight_color.Red = result2.GetUInt8(1);
				c.hair_type_highlight_color.Green = result2.GetUInt8(2);
				c.hair_type_highlight_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_face_color") {
				c.hair_face_color.Red = result2.GetUInt8(1);
				c.hair_face_color.Green = result2.GetUInt8(2);
				c.hair_face_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "hair_face_highlight_color") {
				c.hair_face_highlight_color.Red = result2.GetUInt8(1);
				c.hair_face_highlight_color.Green = result2.GetUInt8(2);
				c.hair_face_highlight_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "wing_color1") {
				c.wing_color1.Red = result2.GetUInt8(1);
				c.wing_color1.Green = result2.GetUInt8(2);
				c.wing_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "wing_color2") {
				c.wing_color2.Red = result2.GetUInt8(1);
				c.wing_color2.Green = result2.GetUInt8(2);
				c.wing_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "shirt_color") {
				c.shirt_color.Red = result2.GetUInt8(1);
				c.shirt_color.Green = result2.GetUInt8(2);
				c.shirt_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown_chest_color") {
				c.unknown_chest_color.Red = result2.GetUInt8(1);
				c.unknown_chest_color.Green = result2.GetUInt8(2);
				c.unknown_chest_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "pants_color") {
				c.pants_color.Red = result2.GetUInt8(1);
				c.pants_color.Green = result2.GetUInt8(2);
				c.pants_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown_legs_color") {
				c.unknown_legs_color.Red = result2.GetUInt8(1);
				c.unknown_legs_color.Green = result2.GetUInt8(2);
				c.unknown_legs_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "unknown9") {
				c.unknown9.Red = result2.GetUInt8(1);
				c.unknown9.Green = result2.GetUInt8(2);
				c.unknown9.Blue = result2.GetUInt8(3);
			}
			else if (type == "eye_type") {
				c.eye_type[0] = result2.GetInt8(1);
				c.eye_type[1] = result2.GetInt8(2);
				c.eye_type[2] = result2.GetInt8(3);
			}
			else if (type == "ear_type") {
				c.ear_type[0] = result2.GetInt8(1);
				c.ear_type[1] = result2.GetInt8(2);
				c.ear_type[2] = result2.GetInt8(3);
			}
			else if (type == "eye_brow_type") {
				c.eye_brow_type[0] = result2.GetInt8(1);
				c.eye_brow_type[1] = result2.GetInt8(2);
				c.eye_brow_type[2] = result2.GetInt8(3);
			}
			else if (type == "cheek_type") {
				c.cheek_type[0] = result2.GetInt8(1);
				c.cheek_type[1] = result2.GetInt8(2);
				c.cheek_type[2] = result2.GetInt8(3);
			}
			else if (type == "lip_type") {
				c.lip_type[0] = result2.GetInt8(1);
				c.lip_type[1] = result2.GetInt8(2);
				c.lip_type[2] = result2.GetInt8(3);
			}
			else if (type == "chin_type") {
				c.chin_type[0] = result2.GetInt8(1);
				c.chin_type[1] = result2.GetInt8(2);
				c.chin_type[2] = result2.GetInt8(3);
			}
			else if (type == "nose_type") {
				c.nose_type[0] = result2.GetInt8(1);
				c.nose_type[1] = result2.GetInt8(2);
				c.nose_type[2] = result2.GetInt8(3);
			}
			else if (type == "body_size") {
				c.body_size = result2.GetInt8(1);
			}
			else if (type == "soga_skin_color") {
				c.soga_skin_color.Red = result2.GetUInt8(1);
				c.soga_skin_color.Green = result2.GetUInt8(2);
				c.soga_skin_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_eye_color") {
				c.soga_eye_color.Red = result2.GetUInt8(1);
				c.soga_eye_color.Green = result2.GetUInt8(2);
				c.soga_eye_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_color1") {
				c.soga_hair_color1.Red = result2.GetUInt8(1);
				c.soga_hair_color1.Green = result2.GetUInt8(2);
				c.soga_hair_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_color2") {
				c.soga_hair_color2.Red = result2.GetUInt8(1);
				c.soga_hair_color2.Green = result2.GetUInt8(2);
				c.soga_hair_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_highlight") {
				// ??? - not in the struct?
				//c.soga_hair_highlight.Red = result2.GetUInt8(1);
				//c.soga_hair_highlight.Green = result2.GetUInt8(2);
				//c.soga_hair_highlight.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_type_color") {
				c.soga_hair_type_color.Red = result2.GetUInt8(1);
				c.soga_hair_type_color.Green = result2.GetUInt8(2);
				c.soga_hair_type_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_type_highlight_color") {
				c.soga_hair_type_highlight_color.Red = result2.GetUInt8(1);
				c.soga_hair_type_highlight_color.Green = result2.GetUInt8(2);
				c.soga_hair_type_highlight_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_face_color") {
				c.soga_hair_face_color.Red = result2.GetUInt8(1);
				c.soga_hair_face_color.Green = result2.GetUInt8(2);
				c.soga_hair_face_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_hair_face_highlight_color") {
				c.soga_hair_face_highlight_color.Red = result2.GetUInt8(1);
				c.soga_hair_face_highlight_color.Green = result2.GetUInt8(2);
				c.soga_hair_face_highlight_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_wing_color1") {
				c.wing_color1.Red = result2.GetUInt8(1);
				c.wing_color1.Green = result2.GetUInt8(2);
				c.wing_color1.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_wing_color2") {
				c.wing_color2.Red = result2.GetUInt8(1);
				c.wing_color2.Green = result2.GetUInt8(2);
				c.wing_color2.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_shirt_color") {
				// ??? - not in the struct?
				//c.soga_shirt_color.Red = result2.GetUInt8(1);
				//c.soga_shirt_color.Green = result2.GetUInt8(2);
				//c.soga_shirt_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_unknown_chest_color") {
				// ??? - not in the struct?
				//c.soga_unknown_chest_color.Red = result2.GetUInt8(1);
				//c.soga_unknown_chest_color.Green = result2.GetUInt8(2);
				//c.soga_unknown_chest_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_pants_color") {
				// ??? - not in the struct?
				//c.soga_pants_color.Red = result2.GetUInt8(1);
				//c.soga_pants_color.Green = result2.GetUInt8(2);
				//c.soga_pants_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_unknown_legs_color") {
				// ??? - not in the struct?
				//c.soga_unknown_legs_color.Red = result2.GetUInt8(1);
				//c.soga_unknown_legs_color.Green = result2.GetUInt8(2);
				//c.soga_unknown_legs_color.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_unknown13") {
				// ??? - not in the struct?
				//c.soga_unknown13.Red = result2.GetUInt8(1);
				//c.soga_unknown13.Green = result2.GetUInt8(2);
				//c.soga_unknown13.Blue = result2.GetUInt8(3);
			}
			else if (type == "soga_eye_type") {
				c.soga_eye_type[0] = result2.GetInt8(1);
				c.soga_eye_type[1] = result2.GetInt8(2);
				c.soga_eye_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_ear_type") {
				c.soga_ear_type[0] = result2.GetInt8(1);
				c.soga_ear_type[1] = result2.GetInt8(2);
				c.soga_ear_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_eye_brow_type") {
				c.soga_eye_brow_type[0] = result2.GetInt8(1);
				c.soga_eye_brow_type[1] = result2.GetInt8(2);
				c.soga_eye_brow_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_cheek_type") {
				c.soga_cheek_type[0] = result2.GetInt8(1);
				c.soga_cheek_type[1] = result2.GetInt8(2);
				c.soga_cheek_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_lip_type") {
				c.soga_lip_type[0] = result2.GetInt8(1);
				c.soga_lip_type[1] = result2.GetInt8(2);
				c.soga_lip_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_chin_type") {
				c.soga_chin_type[0] = result2.GetInt8(1);
				c.soga_chin_type[1] = result2.GetInt8(2);
				c.soga_chin_type[2] = result2.GetInt8(3);
			}
			else if (type == "soga_nose_type") {
				c.soga_nose_type[0] = result2.GetInt8(1);
				c.soga_nose_type[1] = result2.GetInt8(2);
				c.soga_nose_type[2] = result2.GetInt8(3);
			}
		}

		// TODO equipment
		ret = Select(&result2, "SELECT ci.slot, ia.equip_type, ia.red, ia.green, ia.blue, ia.highlight_red, ia.highlight_green, ia.highlight_blue FROM character_items ci INNER JOIN item_appearances ia ON ci.item_id = ia.item_id WHERE ci.type = 'EQUIPPED' AND ci.char_id = %u ORDER BY ci.slot ASC", c.charid);
		if (!ret)
			return ret;

		while (result2.Next()) {
			uint8_t slot = result2.GetUInt8(0);
			c.equip[slot].type = result2.GetUInt16(1);
			c.equip[slot].color.Red = result2.GetUInt8(2);
			c.equip[slot].color.Green = result2.GetUInt8(3);
			c.equip[slot].color.Blue = result2.GetUInt8(4);
			c.equip[slot].highlight.Red = result2.GetUInt8(5);
			c.equip[slot].highlight.Green = result2.GetUInt8(6);
			c.equip[slot].highlight.Blue = result2.GetUInt8(7);
		}
			
		c.server_name = "Rewrite Test Server";
		
		packet->CharacterList.push_back(c);
	}

	packet->NumCharacters = (uint8_t)packet->CharacterList.size();

	return ret;
}