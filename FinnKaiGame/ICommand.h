#pragma once
#include "Entity.h"

class ICommand
{
public:
	virtual void execute(Entity& entity) = 0;
};

