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

-- Dumping structure for table eq2rewrite.chat_filters
CREATE TABLE IF NOT EXISTS `chat_filters` (
  `version` int(10) unsigned NOT NULL,
  `id` int(10) unsigned NOT NULL,
  `name` text COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`version`,`id`),
  UNIQUE KEY `Index 2` (`version`,`name`(100))
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- Dumping data for table eq2rewrite.chat_filters: ~109 rows (approximately)
/*!40000 ALTER TABLE `chat_filters` DISABLE KEYS */;
INSERT INTO `chat_filters` (`version`, `id`, `name`) VALUES
	(60114, 2, 'GameText'),
	(60114, 3, 'DefaultText'),
	(60114, 4, 'ErrorText'),
	(60114, 5, 'StatusText'),
	(60114, 6, 'MOTD'),
	(60114, 7, 'ChatText'),
	(60114, 8, 'NearbyChat'),
	(60114, 9, 'Say'),
	(60114, 10, 'Shout'),
	(60114, 11, 'Emote'),
	(60114, 12, 'Yell'),
	(60114, 13, 'Narrative'),
	(60114, 14, 'NPCSay'),
	(60114, 15, 'GroupChat'),
	(60114, 16, 'GroupSay'),
	(60114, 17, 'RaidSay'),
	(60114, 18, 'ArenaSay'),
	(60114, 19, 'GuildChat'),
	(60114, 20, 'GuildSay'),
	(60114, 21, 'GuildOfficer'),
	(60114, 22, 'GuildMOTD'),
	(60114, 23, 'GuildMemberOnline'),
	(60114, 24, 'GuildEvent'),
	(60114, 25, 'GuildRecruitingPage'),
	(60114, 26, 'GuildRecruitingPageOther'),
	(60114, 27, 'PrivateChat'),
	(60114, 28, 'NonPlayerTell'),
	(60114, 29, 'ObjectText'),
	(60114, 30, 'Tell'),
	(60114, 31, 'CustomerService'),
	(60114, 32, 'Spam'),
	(60114, 33, 'ChatChannelText'),
	(60114, 34, 'OutOfCharacter'),
	(60114, 35, 'Auction'),
	(60114, 36, 'CharacterText'),
	(60114, 37, 'Reward'),
	(60114, 38, 'Death'),
	(60114, 39, 'Pet'),
	(60114, 40, 'Skill'),
	(60114, 41, 'Faction'),
	(60114, 42, 'Spells'),
	(60114, 43, 'YouCast'),
	(60114, 44, 'YouFail'),
	(60114, 45, 'CriticalCast'),
	(60114, 46, 'FriendlyCast'),
	(60114, 47, 'FriendlyFail'),
	(60114, 48, 'OtherCast'),
	(60114, 49, 'OtherFail'),
	(60114, 50, 'HostileCast'),
	(60114, 51, 'HostileFail'),
	(60114, 52, 'WornOff'),
	(60114, 53, 'SpellsOther'),
	(60114, 54, 'HealSpells'),
	(60114, 55, 'Heals'),
	(60114, 56, 'FriendlyHeals'),
	(60114, 57, 'OtherHeals'),
	(60114, 58, 'HostileHeals'),
	(60114, 59, 'CriticalHeals'),
	(60114, 60, 'Combat'),
	(60114, 61, 'GeneralCombat'),
	(60114, 62, 'HeroicOpportunity'),
	(60114, 63, 'NonMeleeDamage'),
	(60114, 64, 'DamageShield'),
	(60114, 65, 'Ward'),
	(60114, 66, 'DamageIntercept'),
	(60114, 67, 'MeleeCombat'),
	(60114, 68, 'Warnings'),
	(60114, 69, 'YouHit'),
	(60114, 70, 'YouMiss'),
	(60114, 71, 'AttackerHits'),
	(60114, 72, 'AttackerMisses'),
	(60114, 73, 'YourPetHits'),
	(60114, 74, 'YourPetMisses'),
	(60114, 75, 'AttackerHitsPet'),
	(60114, 76, 'AttackerMissesPet'),
	(60114, 77, 'OtherHits'),
	(60114, 78, 'OtherMisses'),
	(60114, 79, 'CriticalHit'),
	(60114, 80, 'HateAdjustments'),
	(60114, 81, 'YourHate'),
	(60114, 82, 'OthersHate'),
	(60114, 83, 'DispelsAndCures'),
	(60114, 84, 'DispelYou'),
	(60114, 85, 'DispelOther'),
	(60114, 86, 'CureYou'),
	(60114, 87, 'CureOther'),
	(60114, 88, 'Stoneskins'),
	(60114, 89, 'YourStoneskins'),
	(60114, 90, 'TargetStoneskins'),
	(60114, 91, 'Other'),
	(60114, 92, 'MoneySplit'),
	(60114, 93, 'Loot'),
	(60114, 94, 'LootRolls'),
	(60114, 95, 'CommandText'),
	(60114, 96, 'Broadcast'),
	(60114, 97, 'Who'),
	(60114, 98, 'Commands'),
	(60114, 99, 'Merchant'),
	(60114, 100, 'MerchantBuySell'),
	(60114, 101, 'ConsiderMessages'),
	(60114, 102, 'ConNeg2'),
	(60114, 103, 'ConNeg1'),
	(60114, 104, 'ConZero'),
	(60114, 105, 'ConOne'),
	(60114, 106, 'ConTwo'),
	(60114, 107, 'Tradeskills'),
	(60114, 108, 'Harvesting'),
	(60114, 109, 'HarvestingWarnings'),
	(60114, 110, 'VoiceChat');
/*!40000 ALTER TABLE `chat_filters` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
