#pragma once

#include <memory>
#include <map>

class Character;

class CharacterList {
public:
	CharacterList() = default;
	~CharacterList() = default;

	void AddCharacter(const std::shared_ptr<Character>& character);
	std::shared_ptr<Character> GetCharacterByID(uint32_t id);
	std::shared_ptr<Character> GetCharacterBySessionID(uint32_t session);
	
private:
	std::map<uint32_t, std::shared_ptr<Character> > charMap;
};