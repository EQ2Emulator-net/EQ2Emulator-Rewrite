CREATE TABLE `spawn_camp_spawns` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`spawn_camp_id` INT(10) UNSIGNED NOT NULL,
	`spawn_id` INT(10) UNSIGNED NOT NULL,
	`respawn_timer` INT(10) UNSIGNED NOT NULL DEFAULT '3600',
	`expire_timer` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`expire_offset` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`min_level` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`max_level` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`min_encounter_level` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`max_encounter_level` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`condition` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0',
	`weight` INT(10) UNSIGNED NOT NULL DEFAULT '1',
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `FKSpawnCampSpawns` (`spawn_camp_id`) USING BTREE,
	INDEX `FKSpawn` (`spawn_id`) USING BTREE,
	CONSTRAINT `FKSpawn` FOREIGN KEY (`spawn_id`) REFERENCES `eq2_rewrite`.`spawn` (`id`) ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT `FKSpawnCampSpawns` FOREIGN KEY (`spawn_camp_id`) REFERENCES `eq2_rewrite`.`spawn_camp` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
)
COLLATE='latin1_general_ci'
ENGINE=InnoDB;