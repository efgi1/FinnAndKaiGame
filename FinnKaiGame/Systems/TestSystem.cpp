#include "TestSystem.h"

void TestSystem::operator()()
{
	{
		auto& entities = GameEngine::instance()->entityManager()->getEntities();
		for (auto& entity : entities)
		{
			std::cout << "Enity Id: " << entity->id() << '\n';
			std::cout << "Transform: {x: " << entity->getComponent<CTransform>().pos.x << ", y: " << entity->getComponent<CTransform>().pos.y << '\n';
		}
	}
}
