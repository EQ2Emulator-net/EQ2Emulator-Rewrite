-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.3.15-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table eq2_rewrite.account
DROP TABLE IF EXISTS `account`;
CREATE TABLE IF NOT EXISTS `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `passwd` varchar(128) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `ip_address` varchar(50) COLLATE latin1_general_ci NOT NULL DEFAULT '0',
  `email_address` varchar(50) COLLATE latin1_general_ci NOT NULL DEFAULT 'Unknown',
  `created_date` int(10) NOT NULL DEFAULT 0,
  `allowed_classes` int(10) unsigned NOT NULL DEFAULT 0,
  `allowed_races` int(10) unsigned NOT NULL DEFAULT 0,
  `character_slots` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `key1` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '0',
  `last_update` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `hack_count` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `last_client_version` smallint(5) unsigned NOT NULL DEFAULT 0,
  `account_enabled` tinyint(1) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `Name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
