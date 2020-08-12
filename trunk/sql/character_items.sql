
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
DROP TABLE IF EXISTS `character_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_items` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `type` enum('EQUIPPED','NOT-EQUIPPED','APPEARANCE','DELETED') NOT NULL DEFAULT 'NOT-EQUIPPED',
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `bag_id` int(11) NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `creator` varchar(64) NOT NULL DEFAULT '',
  `condition_` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `attuned` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` smallint(5) unsigned NOT NULL DEFAULT '1',
  `max_sell_value` int(10) unsigned NOT NULL DEFAULT '0',
  `login_checksum` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn0` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn1` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn2` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn1_time` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn3` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn4` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn5` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn6` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn7` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn8` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn9` int(10) unsigned NOT NULL DEFAULT '0',
  `adorn10` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `NewIndex` (`char_id`,`type`,`bag_id`,`slot`),
  KEY `FK_items` (`item_id`),
  CONSTRAINT `FK_character_items` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_items` FOREIGN KEY (`item_id`) REFERENCES `items` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2607181 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

