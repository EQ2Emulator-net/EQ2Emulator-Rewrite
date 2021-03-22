#pragma once

#include "../Spawns/Entity.h"
#include "../Database/ZoneDatabase.h"
#include "../ZoneServer/ZoneServer.h"
#include "../../common/DatabaseRow.h"

extern ZoneDatabase database;

//Can add other game currencies to this
struct CharacterCurrency {
	uint64_t CoinsToInt() {
		return copper + silver * 100 + gold * 10000ull + platinum * 1000000ull;
	}

	uint32_t copper;
	uint32_t silver;
	uint32_t gold;
	uint32_t platinum;

	//Maybe move this to a separate banking handler?
	uint32_t bankCopper;
	uint32_t bankSilver;
	uint32_t bankGold;
	uint32_t bankPlatinum;
};

struct CharacterExperience {
	uint32_t currentAdvExp;
	uint32_t nextAdvLevelExp;
	uint32_t advExpDebt;
	uint32_t advVitality;
	uint32_t currentTsExp;
	uint32_t nextTsLevelExp;
	uint32_t tsVitality;
	uint32_t currentAaExp;
	uint32_t nextAaLevelExp;
};

struct CharacterDetails {
	CharacterDetails() {
		//Not loading these yet
		alignment = 0;
		attributes = nullptr;
	}

	virtual ~CharacterDetails() = default;

	void LinkToGhost(const std::shared_ptr<Entity>& playerGhost) {
		entity = playerGhost;
		if (playerGhost) {
			attributes = &entity->attributes;
			namingInfo = &entity->m_titleStruct;
			combatVoice = &entity->m_infoStruct.combat_voice;
			emoteVoice = &entity->m_infoStruct.emote_voice;
			advLevel = &entity->m_infoStruct.level;
			advOrigLevel = &entity->m_infoStruct.orig_level;
			gender = &entity->m_infoStruct.gender;
			race = &entity->m_infoStruct.race;
		}
	}

	//Keep the player spawn pointer alive until we're done with this struct
	std::shared_ptr<Entity> entity;

	//These should be stored on the player's ghost, can add overrides later in the CharacterSheet for possessing things as needed
	EntityAttributeSheet* attributes;
	SpawnTitleStruct* namingInfo;
	uint32_t* combatVoice;
	uint32_t* emoteVoice;
	uint16_t* advLevel;
	uint16_t* advOrigLevel;
	uint16_t tsLevel;
	//0 = good, 1 = neutral (exile), 2 = evil
	uint8_t alignment;
	uint8_t* gender;
	uint8_t* race;
	uint32_t zoneID;
	uint32_t instanceID;

	CharacterCurrency currency;
	CharacterExperience experience;
	uint32_t flags;
	uint32_t flags2;
	uint64_t statusPoints;
	uint32_t currentLanguage;
	uint32_t houseZoneId;
	uint32_t bindZone;
	vec3 bindLocation;
	float bindHeading;
	std::string biography;
	std::string petName;

	uint32_t advArchetype;
	uint32_t advBaseClass;
	uint32_t advClass;
	uint32_t tsArchetype;
	uint32_t tsBaseClass;
	uint32_t tsClass;

	uint32_t characterID;
	uint32_t deityID;
};

class CharacterUpdateGenerator {
public:
	CharacterUpdateGenerator() = default;
	~CharacterUpdateGenerator() = default;

	void LinkUpdateFields(const CharacterSheet& sheet);
	bool GenerateUpdate(std::ostringstream& out);

private:
	void LinkCharacterFields(const CharacterSheet& sheet);
	void LinkCharacterDetailsFields(const CharacterSheet& sheet);

	DatabaseRow characterUpdates;
	DatabaseRow characterDetailsUpdates;
};

class CharacterSheet : public CharacterDetails {
public:
	CharacterSheet(const std::shared_ptr<Entity>& playerGhost) {
		LinkToGhost(playerGhost);
	}

	void LinkToGhost(const std::shared_ptr<Entity>& playerGhost) {
		CharacterDetails::LinkToGhost(playerGhost);
	}

	CharacterUpdateGenerator updates;
};