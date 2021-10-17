#include "stdafx.h"

#include "Classes.h"

Classes::Classes() {
	class_map["COMMONER"] = 0;
	class_map["FIGHTER"] = 1;
	class_map["WARRIOR"] = 2;
	class_map["GUARDIAN"] = 3;
	class_map["BERSERKER"] = 4;
	class_map["BRAWLER"] = 5;
	class_map["MONK"] = 6;
	class_map["BRUISER"] = 7;
	class_map["CRUSADER"] = 8;
	class_map["SHADOWKNIGHT"] = 9;
	class_map["PALADIN"] = 10;
	class_map["PRIEST"] = 11;
	class_map["CLERIC"] = 12;
	class_map["TEMPLAR"] = 13;
	class_map["INQUISITOR"] = 14;
	class_map["DRUID"] = 15;
	class_map["WARDEN"] = 16;
	class_map["FURY"] = 17;
	class_map["SHAMAN"] = 18;
	class_map["MYSTIC"] = 19;
	class_map["DEFILER"] = 20;
	class_map["MAGE"] = 21;
	class_map["SORCERER"] = 22;
	class_map["WIZARD"] = 23;
	class_map["WARLOCK"] = 24;
	class_map["ENCHANTER"] = 25;
	class_map["ILLUSIONIST"] = 26;
	class_map["COERCER"] = 27;
	class_map["SUMMONER"] = 28;
	class_map["CONJUROR"] = 29;
	class_map["NECROMANCER"] = 30;
	class_map["SCOUT"] = 31;
	class_map["ROGUE"] = 32;
	class_map["SWASHBUCKLER"] = 33;
	class_map["BRIGAND"] = 34;
	class_map["BARD"] = 35;
	class_map["TROUBADOR"] = 36;
	class_map["DIRGE"] = 37;
	class_map["PREDATOR"] = 38;
	class_map["RANGER"] = 39;
	class_map["ASSASSIN"] = 40;
	class_map["ANIMALIST"] = 41;
	class_map["BEASTLORD"] = 42;
	class_map["SHAPER"] = 43;
	class_map["CHANNELER"] = 44;
	class_map["ARTISAN"] = 45;
	class_map["CRAFTSMAN"] = 46;
	class_map["PROVISIONER"] = 47;
	class_map["WOODWORKER"] = 48;
	class_map["CARPENTER"] = 49;
	class_map["OUTFITTER"] = 50;
	class_map["ARMORER"] = 51;
	class_map["WEAPONSMITH"] = 52;
	class_map["TAILOR"] = 53;
	class_map["SCHOLAR"] = 54;
	class_map["JEWELER"] = 55;
	class_map["SAGE"] = 56;
	class_map["ALCHEMIST"] = 57;
}

uint8_t Classes::GetBaseClass(uint8_t class_id) {
	uint8_t ret = 0;
	if (class_id >= WARRIOR && class_id <= PALADIN)
		ret = FIGHTER;
	if ((class_id >= CLERIC && class_id <= DEFILER) || (class_id == SHAPER || class_id == CHANNELER))
		ret = PRIEST;
	if (class_id >= SORCERER && class_id <= NECROMANCER)
		ret = MAGE;
	if (class_id >= ROGUE && class_id <= BEASTLORD)
		ret = SCOUT;
	//LogWrite(WORLD__DEBUG, 5, "World", "%s returning base class ID: %i", __FUNCTION__, ret);
	return ret;
}

uint8_t Classes::GetSecondaryBaseClass(uint8_t class_id) {
	uint8_t ret = 0;
	if (class_id == GUARDIAN || class_id == BERSERKER)
		ret = WARRIOR;
	if (class_id == MONK || class_id == BRUISER)
		ret = BRAWLER;
	if (class_id == SHADOWKNIGHT || class_id == PALADIN)
		ret = CRUSADER;
	if (class_id == TEMPLAR || class_id == INQUISITOR)
		ret = CLERIC;
	if (class_id == WARDEN || class_id == FURY)
		ret = DRUID;
	if (class_id == MYSTIC || class_id == DEFILER)
		ret = SHAMAN;
	if (class_id == WIZARD || class_id == WARLOCK)
		ret = SORCERER;
	if (class_id == ILLUSIONIST || class_id == COERCER)
		ret = ENCHANTER;
	if (class_id == CONJUROR || class_id == NECROMANCER)
		ret = SUMMONER;
	if (class_id == SWASHBUCKLER || class_id == BRIGAND)
		ret = ROGUE;
	if (class_id == TROUBADOR || class_id == DIRGE)
		ret = BARD;
	if (class_id == RANGER || class_id == ASSASSIN)
		ret = PREDATOR;
	if (class_id == BEASTLORD)
		ret = ANIMALIST;
	if (class_id == CHANNELER)
		ret = SHAPER;
	//LogWrite(WORLD__DEBUG, 5, "World", "%s returning secondary class ID: %i", __FUNCTION__, ret);
	return ret;
}

uint8_t Classes::GetTSBaseClass(uint8_t class_id) {
	class_id += 42;
	uint8_t ret = 0;
	if (class_id >= ARTISAN)
		ret = ARTISAN;

	//LogWrite(WORLD__DEBUG, 5, "World", "%s returning base tradeskill class ID: %i", __FUNCTION__, ret - 44);
	return ret - 44;
}

uint8_t Classes::GetSecondaryTSBaseClass(uint8_t class_id) {
	class_id += 42;
	uint8_t ret = 0;
	if (class_id == ARTISAN)
		ret = ARTISAN;
	if (class_id >= CRAFTSMAN && class_id < OUTFITTER)
		ret = CRAFTSMAN;
	if (class_id >= OUTFITTER && class_id < SCHOLAR)
		ret = OUTFITTER;
	if (class_id >= SCHOLAR)
		ret = SCHOLAR;

	//LogWrite(WORLD__DEBUG, 5, "World", "%s returning secondary tradeskill class ID: %i", __FUNCTION__, ret - 44);
	return ret - 44;
}

int8_t Classes::GetClassID(std::string name) {
	std::string class_name = name;
	std::transform(name.begin(), name.end(), class_name.begin(), ::toupper);
	if (class_map.count(class_name) == 1) {
		//LogWrite(WORLD__DEBUG, 5, "World", "%s returning class ID: %i for class name %s", __FUNCTION__, class_map[class_name], class_name.c_str());
		return class_map[class_name];
	}
	//LogWrite(WORLD__WARNING, 0, "World", "Could not find class_id in function: %s (return -1)", __FUNCTION__);
	return -1;
}

std::string Classes::GetClassName(uint8_t class_id) {
	std::map<std::string, uint8_t>::iterator itr;
	for (itr = class_map.begin(); itr != class_map.end(); itr++) {
		if (itr->second == class_id) {
			//LogWrite(WORLD__DEBUG, 5, "World", "%s returning class name: %s for class_id %i", __FUNCTION__, itr->first.c_str(), class_id);
			return itr->first.c_str();
		}
	}
	//LogWrite(WORLD__WARNING, 0, "World", "Could not find class name in function: %s (return 0)", __FUNCTION__);
	return std::string();
}

std::string Classes::GetClassNameCase(uint8_t class_id) {
	std::map<std::string, uint8_t>::iterator itr;
	for (itr = class_map.begin(); itr != class_map.end(); itr++) {
		if (itr->second == class_id) {
			std::string class_name = itr->first;
			class_name[0] = itr->first[0];
			std::transform(itr->first.begin() + 1, itr->first.end(), class_name.begin() + 1, ::tolower);
			//LogWrite(WORLD__DEBUG, 5, "World", "%s returning class name: %s for class_id %i", __FUNCTION__, class_name.c_str(), class_id);
			return class_name;
		}
	}
	//LogWrite(WORLD__WARNING, 0, "World", "Could not find class name in function: %s (return blank)", __FUNCTION__);
	return "";
}