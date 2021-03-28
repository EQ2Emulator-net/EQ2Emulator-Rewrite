-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.6.51-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             11.2.0.6213
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping structure for table eq2parser.items
DROP TABLE IF EXISTS `items`;
CREATE TABLE IF NOT EXISTS `items` (
  `id` int(10) unsigned NOT NULL,
  `bPvpDesc` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `log_id` int(10) unsigned NOT NULL,
  `name` varchar(100) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `item_type` enum('Normal','Weapon','Ranged','Armor','Shield','Bag','Scroll','Recipe','Food','Bauble','House','Thrown','House Container','Adornment','Profile','Pattern Set','Item Set','Book','Decoration','Dungeon Maker','Marketplace','Reward Crate2','Infuser1','Infuser2','Experience Vial','Overseer') COLLATE latin1_general_ci NOT NULL DEFAULT 'Normal',
  `icon` smallint(5) unsigned NOT NULL DEFAULT '0',
  `count` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tier` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `skill_id_req` int(10) unsigned NOT NULL DEFAULT '0',
  `skill_id_req2` int(10) unsigned NOT NULL DEFAULT '0',
  `skill_min` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weight` int(10) unsigned NOT NULL DEFAULT '0',
  `description` text COLLATE latin1_general_ci,
  `show_name` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `attuneable` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `artifact` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `lore` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `temporary` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `notrade` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `novalue` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `nozone` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `nodestroy` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `crafted` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `good_only` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `evil_only` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `stacklore` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `lore_equip` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `no_transmute` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `CURSED_flags_32768` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `ornate` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `heirloom` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `appearance_only` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `unlocked` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `reforged` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `norepair` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `etheral` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `refined` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `no_salvage` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `indestructable` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `no_experiment` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `house_lore` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `flags2_4096` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `building_block` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `free_reforge` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `infusable` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `merc_only` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `mount_only` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `usable` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `slots` int(10) unsigned NOT NULL DEFAULT '0',
  `set_id` int(10) unsigned DEFAULT NULL,
  `sell_price` int(10) unsigned NOT NULL DEFAULT '0',
  `sell_status_amount` int(10) unsigned NOT NULL DEFAULT '0',
  `stack_count` smallint(5) unsigned NOT NULL DEFAULT '1',
  `collectable` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `collectable_unk` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `adornment_slot1` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `adornment_slot2` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `adornment_slot3` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `adornment_slot4` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `adornment_slot5` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `adornment_slot6` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `offers_quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  `offers_quest_name` text COLLATE latin1_general_ci,
  `required_by_quest_name` text COLLATE latin1_general_ci,
  `soe_autoquest_id` int(11) DEFAULT NULL,
  `part_of_quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  `max_charges` smallint(5) unsigned NOT NULL DEFAULT '0',
  `display_charges` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `recommended_level` smallint(5) unsigned NOT NULL DEFAULT '0',
  `adventure_default_level` smallint(5) unsigned NOT NULL DEFAULT '0',
  `tradeskill_default_level` smallint(5) unsigned NOT NULL DEFAULT '0',
  `required_level` smallint(5) unsigned NOT NULL DEFAULT '1',
  `adventure_classes` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tradeskill_classes` bigint(20) unsigned NOT NULL DEFAULT '0',
  `soe_item_id` int(11) NOT NULL DEFAULT '0',
  `soe_item_crc` int(11) NOT NULL DEFAULT '0',
  `soe_item_id_unsigned` int(10) unsigned NOT NULL DEFAULT '0',
  `soe_item_crc_unsigned` int(10) unsigned NOT NULL DEFAULT '0',
  `lua_script` varchar(255) COLLATE latin1_general_ci DEFAULT NULL,
  `transmuted_material` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `harvest` tinyint(3) unsigned DEFAULT '0',
  `body_drop` tinyint(3) NOT NULL DEFAULT '0',
  `no_buy_back` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `footer_unk_61` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `footer_unk_7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `header_oversized1` smallint(6) NOT NULL DEFAULT '0',
  `header_oversized2` smallint(6) NOT NULL DEFAULT '0',
  `header_bytes5` binary(5) NOT NULL DEFAULT '0\0\0\0\0',
  `header_bytes10` binary(10) NOT NULL DEFAULT '0\0\0\0\0\0\0\0\0\0',
  `header_unk19` int(10) unsigned NOT NULL DEFAULT '0',
  `header_ftr_type_unk` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `Pvp_Id_UIDX` (`bPvpDesc`,`soe_item_id`) USING BTREE,
  KEY `ItemTypeIDX` (`item_type`),
  KEY `ItemNameIDX` (`name`),
  KEY `FK_items_item_itemsets` (`set_id`),
  KEY `FK_items_parsed_logs` (`log_id`),
  CONSTRAINT `FK_items_item_itemsets` FOREIGN KEY (`set_id`) REFERENCES `item_itemsets` (`id`),
  CONSTRAINT `FK_items_parsed_logs` FOREIGN KEY (`log_id`) REFERENCES `parsed_logs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_appearances
DROP TABLE IF EXISTS `item_appearances`;
CREATE TABLE IF NOT EXISTS `item_appearances` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `equip_type` int(10) unsigned NOT NULL DEFAULT '0',
  `red` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `green` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `blue` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `highlight_red` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `highlight_green` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `highlight_blue` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_appearances` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_adornments
DROP TABLE IF EXISTS `item_details_adornments`;
CREATE TABLE IF NOT EXISTS `item_details_adornments` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `duration` float unsigned NOT NULL DEFAULT '0',
  `item_types` bigint(20) unsigned NOT NULL DEFAULT '0',
  `slot_type` smallint(5) unsigned NOT NULL DEFAULT '0',
  `description` text COLLATE latin1_general_ci,
  `description2` text COLLATE latin1_general_ci,
  `unk1` int(10) unsigned NOT NULL DEFAULT '0',
  `unk2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `item_id` (`item_id`),
  CONSTRAINT `FK_ida_itemid` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_armor
DROP TABLE IF EXISTS `item_details_armor`;
CREATE TABLE IF NOT EXISTS `item_details_armor` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `mitigation_low` int(10) unsigned NOT NULL DEFAULT '0',
  `mitigation_high` int(10) unsigned NOT NULL DEFAULT '0',
  `absorb` smallint(6) NOT NULL,
  `unknown` int(11) NOT NULL,
  `item_score` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_details_armor` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_bag
DROP TABLE IF EXISTS `item_details_bag`;
CREATE TABLE IF NOT EXISTS `item_details_bag` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `num_slots` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `weight_reduction` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unknown12` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `backpack` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `unknown81` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unknown69` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ItemIndex` (`item_id`),
  CONSTRAINT `FK_item_details_bag` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_bauble
DROP TABLE IF EXISTS `item_details_bauble`;
CREATE TABLE IF NOT EXISTS `item_details_bauble` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `cast` smallint(6) NOT NULL DEFAULT '100',
  `recovery` smallint(6) NOT NULL DEFAULT '0',
  `duration` int(10) unsigned NOT NULL DEFAULT '0',
  `recast` float NOT NULL DEFAULT '1',
  `display_slot_optional` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `display_cast_time` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `display_bauble_type` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `effect_radius` float NOT NULL DEFAULT '0',
  `max_aoe_targets` int(11) NOT NULL DEFAULT '0',
  `display_until_cancelled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_score` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ItemBaubleIDX` (`item_id`),
  CONSTRAINT `FK_item_details_bauble` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_book
DROP TABLE IF EXISTS `item_details_book`;
CREATE TABLE IF NOT EXISTS `item_details_book` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `language` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `author` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `title` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `ItemBookIDX` (`item_id`),
  CONSTRAINT `FK_item_details_book` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_book_pages
DROP TABLE IF EXISTS `item_details_book_pages`;
CREATE TABLE IF NOT EXISTS `item_details_book_pages` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `page` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `page_text` text COLLATE latin1_general_ci NOT NULL,
  `page_text_valign` tinyint(3) NOT NULL DEFAULT '0',
  `page_text_halign` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `ItemBookIDX` (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_decorations
DROP TABLE IF EXISTS `item_details_decorations`;
CREATE TABLE IF NOT EXISTS `item_details_decorations` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `decoration_name` text COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `DecorationIDX` (`item_id`),
  CONSTRAINT `FK_item_details_decorations` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_food
DROP TABLE IF EXISTS `item_details_food`;
CREATE TABLE IF NOT EXISTS `item_details_food` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` smallint(6) NOT NULL DEFAULT '0',
  `duration` float NOT NULL DEFAULT '0',
  `satiation` tinyint(3) unsigned NOT NULL DEFAULT '2',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ItemIndex` (`item_id`),
  CONSTRAINT `FK_item_details_food` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_house
DROP TABLE IF EXISTS `item_details_house`;
CREATE TABLE IF NOT EXISTS `item_details_house` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `rent_reduction` int(10) unsigned NOT NULL DEFAULT '0',
  `status_rent_reduction` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_rent_reduction` float NOT NULL DEFAULT '0',
  `house_only` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_details_house` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_house_container
DROP TABLE IF EXISTS `item_details_house_container`;
CREATE TABLE IF NOT EXISTS `item_details_house_container` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `num_slots` smallint(5) unsigned NOT NULL DEFAULT '0',
  `allowed_types` bigint(20) unsigned NOT NULL DEFAULT '0',
  `unknown12` int(10) unsigned NOT NULL DEFAULT '0',
  `unknown13` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `broker_commission` smallint(5) unsigned NOT NULL DEFAULT '0',
  `fence_commission` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `HouseContainerIDX` (`item_id`),
  CONSTRAINT `FK_item_details_house_container` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_reward_crate
DROP TABLE IF EXISTS `item_details_reward_crate`;
CREATE TABLE IF NOT EXISTS `item_details_reward_crate` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL,
  `unk1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_item_details_reward_crate_items` (`item_id`),
  CONSTRAINT `FK_item_details_reward_crate_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_reward_crate_item
DROP TABLE IF EXISTS `item_details_reward_crate_item`;
CREATE TABLE IF NOT EXISTS `item_details_reward_crate_item` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `crate_item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `soe_item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `soe_item_crc` int(10) unsigned NOT NULL,
  `icon` smallint(5) unsigned NOT NULL DEFAULT '0',
  `stack_size` int(10) unsigned NOT NULL DEFAULT '0',
  `name_color` int(10) NOT NULL DEFAULT '0',
  `name` text COLLATE latin1_general_ci,
  `language_type` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_item_details_itemset` FOREIGN KEY (`crate_item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;


-- Dumping structure for table eq2parser.item_details_marketplace
DROP TABLE IF EXISTS `item_details_reward_voucher`;
CREATE TABLE IF NOT EXISTS `item_details_reward_voucher` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `voucher_item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `soe_item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `soe_item_crc` int(10) unsigned NOT NULL DEFAULT '0',
  `icon` smallint(5) unsigned NOT NULL DEFAULT '0',
  `name` varchar(255) COLLATE latin1_general_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `VoucherUIDX` (`voucher_item_id`,`soe_item_id`),
  CONSTRAINT `FK_item_details_rewardvoucher` FOREIGN KEY (`voucher_item_id`) REFERENCES `items` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_range
DROP TABLE IF EXISTS `item_details_range`;
CREATE TABLE IF NOT EXISTS `item_details_range` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `dmg_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_high` smallint(5) NOT NULL DEFAULT '0',
  `dmg_mastery_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_mastery_high` smallint(5) NOT NULL DEFAULT '0',
  `dmg_base_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_base_high` smallint(5) NOT NULL DEFAULT '0',
  `delay` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `range_low` smallint(5) unsigned NOT NULL DEFAULT '0',
  `range_high` smallint(5) unsigned NOT NULL DEFAULT '0',
  `damage_rating` float unsigned NOT NULL DEFAULT '0',
  `damage_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_score` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_details_range` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_recipe
DROP TABLE IF EXISTS `item_details_recipe`;
CREATE TABLE IF NOT EXISTS `item_details_recipe` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `max_uses` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ItemIDX` (`item_id`),
  CONSTRAINT `FK_item_details_recipe` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_recipe_items
DROP TABLE IF EXISTS `item_details_recipe_items`;
CREATE TABLE IF NOT EXISTS `item_details_recipe_items` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `icon` smallint(5) unsigned NOT NULL DEFAULT '0',
  `soe_recipe_crc` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_item_details_recipe_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_shield
DROP TABLE IF EXISTS `item_details_shield`;
CREATE TABLE IF NOT EXISTS `item_details_shield` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `mitigation_low` smallint(5) unsigned NOT NULL DEFAULT '0',
  `mitigation_high` smallint(5) unsigned NOT NULL DEFAULT '0',
  `item_score` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_details_shield` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_skill
DROP TABLE IF EXISTS `item_details_skill`;
CREATE TABLE IF NOT EXISTS `item_details_skill` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `spell_id` int(10) unsigned NOT NULL DEFAULT '0',
  `spell_tier` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `soe_spell_crc` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_item_details_skill` (`item_id`),
  CONSTRAINT `FK_item_details_skill` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_thrown
DROP TABLE IF EXISTS `item_details_thrown`;
CREATE TABLE IF NOT EXISTS `item_details_thrown` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `range_bonus` int(11) NOT NULL DEFAULT '0',
  `damage_bonus` int(11) NOT NULL DEFAULT '0',
  `hit_bonus` float NOT NULL DEFAULT '0',
  `damage_type` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`item_id`),
  CONSTRAINT `FK_item_details_thrown` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_weapon
DROP TABLE IF EXISTS `item_details_weapon`;
CREATE TABLE IF NOT EXISTS `item_details_weapon` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `wield_style` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `damage_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `dmg_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_high` smallint(5) NOT NULL DEFAULT '0',
  `dmg_mastery_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_mastery_high` smallint(5) NOT NULL DEFAULT '0',
  `dmg_base_low` smallint(5) NOT NULL DEFAULT '0',
  `dmg_base_high` smallint(5) NOT NULL DEFAULT '0',
  `delay` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `damage_rating` float NOT NULL DEFAULT '0',
  `item_score` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ItemIndex` (`item_id`),
  CONSTRAINT `FK_item_details_weapon` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_effects
DROP TABLE IF EXISTS `item_effects`;
CREATE TABLE IF NOT EXISTS `item_effects` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `effect` varchar(512) COLLATE latin1_general_ci NOT NULL DEFAULT 'Unknown',
  `percentage` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `bullet` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `index` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_itemid_index` (`item_id`,`index`),
  KEY `FK_item_effects` (`item_id`),
  CONSTRAINT `FK_item_effects` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_itemsets
DROP TABLE IF EXISTS `item_itemsets`;
CREATE TABLE IF NOT EXISTS `item_itemsets` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `set_name` varchar(255) COLLATE latin1_general_ci NOT NULL,
  `bPvpDesc` tinyint(4) NOT NULL,
  `itemLevel` smallint(6) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_ItemSet_IDX` (`bPvpDesc`,`set_name`,`itemLevel`) USING BTREE,
  KEY `SetNameIDX` (`set_name`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_itemset_bonus
DROP TABLE IF EXISTS `item_itemset_bonus`;
CREATE TABLE IF NOT EXISTS `item_itemset_bonus` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `set_id` int(10) unsigned NOT NULL,
  `index` int(10) unsigned NOT NULL,
  `num_items_needed` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_setid_index` (`set_id`,`index`),
  CONSTRAINT `FK__item_itemsets` FOREIGN KEY (`set_id`) REFERENCES `item_itemsets` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_itemset_bonus_effects
DROP TABLE IF EXISTS `item_itemset_bonus_effects`;
CREATE TABLE IF NOT EXISTS `item_itemset_bonus_effects` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `set_bonus_id` int(10) unsigned NOT NULL,
  `indent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `description` text COLLATE latin1_general_ci,
  `percentage` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `effect_order` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_setbonusid_effectorder` (`set_bonus_id`,`effect_order`),
  CONSTRAINT `FK_item_itemset_bonus_effects_item_itemset_bonus` FOREIGN KEY (`set_bonus_id`) REFERENCES `item_itemset_bonus` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_itemset_bonus_stats
DROP TABLE IF EXISTS `item_itemset_bonus_stats`;
CREATE TABLE IF NOT EXISTS `item_itemset_bonus_stats` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `set_bonus_id` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `subtype` smallint(6) NOT NULL DEFAULT '0',
  `iValue` int(11) DEFAULT NULL,
  `fValue` float DEFAULT NULL,
  `sValue` text COLLATE latin1_general_ci,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stats_order` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_setbonusid_statsorder` (`set_bonus_id`,`stats_order`),
  CONSTRAINT `FK_item_itemset_bonus_stats_item_itemset_bonus` FOREIGN KEY (`set_bonus_id`) REFERENCES `item_itemset_bonus` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_itemset_items
DROP TABLE IF EXISTS `item_itemset_items`;
CREATE TABLE IF NOT EXISTS `item_itemset_items` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `set_id` int(10) unsigned NOT NULL,
  `item_name` text COLLATE latin1_general_ci NOT NULL,
  `index` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_item_itemset_items_item_itemsets` (`set_id`),
  CONSTRAINT `FK_item_itemset_items_item_itemsets` FOREIGN KEY (`set_id`) REFERENCES `item_itemsets` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_mod_stats
DROP TABLE IF EXISTS `item_mod_stats`;
CREATE TABLE IF NOT EXISTS `item_mod_stats` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `subtype` smallint(6) NOT NULL DEFAULT '0',
  `iValue` int(11) DEFAULT NULL,
  `fValue` float DEFAULT NULL,
  `sValue` text COLLATE latin1_general_ci,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stats_order` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_setbonusid_statsorder` (`item_id`,`stats_order`) USING BTREE,
  CONSTRAINT `FK_item_mod_stats_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_mod_strings
DROP TABLE IF EXISTS `item_mod_strings`;
CREATE TABLE IF NOT EXISTS `item_mod_strings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL,
  `mod` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `description` text COLLATE latin1_general_ci NOT NULL,
  `unk1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `index` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_itemid_index` (`index`,`item_id`) USING BTREE,
  KEY `FK_item_mod_strings_items` (`item_id`),
  CONSTRAINT `FK_item_mod_strings_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.parsed_logs
DROP TABLE IF EXISTS `parsed_logs`;
CREATE TABLE IF NOT EXISTS `parsed_logs` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` text COLLATE latin1_general_ci NOT NULL,
  `dataVersion` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

-- Dumping structure for table eq2parser.item_details_achievement_profile
DROP TABLE IF EXISTS `item_details_achievement_profile`;
CREATE TABLE IF NOT EXISTS `item_details_achievement_profile` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `item_id` int(10) unsigned NOT NULL,
  `status_reduction` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_reduction` float NOT NULL DEFAULT '0',
  `house_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unk_string` text COLLATE latin1_general_ci,
  `unk1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_item_details_achievement_profile_items` (`item_id`),
  CONSTRAINT `FK_item_details_achievement_profile_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Dumping structure for table eq2parser.item_pvp_link
DROP TABLE IF EXISTS `item_pvp_link`;
CREATE TABLE IF NOT EXISTS `item_pvp_link` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `base_item` int(10) unsigned NOT NULL,
  `pvp_item` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK_baseitem` (`base_item`),
  UNIQUE KEY `UK_pvpitem` (`pvp_item`),
  CONSTRAINT `FK__items` FOREIGN KEY (`base_item`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK__items_2` FOREIGN KEY (`pvp_item`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.


/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
