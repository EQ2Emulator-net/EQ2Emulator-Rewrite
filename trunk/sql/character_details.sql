
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
DROP TABLE IF EXISTS `character_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character_details` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `hp` int(10) unsigned NOT NULL DEFAULT '0',
  `power` int(10) unsigned NOT NULL DEFAULT '0',
  `savagery` int(10) unsigned NOT NULL DEFAULT '0',
  `dissonance` int(10) unsigned NOT NULL DEFAULT '0',
  `max_concentration` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `attack` int(10) unsigned NOT NULL DEFAULT '0',
  `mitigation` smallint(5) unsigned NOT NULL DEFAULT '0',
  `avoidance` smallint(5) unsigned NOT NULL DEFAULT '0',
  `parry` smallint(5) unsigned NOT NULL DEFAULT '0',
  `deflection` smallint(5) unsigned NOT NULL DEFAULT '0',
  `block` smallint(5) unsigned NOT NULL DEFAULT '0',
  `str` smallint(5) unsigned NOT NULL DEFAULT '0',
  `sta` smallint(5) unsigned NOT NULL DEFAULT '0',
  `agi` smallint(5) unsigned NOT NULL DEFAULT '0',
  `wis` smallint(5) unsigned NOT NULL DEFAULT '0',
  `intel` smallint(5) unsigned NOT NULL DEFAULT '0',
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
  `pet_name` varchar(32) COLLATE latin1_general_ci NOT NULL DEFAULT 'No Pet',
  `status_points` int(10) unsigned NOT NULL DEFAULT '0',
  `max_power` int(10) unsigned NOT NULL DEFAULT '0',
  `max_hp` int(10) unsigned NOT NULL DEFAULT '0',
  `max_savagery` int(10) unsigned NOT NULL DEFAULT '0',
  `max_dissonance` int(10) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `xp_needed` int(10) unsigned NOT NULL DEFAULT '0',
  `xp_debt` int(10) unsigned NOT NULL DEFAULT '0',
  `xp_vitality` float unsigned NOT NULL DEFAULT '0',
  `tradeskill_xp` int(10) unsigned NOT NULL DEFAULT '0',
  `tradeskill_xp_needed` int(10) unsigned NOT NULL DEFAULT '0',
  `tradeskill_xp_debt` int(10) unsigned NOT NULL DEFAULT '0',
  `tradeskill_xp_vitality` float unsigned NOT NULL DEFAULT '0',
  `bank_copper` int(10) unsigned NOT NULL DEFAULT '0',
  `bank_silver` int(10) unsigned NOT NULL DEFAULT '0',
  `bank_gold` int(10) unsigned NOT NULL DEFAULT '0',
  `bank_plat` int(10) unsigned NOT NULL DEFAULT '0',
  `bind_zone_id` int(10) unsigned NOT NULL DEFAULT '0',
  `bind_x` float NOT NULL DEFAULT '0',
  `bind_y` float NOT NULL DEFAULT '0',
  `bind_z` float NOT NULL DEFAULT '0',
  `bind_heading` float unsigned NOT NULL DEFAULT '0',
  `house_zone_id` int(10) unsigned NOT NULL DEFAULT '0',
  `combat_voice` smallint(5) unsigned NOT NULL DEFAULT '52',
  `emote_voice` smallint(5) unsigned NOT NULL DEFAULT '1060',
  `biography` varchar(512) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `flags` int(10) unsigned NOT NULL DEFAULT '301465665',
  `flags2` int(10) unsigned NOT NULL DEFAULT '40',
  `prefix_title` smallint(5) NOT NULL DEFAULT '-1',
  `suffix_title` smallint(5) NOT NULL DEFAULT '-1',
  `current_language` smallint(5) NOT NULL DEFAULT '0',
  `last_name` varchar(64) COLLATE latin1_general_ci NOT NULL DEFAULT '',
  `assigned_aa` smallint(5) NOT NULL DEFAULT '0',
  `unassigned_aa` smallint(5) NOT NULL DEFAULT '0',
  `tradeskill_aa` smallint(5) NOT NULL DEFAULT '0',
  `unassigned_tradeskill_aa` smallint(5) NOT NULL DEFAULT '0',
  `prestige_aa` smallint(5) NOT NULL DEFAULT '0',
  `unassigned_prestige_aa` smallint(5) NOT NULL DEFAULT '0',
  `tradeskill_prestige_aa` smallint(5) NOT NULL DEFAULT '0',
  `unassigned_tradeskill_prestige_aa` smallint(5) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `CharIDX` (`char_id`),
  CONSTRAINT `FK_character_details` FOREIGN KEY (`char_id`) REFERENCES `characters` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
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

