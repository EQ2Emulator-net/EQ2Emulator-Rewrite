CREATE TABLE `spawn_camp` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`type` ENUM('Single','Area','Group','Boss','Event') NOT NULL DEFAULT 'Single' COLLATE 'latin1_general_ci',
	`name` CHAR(50) NOT NULL DEFAULT '' COLLATE 'latin1_general_ci',
	`zone_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`x` FLOAT NOT NULL DEFAULT '0',
	`y` FLOAT NOT NULL DEFAULT '0',
	`z` FLOAT NOT NULL DEFAULT '0',
	`grid` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`radius` FLOAT NOT NULL DEFAULT '0',
	`min_spawns` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
	`max_spawns` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `FK_spawn_camp_zones` (`zone_id`) USING BTREE,
	CONSTRAINT `FK_spawn_camp_zones` FOREIGN KEY (`zone_id`) REFERENCES `eq2_rewrite`.`zones` (`id`) ON UPDATE RESTRICT ON DELETE RESTRICT
)
COLLATE='latin1_general_ci'
ENGINE=InnoDB;
