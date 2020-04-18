#pragma once

#include "BaseController.h"

class PlayerController : public BaseController {
public:
	PlayerController() = default;

	bool Process() override { return true; };
	void Possess() override {};
	void UnPossess() override {};

private:
	
};