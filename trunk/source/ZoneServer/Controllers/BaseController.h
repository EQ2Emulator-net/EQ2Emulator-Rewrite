#pragma once

#include <memory>

// Temp until ai is worked out
#include "../AI/NPCMovement.h"

class Spawn;
class Entity;

class BaseController : public std::enable_shared_from_this<BaseController> {
public:
	virtual ~BaseController() = default;
	virtual bool Process() = 0;
	virtual void Possess() = 0;
	virtual void UnPossess() = 0;

	virtual std::shared_ptr<Entity> GetControlled() { return controlled.lock(); }
	virtual void SetControlled(const std::shared_ptr<Entity>& spawn) { controlled = spawn; }
	virtual void ClearTarget(bool bUpdateClient = true);
	virtual void SetTarget(const std::shared_ptr<class Spawn>& spawn, bool bUpdateClient = true);
	std::shared_ptr<class Spawn> GetTarget();

	virtual bool IsNPCController() { return false; }

	// temp until ai is worked out
	NPCMovement* GetNPCMovement() { return &movement; }
	void AddMovementLocation(std::shared_ptr<MovementLocationInfo> loc) { movement.AddLocation(loc); }

protected:
	std::weak_ptr<Entity> controlled;
	std::weak_ptr<Spawn> target;

	// temp for now until actual ai is worked out
	NPCMovement movement;
};