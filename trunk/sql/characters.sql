-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.6.41-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             9.5.0.5196
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table eq2rewrite.characters
CREATE TABLE IF NOT EXISTS `characters` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `server_id` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(64) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `deity` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `body_size` float NOT NULL DEFAULT '0',
  `body_age` float NOT NULL DEFAULT '0',
  `current_zone_id` int(11) unsigned NOT NULL DEFAULT '253',
  `level` int(10) unsigned NOT NULL DEFAULT '1',
  `tradeskill_class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tradeskill_level` int(10) unsigned NOT NULL DEFAULT '1',
  `soga_wing_type` int(10) unsigned NOT NULL,
  `soga_chest_type` int(10) unsigned NOT NULL,
  `soga_legs_type` int(10) unsigned NOT NULL,
  `soga_hair_type` int(10) unsigned NOT NULL,
  `soga_facial_hair_type` int(10) unsigned NOT NULL DEFAULT '0',
  `soga_model_type` int(10) unsigned NOT NULL,
  `legs_type` int(10) unsigned NOT NULL,
  `chest_type` int(10) unsigned NOT NULL,
  `wing_type` int(10) unsigned NOT NULL,
  `hair_type` int(10) unsigned NOT NULL,
  `facial_hair_type` int(10) unsigned NOT NULL DEFAULT '0',
  `model_type` int(10) unsigned NOT NULL,
  `tail_type` int(10) unsigned NOT NULL,
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `heading` float NOT NULL DEFAULT '0',
  `instance_id` int(10) unsigned NOT NULL DEFAULT '0',
  `starting_city` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `deleted` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `unix_timestamp` int(10) NOT NULL DEFAULT '0',
  `created_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_played` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `last_saved` int(10) unsigned NOT NULL DEFAULT '0',
  `admin_status` varchar(64) CHARACTER SET utf8 DEFAULT NULL,
  `is_online` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `CharIDX` (`name`),
  KEY `CharZoneIDX` (`current_zone_id`),
  KEY `CharAcctIDX` (`account_id`),
  CONSTRAINT `FK_characters` FOREIGN KEY (`current_zone_id`) REFERENCES `zones` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
