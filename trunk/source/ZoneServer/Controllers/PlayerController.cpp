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

void PlayerController::SetControlled(const std::shared_ptr<Entity>& spawn) {
	BaseController::SetControlled(spawn);

	if (!charSheet) {
		charSheet.emplace(spawn);
	}
	else {
		charSheet->LinkToGhost(spawn);
	}
}