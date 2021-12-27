#pragma once

#include "ISystem.h"
#include <iostream>
#include "GameEngine.h"


class TestSystem : public ISystem
{
public:
	TestSystem() : ISystem() { pausable = true; }

	void operator()() override;
};