#pragma once

#include "BaseController.h"
#include "../Players/CharacterSheet.h"
#include <optional>

class NPCPathDebug;

class PlayerController : public BaseController {
public:
	PlayerController() :predictionCRC(0) {}

	bool Process() override;
	void Possess() override {};
	void UnPossess() override {};
	
	bool CheckPredictionCrc(uint32_t crc);
	void ApplyPredictionUpdate(uint32_t deltaMS, const struct SpawnPositionStruct& update);
	void SetControlled(const std::shared_ptr<Entity>& spawn) override;
	CharacterSheet* GetCharacterSheet() { return charSheet ? &charSheet.value() : nullptr; }
	void MoveCharacterSheetFrom(CharacterSheet* rhs) { charSheet.emplace(std::move(*rhs)); }

	std::shared_ptr<NPCPathDebug> GetNPCPathDebug() { return m_npcPathDebug; }
	void SetNPCPathDebug(std::shared_ptr<NPCPathDebug> path) { m_npcPathDebug = path; }

private:
	uint32_t predictionCRC;
	std::optional<CharacterSheet> charSheet;
	std::shared_ptr<NPCPathDebug> m_npcPathDebug;
};