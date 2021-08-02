#pragma once

#include <memory>

class Spawn;

class BaseController : public std::enable_shared_from_this<BaseController> {
public:
	virtual ~BaseController() = default;
	virtual bool Process() = 0;
	virtual void Possess(std::shared_ptr<Spawn> spawn);
	virtual void UnPossess() { }

	virtual std::shared_ptr<Spawn> GetControlled() { return controlled.lock(); }
	virtual void ClearTarget(bool bUpdateClient = true);
	virtual void SetTarget(const std::shared_ptr<class Spawn>& spawn, bool bUpdateClient = true);
	std::shared_ptr<class Spawn> GetTarget();

	virtual bool IsNPCController() { return false; }

protected:
	std::weak_ptr<Spawn> controlled;
	std::weak_ptr<Spawn> target;
};