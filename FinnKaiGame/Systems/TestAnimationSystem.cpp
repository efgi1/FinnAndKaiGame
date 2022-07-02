#include "TestAnimationSystem.h"
#include "..\Core\GameEngine.h"

void TestAnimationSystem::operator()()
{
	for (auto entity : GameEngine::instance()->entityManager()->getEntities())
	{
		//TODO animate based on state
	}
}
