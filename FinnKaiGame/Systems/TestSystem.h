#pragma once

#include "..\Common\ISystem.h"
#include <iostream>
#include "..\Core\GameEngine.h"


class TestSystem : public ISystem
{
public:
	TestSystem(Scene* scene) : ISystem(scene) { pausable = true; }

	void operator()() override;
};