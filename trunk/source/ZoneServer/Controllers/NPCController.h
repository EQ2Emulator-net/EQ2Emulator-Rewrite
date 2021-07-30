#pragma once

#include "BaseController.h"

// Temp until ai is worked out
#include "../AI/NPCMovement.h"

class NPCController : public BaseController {
public:
	NPCController() { }

	bool IsNPCController() override { return true; }

	bool Process() override;
	void Possess() override {};
	void UnPossess() override {};

	// temp until ai is worked out
	NPCMovement* GetNPCMovement() { return &movement; }
	void AddMovementLocation(std::shared_ptr<MovementLocationInfo> loc) { movement.AddLocation(loc); }

protected:
	// temp for now until actual ai is worked out
	NPCMovement movement;

private:

};