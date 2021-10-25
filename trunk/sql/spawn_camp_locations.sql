CREATE TABLE `spawn_camp_locations` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`spawn_camp_id` INT(10) UNSIGNED NOT NULL,
	`group_index` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`x` FLOAT NOT NULL DEFAULT '0',
	`y` FLOAT NOT NULL DEFAULT '0',
	`z` FLOAT NOT NULL DEFAULT '0',
	`x_offset` FLOAT NOT NULL DEFAULT '0',
	`y_offset` FLOAT NOT NULL DEFAULT '0',
	`z_offset` FLOAT NOT NULL DEFAULT '0',
	`heading` FLOAT NOT NULL DEFAULT '0',
	`pitch` FLOAT NOT NULL DEFAULT '0',
	`roll` FLOAT NOT NULL DEFAULT '0',
	`grid_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`visual_state_override` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`action_state_override` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `FKSpawnCamp` (`spawn_camp_id`) USING BTREE,
	CONSTRAINT `FKSpawnCamp` FOREIGN KEY (`spawn_camp_id`) REFERENCES `eq2_rewrite`.`spawn_camp` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
)
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB;