#include "stdafx.h"

#include "PlayerController.h"
#include "../Spawns/Entity.h"
#include "../Players/NPCPathDebug.h"
#include "../Players/SpawnCampDebug.h"

bool PlayerController::Process() {
	bool ret = true;

	if (m_npcPathDebug) {
		ret = m_npcPathDebug->Process();
	}

	if (m_spawnCampDebug)
		ret = m_spawnCampDebug->Process();

	return ret;
}

void PlayerController::Possess(std::shared_ptr<Spawn> spawn) {
	BaseController::Possess(spawn);

	std::shared_ptr<Entity> ent = std::dynamic_pointer_cast<Entity>(spawn);
	if (!ent)
		return;

	if (!charSheet) {
		charSheet.emplace(ent);
	}
	else {
		charSheet->LinkToGhost(ent);
	}
}

bool PlayerController::CheckPredictionCrc(uint32_t crc) { 
	bool ret = predictionCRC != crc;
	predictionCRC = crc;
	return ret;
}

void PlayerController::ApplyPredictionUpdate(uint32_t deltaMS, const SpawnPositionStruct& update) {
	auto controlled = GetControlled();
	if (!controlled) {
		return;
	}

	controlled->SetSpawnPositionData(update, deltaMS);
}