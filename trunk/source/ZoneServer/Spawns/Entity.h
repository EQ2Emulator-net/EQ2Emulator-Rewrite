#pragma once

#include "Spawn.h"

struct EntityAttribute {
	inline void Initialize(int32_t val) { baseValue = currentValue = val; }
	int32_t baseValue;
	int32_t currentValue;
};

struct EntityFloatAttribute {
	inline void Initialize(float val) { baseValue = currentValue = val; }
	float baseValue;
	float currentValue;
};

struct ExpendableAttribute : EntityAttribute {
	inline void Initialize(int32_t val) { EntityAttribute::Initialize(val); maxValue = val; }
	int32_t maxValue;
};

struct EntityAttributeSheet {
	EntityAttributeSheet() {
		memset(this, 0, sizeof(*this));
	}

	uint32_t DatabaseCrc(uint32_t crc);

	//Expendables
	ExpendableAttribute hp;
	ExpendableAttribute power;
	ExpendableAttribute savagery;
	ExpendableAttribute dissonance;
	ExpendableAttribute concentration;

	//Attributes
	EntityAttribute intel;
	EntityAttribute str;
	EntityAttribute sta;
	EntityAttribute agi;
	EntityAttribute wis;
	EntityAttribute mitigation;
	EntityAttribute avoidance;

	//Blue attributes
	EntityFloatAttribute critMitigation;
	EntityFloatAttribute critChance;
	EntityFloatAttribute critBonus;
	EntityFloatAttribute multiAttackChance;
	EntityFloatAttribute flurryChance;
	EntityFloatAttribute aeAttackChance;
	EntityFloatAttribute potency;
	EntityFloatAttribute attackSpeed;

	//Resists
	//EntityAttribute heat;
	//EntityAttribute cold;
	//EntityAttribute magic;
	//EntityAttribute mental;
	//EntityAttribute divine;
	//EntityAttribute disease;
	//EntityAttribute poison;
	EntityAttribute elemental;
	EntityAttribute arcane;
	EntityAttribute noxious;
};

class Entity : public Spawn {
public:
	Entity() { };
	Entity(std::shared_ptr<Entity> in) : Spawn(in) {
		SetSwimmingSpeedMultiplier(0.78125f, false);
	}

	EntityAttributeSheet attributes;
};