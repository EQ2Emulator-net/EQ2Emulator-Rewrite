#pragma once

#include <memory>

class Spawn;
class Entity;

class BaseController : public std::enable_shared_from_this<BaseController> {
public:
	virtual ~BaseController() = default;
	virtual bool Process() = 0;
	virtual void Possess() = 0;
	virtual void UnPossess() = 0;

	virtual std::shared_ptr<Entity> GetControlled() { return controlled.lock(); }
	void SetControlled(const std::shared_ptr<Entity>& spawn) { controlled = spawn; }
	virtual void ClearTarget(bool bUpdateClient = true);
	virtual void SetTarget(const std::shared_ptr<class Spawn>& spawn, bool bUpdateClient = true);
	std::shared_ptr<class Spawn> GetTarget();

protected:
	std::weak_ptr<Entity> controlled;
	std::weak_ptr<Spawn> target;
};