#pragma once

#include "BaseController.h"

class PlayerController : public BaseController {
public:
	PlayerController() :predictionCRC(0) {}

	bool Process() override { return true; };
	void Possess() override {};
	void UnPossess() override {};
	
	bool CheckPredictionCrc(uint32_t crc);
	void ApplyPredictionUpdate(uint32_t deltaMS, const struct SpawnPositionStruct& update);

private:
	uint32_t predictionCRC;
};