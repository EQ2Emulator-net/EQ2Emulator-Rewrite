
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
DROP TABLE IF EXISTS `characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `characters` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `server_id` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `deity` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `body_size` float NOT NULL DEFAULT '0',
  `body_age` float NOT NULL DEFAULT '0',
  `current_zone_id` int(11) unsigned NOT NULL DEFAULT '1',
  `level` int(10) unsigned NOT NULL DEFAULT '1',
  `tradeskill_class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tradeskill_level` int(10) unsigned NOT NULL DEFAULT '1',
  `soga_wing_type` mediumint(8) unsigned NOT NULL,
  `soga_chest_type` mediumint(8) unsigned NOT NULL,
  `soga_legs_type` mediumint(8) unsigned NOT NULL,
  `soga_hair_type` mediumint(8) unsigned NOT NULL,
  `soga_facial_hair_type` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `soga_model_type` mediumint(8) unsigned NOT NULL,
  `legs_type` mediumint(8) unsigned NOT NULL,
  `chest_type` mediumint(8) unsigned NOT NULL,
  `wing_type` mediumint(8) unsigned NOT NULL,
  `hair_type` mediumint(8) unsigned NOT NULL,
  `facial_hair_type` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `model_type` mediumint(8) unsigned NOT NULL,
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
  `admin_status` int(10) NOT NULL DEFAULT '0',
  `is_online` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `CharIDX` (`name`),
  KEY `CharZoneIDX` (`current_zone_id`),
  KEY `CharAcctIDX` (`account_id`),
  CONSTRAINT `FK_characters` FOREIGN KEY (`current_zone_id`) REFERENCES `zones` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=907 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

