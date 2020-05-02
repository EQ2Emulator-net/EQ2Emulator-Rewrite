CREATE TABLE `lua_scripts` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`type` ENUM('Spawn','Spell','Quest','Zone','Item') NOT NULL COLLATE 'latin1_general_ci',
	`name` TEXT NOT NULL COLLATE 'latin1_general_ci',
	PRIMARY KEY (`id`),
	UNIQUE INDEX `UniqueNameIDX` (`name`(100))
)
COLLATE='latin1_general_ci'
ENGINE=InnoDB
;