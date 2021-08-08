#pragma once

#include "BaseController.h"

// Temp until ai is worked out
#include "../AI/NPCMovement.h"

class NPCController : public BaseController {
public:
	NPCController() { }

	bool IsNPCController() override { return true; }

	bool Process() override;

	// temp until ai is worked out
	NPCMovement* GetNPCMovement() { return &movement; }
	void AddMovementLocation(std::shared_ptr<MovementLocationInfo> loc) { movement.AddLocation(loc); }
	void UpdateMovementTimestamp() { movement.UpdateMovementTimestamp(); }

	void SetFollowTarget(std::shared_ptr<Spawn> spawn) { movement.SetFollowTarget(spawn); }
	void ClearFollowTarget() { movement.ClearFollowTarget(); }

protected:
	// temp for now until actual ai is worked out
	NPCMovement movement;

private:

};