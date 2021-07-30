#include "stdafx.h"

#include "NPCController.h"

bool NPCController::Process() {
	std::shared_ptr<Entity> body = controlled.lock();
	if (body) {
		movement.Process(body);
	}
	return true;
}