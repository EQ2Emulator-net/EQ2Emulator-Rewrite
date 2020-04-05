#pragma once

#include <memory>

class Spawn;

class BaseController {
public:

	virtual bool Process() = 0;
	virtual void Possess() = 0;
	virtual void UnPossess() = 0;

	virtual std::shared_ptr<Spawn> GetControlled() { return controlled.lock(); }

protected:
	std::weak_ptr<Spawn> controlled;

};