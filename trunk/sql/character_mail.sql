
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
DROP TABLE IF EXISTS `character_mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_mail` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `player_to_id` int(10) unsigned NOT NULL DEFAULT '0',
  `player_from` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `subject` varchar(255) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `mail_body` text COLLATE latin1_general_ci,
  `already_read` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `mail_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `coin_copper` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_silver` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_gold` int(10) unsigned NOT NULL DEFAULT '0',
  `coin_plat` int(10) unsigned NOT NULL DEFAULT '0',
  `stack` smallint(5) unsigned NOT NULL DEFAULT '0',
  `postage_cost` int(10) unsigned NOT NULL DEFAULT '0',
  `attachment_cost` int(10) unsigned NOT NULL DEFAULT '0',
  `char_item_id` int(10) unsigned NOT NULL DEFAULT '0',
  `time_sent` int(10) unsigned NOT NULL DEFAULT '0',
  `expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_character_mail` (`player_to_id`),
  CONSTRAINT `FK_character_mail` FOREIGN KEY (`player_to_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

