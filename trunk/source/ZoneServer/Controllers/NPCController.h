#pragma once

#include "BaseController.h"

class NPCController : public BaseController {
public:
	NPCController() { }

	bool IsNPCController() override { return true; }

	bool Process() override { return true; };
	void Possess() override {};
	void UnPossess() override {};

private:

};