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

	ret = Select(&result, "SELECT id, name, race, class, gender, deity, body_size, body_age, current_zone_id, level, tradeskill_class, tradeskill_level, soga_wing_type, soga_chest_type, soga_legs_type, soga_hair_type, soga_facial_hair_type, soga_model_type, legs_type, chest_type, wing_type, hair_type, facial_hair_type, model_type, unix_timestamp(created_date), unix_timestamp(last_played)   FROM characters WHERE account_id = %u AND deleted = 0", account);
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
		//c.tradeskill_class = result.GetUInt8(9);
		//c.tradeskill_level = result.GetUInt8(10);

		/* SOGA Appearances */
		//c.soga_wing_type = result.GetUInt16(11);
		//c.soga_cheek_type = result.GetUInt16(12);
		//c.soga_legs_type = result.GetUInt16(13);
		c.soga_hair_type = result.GetUInt16(14);
		c.soga_hair_face_type = result.GetUInt16(15);
		c.soga_race_type = result.GetUInt16(16);

		/* NORMAL Appearances */
		c.legs_type = result.GetUInt16(17);
		c.chest_type = result.GetUInt16(18);
		c.wing_type = result.GetUInt16(19);
		c.hair_type = result.GetUInt16(20);
		c.hair_face_type = result.GetUInt16(21);
		c.race_type = result.GetUInt16(22);

		if (!result.IsNull(23))
			c.created_date = result.GetUInt32(23);
		if (!result.IsNull(24))
			c.last_played = result.GetUInt32(24);


		// TODO char_colors table

		// TODO equipment

		c.server_name = "Rewrite Test Server";
		
		packet->CharacterList.push_back(c);
	}

	packet->NumCharacters = (uint8_t)packet->CharacterList.size();

	return ret;
}