#include "TestAnimationSystem.h"
#include "GameEngine.h"

void TestAnimationSystem::operator()()
{
	for (auto entity : GameEngine::instance()->entityManager()->getEntities())
	{
		//TODO animate based on state
	}
}
