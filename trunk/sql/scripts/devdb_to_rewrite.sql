/*Verify spawn_scripts data and add FKs*/;
DELETE FROM spawn_scripts WHERE spawn_location_id > 0 AND spawn_location_id NOT IN (select id from spawn_location_name);
DELETE FROM spawn_scripts where spawn_id > 0 AND spawn_id NOT IN (select id from spawn);
DELETE FROM spawn_scripts WHERE spawnentry_id > 0 AND spawnentry_id NOT IN (select id from spawn_location_entry);
ALTER TABLE `spawn_scripts`
	ADD INDEX `SpawnIDX` (`spawn_id`),
	ADD INDEX `SpawnEntryIDX` (`spawnentry_id`),
	ADD INDEX `SpawnLocationIDX` (`spawn_location_id`),
	CHANGE COLUMN `spawn_id` `spawn_id` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `id`,
	CHANGE COLUMN `spawnentry_id` `spawnentry_id` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `spawn_id`,
	CHANGE COLUMN `spawn_location_id` `spawn_location_id` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `spawnentry_id`;
UPDATE spawn_scripts SET spawn_location_id = NULL WHERE spawn_location_id = 0;
UPDATE spawn_scripts SET spawnentry_id = NULL WHERE spawnentry_id = 0;
UPDATE spawn_scripts SET spawn_id = NULL WHERE spawn_id = 0;
ALTER TABLE `spawn_scripts`
	ADD CONSTRAINT `FK_SpawnScripts_SpawnID` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`),
	ADD CONSTRAINT `FK_SpawnScripts_SpawnEntry` FOREIGN KEY (`spawnentry_id`) REFERENCES `spawn_location_entry` (`id`),
	ADD CONSTRAINT `FK_SpawnScripts_SpawnLocation` FOREIGN KEY (`spawn_location_id`) REFERENCES `spawn_location_name` (`id`);

/*Insert data into the lua_scripts table*/;
INSERT IGNORE INTO lua_scripts (`type`, `name`) SELECT DISTINCT 'Spawn', lua_script FROM spawn_scripts;
INSERT IGNORE INTO lua_scripts (`type`, `name`) SELECT DISTINCT 'Item', lua_script FROM items WHERE LENGTH(lua_script) > 0;
INSERT IGNORE INTO lua_scripts (`type`, `name`) SELECT DISTINCT 'Zone', lua_script FROM zones WHERE LENGTH(lua_script) > 0;
INSERT IGNORE INTO lua_scripts (`type`, `name`) SELECT DISTINCT 'Spell', lua_script FROM spells WHERE LENGTH(lua_script) > 0;
INSERT IGNORE INTO lua_scripts (`type`, `name`) SELECT DISTINCT 'Quest', lua_script FROM quests WHERE LENGTH(lua_script) > 0;

/*Convert lua_script entries in various tables to reference the lua_scripts table*/;

/*Spawns*/;
ALTER TABLE `spawn_scripts`
	ADD COLUMN `script_id` INT(10) UNSIGNED NULL AFTER `lua_script`,
	ADD CONSTRAINT `FK_SpawnScripts_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
UPDATE spawn_scripts ss
	INNER JOIN lua_scripts ls ON ss.lua_script = ls.name
	SET ss.script_id = ls.id
	WHERE 1;

ALTER TABLE spawn_scripts 
	DROP COLUMN lua_script,
	CHANGE COLUMN `script_id` `script_id` INT(10) UNSIGNED NOT NULL AFTER `spawn_location_id`;

/*This is to deal with dupe entries that existed*/;
UPDATE spawn_scripts SET spawn_id = NULL WHERE spawnentry_id OR spawn_location_id;
UPDATE spawn_scripts SET spawn_location_id = NULL WHERE spawnentry_id;
DELETE FROM spawn_scripts WHERE spawn_id = 2490682 AND id = 74330;
DELETE FROM spawn_scripts WHERE spawn_id = 4120016 AND id = 141530;
DELETE FROM spawn_scripts WHERE spawnentry_id = 1001535 AND id = 74256;
DELETE FROM spawn_scripts WHERE spawn_location_id = 1572706 AND id = 73850;
DELETE FROM spawn_scripts WHERE spawn_location_id = 1572511 AND id = 73736;
DELETE FROM spawn_scripts WHERE spawn_location_id = 360299 AND id = 106843;
DELETE FROM spawn_scripts WHERE spawn_location_id = 298437 AND id = 140852;

ALTER TABLE `spawn_location_name`
	ADD COLUMN `script_id` INT UNSIGNED NULL DEFAULT NULL AFTER `name`,
	ADD CONSTRAINT `FK_SpawnLocationName_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
ALTER TABLE `spawn_location_entry`
	ADD COLUMN `script_id` INT(10) UNSIGNED NULL DEFAULT NULL AFTER `condition`,
	ADD CONSTRAINT `FK_SpawnEntry_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
ALTER TABLE `spawn`
	ADD COLUMN `script_id` INT UNSIGNED NULL DEFAULT NULL AFTER `merchant_type`,
	ADD CONSTRAINT `FK_Spawn_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
UPDATE spawn s
	INNER JOIN spawn_scripts ss ON ss.spawn_id = s.id
	SET s.script_id = ss.script_id
	WHERE 1;
	
UPDATE spawn_location_name sln
	INNER JOIN spawn_scripts ss ON ss.spawn_location_id = sln.id
	SET sln.script_id = ss.script_id
	WHERE 1;
	
UPDATE spawn_location_entry sle
	INNER JOIN spawn_scripts ss ON ss.spawnentry_id = sle.id
	SET sle.script_id = ss.script_id
	WHERE 1;

DROP TABLE spawn_scripts;

/*Items*/;
ALTER TABLE `items`
	ADD COLUMN `script_id` INT(10) UNSIGNED NULL AFTER `lua_script`,
	ADD CONSTRAINT `FK_Item_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
UPDATE items i
	INNER JOIN lua_scripts ls ON LENGTH(i.lua_script) > 0 AND i.lua_script = ls.name
	SET i.script_id = ls.id
	WHERE 1;
	
ALTER TABLE items
	DROP COLUMN lua_script;

/*Spells*/;
ALTER TABLE `spells`
	ADD COLUMN `script_id` INT UNSIGNED NULL DEFAULT NULL AFTER `lua_script`,
	ADD CONSTRAINT `FK_Spells_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
UPDATE spells s
	INNER JOIN lua_scripts ls ON LENGTH(s.lua_script) > 0 AND s.lua_script = ls.name
	SET s.script_id = ls.id
	WHERE 1;
	
ALTER TABLE spells
	DROP COLUMN lua_script;
	
/*Quests*/;
ALTER TABLE `quests`
	ADD COLUMN `script_id` INT UNSIGNED NULL DEFAULT NULL AFTER `lua_script`,
	ADD CONSTRAINT `FK_Quests_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);

UPDATE quests q
	INNER JOIN lua_scripts ls ON LENGTH(q.lua_script) > 0 AND q.lua_script = ls.name
	SET q.script_id = ls.id
	WHERE 1;

ALTER TABLE quests
	DROP COLUMN lua_script;
	
/*Zones*/;
ALTER TABLE `zones`
	ADD COLUMN `script_id` INT UNSIGNED NULL DEFAULT NULL AFTER `lua_script`,
	ADD CONSTRAINT `FK_Zones_ScriptID` FOREIGN KEY (`script_id`) REFERENCES `lua_scripts` (`id`);
	
UPDATE zones z
	INNER JOIN lua_scripts ls ON LENGTH(z.lua_script) > 0 AND z.lua_script = ls.name
	SET z.script_id = ls.id
	WHERE 1;
	
ALTER TABLE zones
	DROP COLUMN lua_script;
	
	
	
/*Now convert values in the spawn table*/;
ALTER TABLE `spawn`
	CHANGE COLUMN `size` `size` FLOAT NOT NULL DEFAULT '1.0' AFTER `model_type`,
	CHANGE COLUMN `size_offset` `size_offset` FLOAT NOT NULL DEFAULT '0' AFTER `size`,
	CHANGE COLUMN `collision_radius` `collision_radius` FLOAT NOT NULL DEFAULT '0' AFTER `faction_id`;

UPDATE spawn SET `size` = `size` / 32.0, 
	size_offset = size_offset / 32.0,
 	collision_radius = collision_radius / 32.0
	WHERE 1;