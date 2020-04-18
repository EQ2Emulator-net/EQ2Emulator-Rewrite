#include "stdafx.h"

#include "PlayerController.h"
#include "../Spawns/Spawn.h"

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