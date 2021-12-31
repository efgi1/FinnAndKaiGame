#pragma once

#include "ISystem.h"
#include <iostream>
#include "GameEngine.h"


class TestSystem : public ISystem
{
public:
	TestSystem(Scene* scene) : ISystem(scene) { pausable = true; }

	void operator()() override;
};