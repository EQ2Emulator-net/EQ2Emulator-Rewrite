#pragma once

#include "BaseController.h"
#include "../Players/CharacterSheet.h"
#include <optional>

class NPCPathDebug;
class SpawnCampDebug;

class PlayerController : public BaseController {
public:
	PlayerController() :predictionCRC(0) {}

	bool Process() override;
	void Possess(std::shared_ptr<Spawn> spawn) override;

	bool CheckPredictionCrc(uint32_t crc);
	void ApplyPredictionUpdate(uint32_t deltaMS, const struct SpawnPositionStruct& update);
	CharacterSheet* GetCharacterSheet() { return charSheet ? &charSheet.value() : nullptr; }
	void MoveCharacterSheetFrom(CharacterSheet* rhs) { charSheet.emplace(std::move(*rhs)); }

	std::shared_ptr<NPCPathDebug> GetNPCPathDebug() { return m_npcPathDebug; }
	void SetNPCPathDebug(std::shared_ptr<NPCPathDebug> path) { m_npcPathDebug = path; }

	std::shared_ptr<SpawnCampDebug> GetSpawnCampDebug() { return m_spawnCampDebug; }
	void SetSpawnCampDebug(std::shared_ptr<SpawnCampDebug> camp) { m_spawnCampDebug = camp; }

private:
	uint32_t predictionCRC;
	std::optional<CharacterSheet> charSheet;
	std::shared_ptr<NPCPathDebug> m_npcPathDebug;
	std::shared_ptr<SpawnCampDebug> m_spawnCampDebug;
};