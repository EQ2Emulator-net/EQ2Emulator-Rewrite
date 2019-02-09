#pragma once

#include <map>
#include <string>
#include <stdint.h>

#define COMMONER		 0
#define FIGHTER			 1
#define WARRIOR			 2
#define GUARDIAN		 3
#define BERSERKER		 4
#define BRAWLER			 5
#define MONK			 6
#define BRUISER			 7
#define CRUSADER		 8
#define SHADOWKNIGHT	 9
#define PALADIN			10
#define PRIEST			11
#define CLERIC			12
#define TEMPLAR			13
#define INQUISITOR		14
#define DRUID			15
#define WARDEN			16
#define FURY			17
#define SHAMAN			18
#define MYSTIC			19
#define DEFILER			20
#define MAGE			21
#define SORCERER		22
#define WIZARD			23
#define WARLOCK			24
#define ENCHANTER		25
#define ILLUSIONIST		26
#define COERCER			27
#define SUMMONER		28
#define CONJUROR		29
#define NECROMANCER		30
#define SCOUT			31
#define ROGUE			32
#define SWASHBUCKLER	33
#define BRIGAND			34
#define BARD			35
#define TROUBADOR		36
#define DIRGE			37
#define PREDATOR		38
#define RANGER			39
#define ASSASSIN		40
#define ANIMALIST		41
#define BEASTLORD		42
#define SHAPER          43
#define CHANNELER       44

//Tradeskills
// 0 - transmuting/tinkering
#define ARTISAN			45 // 1
#define CRAFTSMAN		46 // 2
#define PROVISIONER		47 // 3
#define WOODWORKER		48 // 4
#define CARPENTER		49 // 5
#define OUTFITTER		50 // 6
#define ARMORER			51 // 7
#define WEAPONSMITH		52 // 8
#define TAILOR			53 // 9
#define SCHOLAR			54 // 10
#define JEWELER			55 // 11
#define SAGE			56 // 12
#define ALCHEMIST		57 // 13

class Classes {
public:
	Classes();
	char* GetEQClassName(uint8_t class_, uint8_t level);
	std::string GetClassName(uint8_t class_id);
	std::string GetClassNameCase(uint8_t class_id);
	int8_t GetClassID(std::string name);
	static uint8_t GetBaseClass(uint8_t class_id);
	static uint8_t GetSecondaryBaseClass(uint8_t class_id);
	static uint8_t GetTSBaseClass(uint8_t class_id);
	static uint8_t GetSecondaryTSBaseClass(uint8_t class_id);

private:
	std::map<std::string, uint8_t> class_map;
};