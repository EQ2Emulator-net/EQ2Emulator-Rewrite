#pragma once

#include "../../common/database.h"
#include "../WorldServer/Client.h"

class OP_AllCharactersDescReplyMsg_Packet;
class OP_CreateCharacterRequestMsg_Packet;
class WorldServer;
struct EQ2ColorFloat;

class WorldDatabase : public Database {
public:
	WorldDatabase();
	~WorldDatabase();

	bool Start();
	void Stop();

	bool LoadOpcodes();

	bool GetAccount(Client* client, std::string user, std::string pass);
	bool UpdateAccountIPAddress(uint32_t account, uint32_t address);
	bool UpdateAccountClientVersion(uint32_t account, uint32_t version);

	bool LoadCharacters(uint32_t account, OP_AllCharactersDescReplyMsg_Packet* packet, uint8_t max_adv_level, uint8_t max_ts_level);

	bool DeleteCharacter(uint32_t account_id, uint32_t char_id, std::string name);

	bool SaveClientLog(std::string type, char* message, uint32_t version);
	uint32_t CreateCharacter(uint32_t account_id, OP_CreateCharacterRequestMsg_Packet* packet);
	uint16_t GetAppearanceID(std::string name);
	void UpdateStartingFactions(uint32_t char_id, uint8_t choice);
	void UpdateStartingZone(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint8_t choice);
	void UpdateStartingItems(uint32_t char_id, uint8_t class_id, uint8_t race_id, bool base_class = false);
	void UpdateStartingSkills(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingSpells(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingSkillbar(uint32_t char_id, uint8_t class_id, uint8_t race_id);
	void UpdateStartingTitles(uint32_t char_id, uint8_t class_id, uint8_t race_id, uint8_t gender_id);
	bool InsertCharacterStats(uint32_t character_id, uint8_t class_id, uint8_t race_id);
	void SaveCharacterColors(uint32_t char_id, const char* type, EQ2ColorFloat color);
	void SaveCharacterFloats(uint32_t char_id, const char* type, float float1, float float2, float float3);
	uint8_t CheckNameFilter(const char* name);

	bool LoadServerVariables(WorldServer* s);
	bool LoadServerConfig(WorldServer* s);

	uint32_t GetZoneIDForCharacter(uint32_t char_id);

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