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

-- Dumping structure for table eq2rewrite.starting_details
CREATE TABLE IF NOT EXISTS `starting_details` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `race_id` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `class_id` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `max_hp` int(10) unsigned NOT NULL DEFAULT '0',
  `max_power` int(10) unsigned NOT NULL DEFAULT '0',
  `max_savagery` int(10) unsigned NOT NULL DEFAULT '0',
  `max_dissonance` int(10) unsigned NOT NULL DEFAULT '0',
  `max_concentration` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `str` smallint(5) unsigned NOT NULL DEFAULT '0',
  `agi` smallint(5) unsigned NOT NULL DEFAULT '0',
  `sta` smallint(5) unsigned NOT NULL DEFAULT '0',
  `intel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `wis` smallint(5) unsigned NOT NULL DEFAULT '0',
  `heat` smallint(5) unsigned NOT NULL DEFAULT '0',
  `cold` smallint(5) unsigned NOT NULL DEFAULT '0',
  `magic` smallint(5) unsigned NOT NULL DEFAULT '0',
  `mental` smallint(5) unsigned NOT NULL DEFAULT '0',
  `divine` smallint(5) unsigned NOT NULL DEFAULT '0',
  `disease` smallint(5) unsigned NOT NULL DEFAULT '0',
  `poison` smallint(5) unsigned NOT NULL DEFAULT '0',
  `elemental` smallint(5) unsigned NOT NULL DEFAULT '0',
  `arcane` smallint(5) unsigned NOT NULL DEFAULT '0',
  `noxious` smallint(5) unsigned NOT NULL DEFAULT '0',
  `coin_copper` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_silver` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_gold` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_plat` int(10) unsigned NOT NULL DEFAULT '0',
  `status_points` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `RaceClassIDX` (`race_id`,`class_id`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=latin1;

-- Dumping data for table eq2rewrite.starting_details: ~21 rows (approximately)
/*!40000 ALTER TABLE `starting_details` DISABLE KEYS */;
INSERT INTO `starting_details` (`id`, `race_id`, `class_id`, `max_hp`, `max_power`, `max_savagery`, `max_dissonance`, `max_concentration`, `str`, `agi`, `sta`, `intel`, `wis`, `heat`, `cold`, `magic`, `mental`, `divine`, `disease`, `poison`, `elemental`, `arcane`, `noxious`, `coin_copper`, `coin_silver`, `coin_gold`, `coin_plat`, `status_points`) VALUES
	(1, 0, 255, 40, 45, 0, 0, 0, 25, 20, 25, 12, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(2, 1, 255, 40, 45, 0, 0, 0, 13, 23, 15, 26, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(3, 2, 255, 40, 45, 0, 0, 0, 22, 16, 25, 12, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(4, 3, 255, 40, 45, 0, 0, 0, 12, 18, 15, 30, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(5, 4, 255, 40, 45, 0, 0, 0, 16, 24, 20, 20, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(6, 5, 255, 40, 45, 0, 0, 0, 12, 25, 16, 27, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(7, 6, 255, 40, 45, 0, 0, 0, 17, 25, 18, 21, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8, 7, 255, 40, 45, 0, 0, 0, 15, 30, 17, 16, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(9, 8, 255, 40, 45, 0, 0, 0, 10, 22, 15, 23, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(10, 9, 255, 40, 45, 0, 0, 0, 20, 20, 20, 20, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(11, 10, 255, 40, 45, 0, 0, 0, 18, 22, 18, 20, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(12, 11, 255, 40, 45, 0, 0, 0, 22, 23, 20, 15, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(13, 12, 255, 40, 45, 0, 0, 0, 30, 15, 25, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(14, 13, 255, 40, 45, 0, 0, 0, 10, 30, 15, 25, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(15, 14, 255, 40, 45, 0, 0, 0, 25, 18, 30, 10, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(16, 15, 255, 40, 45, 0, 0, 0, 14, 30, 16, 17, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(17, 16, 255, 40, 45, 0, 0, 0, 10, 30, 10, 25, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(18, 17, 255, 40, 45, 0, 0, 0, 10, 30, 10, 25, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(19, 18, 255, 40, 45, 0, 0, 0, 21, 17, 23, 15, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(20, 19, 255, 40, 45, 0, 0, 0, 15, 25, 18, 25, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(21, 20, 255, 40, 45, 0, 0, 0, 25, 18, 30, 10, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(22, 21, 255, 40, 45, 0, 0, 0, 22, 23, 20, 15, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*!40000 ALTER TABLE `starting_details` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
