-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.5.9-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             11.2.0.6262
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping structure for table eq2parser.npc_appearance
CREATE TABLE IF NOT EXISTS `npc_appearance` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `signed_value` tinyint(4) NOT NULL DEFAULT 0,
  `type` varchar(32) COLLATE latin1_general_ci NOT NULL DEFAULT ' ',
  `red` smallint(6) NOT NULL DEFAULT 0,
  `green` smallint(6) NOT NULL DEFAULT 0,
  `blue` smallint(6) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `SpawnIDX` (`spawn_id`),
  KEY `TypeIDX` (`type`),
  CONSTRAINT `FK_npc_appearance` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=225709 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.npc_appearance_equip
CREATE TABLE IF NOT EXISTS `npc_appearance_equip` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `slot_id` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `equip_type` int(10) unsigned NOT NULL DEFAULT 0,
  `red` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `green` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `blue` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `highlight_red` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `highlight_green` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `highlight_blue` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`spawn_id`,`slot_id`),
  CONSTRAINT `FK_npc_equipment` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=11174 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn
CREATE TABLE IF NOT EXISTS `spawn` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(64) COLLATE latin1_general_ci DEFAULT NULL,
  `sub_title` varchar(250) COLLATE latin1_general_ci DEFAULT NULL,
  `prefix` varchar(128) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `suffix` varchar(128) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `last_name` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `model_type` mediumint(8) unsigned NOT NULL DEFAULT 0,
  `size` smallint(5) unsigned NOT NULL DEFAULT 32,
  `size_offset` tinyint(3) NOT NULL DEFAULT 0,
  `targetable` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `show_name` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `command_primary` int(10) unsigned NOT NULL DEFAULT 0,
  `command_secondary` int(10) unsigned NOT NULL DEFAULT 0,
  `visual_state` int(10) unsigned NOT NULL DEFAULT 0,
  `attackable` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `show_level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `show_command_icon` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `display_hand_icon` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `faction_id` int(10) unsigned NOT NULL DEFAULT 0,
  `collision_radius` smallint(5) unsigned NOT NULL DEFAULT 0,
  `hp` int(10) unsigned NOT NULL DEFAULT 0,
  `power` int(10) unsigned NOT NULL DEFAULT 0,
  `savagery` int(10) unsigned NOT NULL DEFAULT 0,
  `dissonance` int(10) unsigned NOT NULL DEFAULT 0,
  `merchant_id` int(10) unsigned NOT NULL DEFAULT 0,
  `transport_id` int(10) unsigned NOT NULL DEFAULT 0,
  `merchant_type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `processed` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `expansion_flag` int(10) NOT NULL DEFAULT 0,
  `is_instanced_spawn` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `disable_sounds` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `merchant_min_level` int(10) unsigned NOT NULL DEFAULT 0,
  `merchant_max_level` int(10) unsigned NOT NULL DEFAULT 0,
  `holiday_flag` int(10) unsigned NOT NULL DEFAULT 0,
  `aaxp_rewards` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `SpawnIDX` (`name`),
  KEY `SpawnCmdIDX` (`command_primary`,`command_secondary`),
  KEY `SpawnMiscIDX` (`faction_id`,`merchant_id`,`transport_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn_ground
CREATE TABLE IF NOT EXISTS `spawn_ground` (
  `spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `number_harvests` tinyint(3) unsigned NOT NULL DEFAULT 3,
  `num_attempts_per_harvest` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `groundspawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `collection_skill` enum('Unused','Mining','Gathering','Fishing','Trapping','Foresting','Collecting') COLLATE latin1_general_ci NOT NULL DEFAULT 'Unused',
  PRIMARY KEY (`spawn_id`) USING BTREE,
  KEY `GroundIDX` (`groundspawn_id`),
  CONSTRAINT `FK_groundspawn` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn_npcs
CREATE TABLE IF NOT EXISTS `spawn_npcs` (
  `spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `min_level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `max_level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `enc_level` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `class_` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `gender` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `min_group_size` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `max_group_size` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `hair_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `facial_hair_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `wing_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `chest_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `legs_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `soga_hair_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `soga_facial_hair_type_id` smallint(5) unsigned NOT NULL DEFAULT 0,
  `soga_model_type` smallint(5) unsigned NOT NULL DEFAULT 0,
  `hide_hood` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `heroic_flag` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `action_state` smallint(5) unsigned NOT NULL DEFAULT 0,
  `mood_state` smallint(5) unsigned NOT NULL DEFAULT 0,
  `emote_state` smallint(5) unsigned NOT NULL DEFAULT 0,
  `initial_state` smallint(5) unsigned NOT NULL DEFAULT 0,
  `activity_status` smallint(5) unsigned NOT NULL DEFAULT 0,
  `attack_type` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `aggro_radius` float unsigned NOT NULL DEFAULT 10,
  `ai_strategy` enum('BALANCED','OFFENSIVE','DEFENSIVE') COLLATE latin1_general_ci NOT NULL DEFAULT 'BALANCED',
  `spell_list_id` int(10) unsigned NOT NULL DEFAULT 0,
  `secondary_spell_list_id` int(10) unsigned NOT NULL DEFAULT 0,
  `skill_list_id` int(10) unsigned NOT NULL DEFAULT 0,
  `secondary_skill_list_id` int(10) unsigned NOT NULL DEFAULT 0,
  `equipment_list_id` int(10) unsigned NOT NULL DEFAULT 0,
  `str` smallint(5) NOT NULL DEFAULT 0,
  `sta` smallint(5) NOT NULL DEFAULT 0,
  `wis` smallint(5) NOT NULL DEFAULT 0,
  `intel` smallint(5) NOT NULL DEFAULT 0,
  `agi` smallint(5) NOT NULL DEFAULT 0,
  `heat` smallint(5) NOT NULL DEFAULT 0,
  `cold` smallint(5) NOT NULL DEFAULT 0,
  `magic` smallint(5) NOT NULL DEFAULT 0,
  `mental` smallint(5) NOT NULL DEFAULT 0,
  `divine` smallint(5) NOT NULL DEFAULT 0,
  `disease` smallint(5) NOT NULL DEFAULT 0,
  `poison` smallint(5) NOT NULL DEFAULT 0,
  `elemental` smallint(5) unsigned NOT NULL DEFAULT 0,
  `arcane` smallint(5) unsigned NOT NULL DEFAULT 0,
  `noxious` smallint(5) unsigned NOT NULL DEFAULT 0,
  `cast_percentage` tinyint(3) unsigned NOT NULL DEFAULT 10,
  `randomize` int(10) unsigned NOT NULL DEFAULT 0,
  `alignment` tinyint(3) NOT NULL DEFAULT 0,
  `water_type` tinyint(1) unsigned NOT NULL DEFAULT 0,
  `flying_type` tinyint(1) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`spawn_id`) USING BTREE,
  CONSTRAINT `FK_npcs_spawn` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn_objects
CREATE TABLE IF NOT EXISTS `spawn_objects` (
  `spawn_id` int(10) unsigned NOT NULL,
  `device_id` tinyint(3) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`spawn_id`) USING BTREE,
  CONSTRAINT `FK_objects_spawn` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn_signs
CREATE TABLE IF NOT EXISTS `spawn_signs` (
  `spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `type` enum('Zone','Generic') COLLATE latin1_general_ci NOT NULL DEFAULT 'Generic',
  `zone_id` int(10) unsigned NOT NULL DEFAULT 0,
  `widget_id` int(10) unsigned NOT NULL DEFAULT 0,
  `title` varchar(250) COLLATE latin1_general_ci DEFAULT NULL,
  `widget_x` float NOT NULL DEFAULT 0,
  `widget_y` float NOT NULL DEFAULT 0,
  `widget_z` float NOT NULL DEFAULT 0,
  `icon` smallint(6) NOT NULL DEFAULT 0,
  `description` text COLLATE latin1_general_ci NOT NULL,
  `sign_distance` float NOT NULL DEFAULT 0,
  `zone_x` float NOT NULL DEFAULT 0,
  `zone_y` float NOT NULL DEFAULT 0,
  `zone_z` float NOT NULL DEFAULT 0,
  `zone_heading` float NOT NULL DEFAULT 0,
  `include_heading` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `include_location` tinyint(3) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`spawn_id`) USING BTREE,
  CONSTRAINT `FK_signs_spawn` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.spawn_widgets
CREATE TABLE IF NOT EXISTS `spawn_widgets` (
  `spawn_id` int(10) unsigned NOT NULL,
  `widget_id` int(10) unsigned NOT NULL DEFAULT 0,
  `widget_x` float NOT NULL DEFAULT 0,
  `widget_y` float NOT NULL DEFAULT 0,
  `widget_z` float NOT NULL DEFAULT 0,
  `include_heading` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `include_location` tinyint(3) unsigned NOT NULL DEFAULT 1,
  `icon` tinyint(3) unsigned NOT NULL DEFAULT 4,
  `type` enum('Generic','Door','Lift') COLLATE latin1_general_ci NOT NULL DEFAULT 'Generic',
  `open_heading` float NOT NULL DEFAULT -1,
  `closed_heading` float NOT NULL DEFAULT -1,
  `open_x` float NOT NULL DEFAULT 0,
  `open_y` float NOT NULL DEFAULT 0,
  `open_z` float NOT NULL DEFAULT 0,
  `action_spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `open_sound_file` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '0',
  `close_sound_file` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '0',
  `open_duration` smallint(5) unsigned NOT NULL DEFAULT 10,
  `close_x` float NOT NULL DEFAULT 0,
  `close_y` float NOT NULL DEFAULT 0,
  `close_z` float NOT NULL DEFAULT 0,
  `linked_spawn_id` int(10) unsigned NOT NULL DEFAULT 0,
  `house_id` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`spawn_id`) USING BTREE,
  KEY `WidgetIDX` (`widget_id`),
  CONSTRAINT `FK_widgets_spawn` FOREIGN KEY (`spawn_id`) REFERENCES `spawn` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
