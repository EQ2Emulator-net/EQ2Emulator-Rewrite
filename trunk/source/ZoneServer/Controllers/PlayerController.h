#pragma once

#include "BaseController.h"
#include "../Players/CharacterSheet.h"
#include <optional>

class PlayerController : public BaseController {
public:
	PlayerController() :predictionCRC(0) {}

	bool Process() override { return true; };
	void Possess() override {};
	void UnPossess() override {};
	
	bool CheckPredictionCrc(uint32_t crc);
	void ApplyPredictionUpdate(uint32_t deltaMS, const struct SpawnPositionStruct& update);
	void SetControlled(const std::shared_ptr<Entity>& spawn) override;
	CharacterSheet* GetCharacterSheet() { return charSheet ? &charSheet.value() : nullptr; }
	void MoveCharacterSheetFrom(CharacterSheet* rhs) { charSheet.emplace(std::move(*rhs)); }

private:
	uint32_t predictionCRC;
	std::optional<CharacterSheet> charSheet;
};