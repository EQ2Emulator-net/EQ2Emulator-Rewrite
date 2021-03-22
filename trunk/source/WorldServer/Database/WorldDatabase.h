#pragma once

#include "../../common/CommonDatabase.h"
#include "../WorldServer/Client.h"
#include <array>

class OP_AllCharactersDescReplyMsg_Packet;
class OP_CreateCharacterRequestMsg_Packet;
class WorldServer;
struct EQ2ColorFloat;

class WorldDatabase : public CommonDatabase {
public:
	WorldDatabase();
	~WorldDatabase();

	bool Start();

	bool GetAccount(Client* client, std::string user, std::string pass);
	bool UpdateAccountIPAddress(uint32_t account, uint32_t address);
	bool UpdateAccountClientVersion(uint32_t account, uint32_t version);

	bool LoadCharacterAppearances(uint32_t account, OP_AllCharactersDescReplyMsg_Packet* packet, uint8_t max_adv_level, uint8_t max_ts_level);
	bool LoadCharacterList(class CharacterList& charList);

	bool DeleteCharacter(uint32_t account_id, uint32_t char_id, std::string name);

	bool SaveClientLog(const std::string& type, const std::string& message, uint32_t version);
	uint32_t CreateCharacter(uint32_t account_id, OP_CreateCharacterRequestMsg_Packet* packet);
	std::array<uint32_t, 13> GetCreateAppearanceIDs(const class Substruct_CharacterCustomization& assets, const class Substruct_CharacterCustomization& sogaAssets);
	void UpdateStartingFactions(uint32_t char_id, uint16_t choice);
	void UpdateStartingZone(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint16_t choice);
	void UpdateStartingItems(uint32_t char_id, uint8_t class_id, uint8_t race_id, bool base_class = false);
	void UpdateStartingSkills(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingSpells(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingSkillbar(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingTitles(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint8_t gender_id);
	bool InsertCharacterStats(uint32_t character_id, uint8_t class_id, uint8_t race_id);
	void SaveCharacterColors(uint32_t char_id, const char* type, EQ2ColorFloat color);
	void SaveCharacterColors(uint32_t char_id, const char* type, const int8_t* color);
	void SaveCharacterFloats(uint32_t char_id, const char* type, float float1, float float2, float float3);
	uint8_t CheckNameFilter(const char* name);

	bool LoadServerVariables(WorldServer* s);
	bool LoadServerConfig(WorldServer* s);

	uint32_t GetZoneIDForCharacter(uint32_t char_id);
	bool LoadNextItemUniqueIDRange(std::pair<uint32_t,uint32_t>& out);

	bool SetServerStartTime();
	bool SetServerLastUpdateTime();

private:

};

/*
CREATE TABLE `account` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(64) NOT NULL DEFAULT '',
	`passwd` VARCHAR(64) NOT NULL DEFAULT '',
	`ip_address` VARCHAR(50) NOT NULL DEFAULT '0',
	`email_address` VARCHAR(50) NOT NULL DEFAULT 'Unknown',
	`last_client_version` VARCHAR(50) NOT NULL DEFAULT 'Unknown',
	`flag` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
	PRIMARY KEY (`id`),
	UNIQUE INDEX `Name` (`name`)
)
COLLATE='latin1_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `log_messages` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`log_time` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`log_type` VARCHAR(64) NOT NULL,
	`message` TEXT NOT NULL,
	`account` VARCHAR(64) NOT NULL DEFAULT '',
	`client_data_version` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`log_entry_archived` TINYINT(1) UNSIGNED NOT NULL DEFAULT '0',
	PRIMARY KEY (`id`)
)
COLLATE='latin1_general_ci'
ENGINE=InnoDB
;
*/