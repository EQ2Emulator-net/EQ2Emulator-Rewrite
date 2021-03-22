-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.3.15-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             10.3.0.5771
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table eq2_rewrite.config_world
DROP TABLE IF EXISTS `config_world`;
CREATE TABLE IF NOT EXISTS `config_world` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(255) COLLATE latin1_general_ci NOT NULL,
  `locked` tinyint(3) unsigned NOT NULL DEFAULT 0,
  `character_slots` tinyint(3) unsigned NOT NULL DEFAULT 10,
  `max_adv_level` tinyint(3) unsigned NOT NULL DEFAULT 50,
  `max_ts_level` tinyint(3) unsigned NOT NULL DEFAULT 50,
  `allowed_races` int(10) unsigned NOT NULL DEFAULT 4194303,
  `allowed_classes` int(10) unsigned NOT NULL DEFAULT 134217726,
  `auto_account_creation` tinyint(3) unsigned NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
