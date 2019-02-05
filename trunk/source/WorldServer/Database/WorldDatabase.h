#pragma once

#include "../../common/database.h"
#include "../WorldServer/Client.h"

class OP_AllCharactersDescReplyMsg_Packet;

class WorldDatabase : public Database {
public:
	WorldDatabase();
	~WorldDatabase();

	bool Start();
	void Stop();

	bool LoadOpcodes();

	bool GetAccount(Client* client, std::string user, std::string pass);
	bool UpdateAccountIPAddress(uint32_t account, uint32_t address);
	bool UpdateAccountClientVersion(uint32_t account, uint16_t version);

	bool LoadCharacters(uint32_t account, OP_AllCharactersDescReplyMsg_Packet* packet);

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

*/