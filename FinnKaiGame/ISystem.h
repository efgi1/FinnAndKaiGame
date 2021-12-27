#pragma once
#include "EntityManager.h"
#include <functional>

class ISystem {
public:
	ISystem() : pausable(false) {}
	bool pausable;
	virtual void operator()() = 0;
};