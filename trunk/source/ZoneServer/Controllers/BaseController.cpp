#include "stdafx.h"

#include "BaseController.h"
#include "../Spawns/Spawn.h"

void BaseController::Possess(std::shared_ptr<Spawn> spawn) {
	controlled = spawn;
}

void BaseController::SetTarget(const std::shared_ptr<Spawn>& spawn, bool bUpdateClient) {
	target = spawn;
}

std::shared_ptr<Spawn> BaseController::GetTarget() {
	return target.lock();
}

void BaseController::ClearTarget(bool bUpdateClient) {
	target.reset();
}