#include "TestSystem.h"

void TestSystem::operator()()
{
	{
		auto& entities = GameEngine::instance()->entityManager()->getEntities();
		for (auto& entity : entities)
		{
			std::cout << "Enity Id: " << entity->id() << '\n';
			std::cout << "Enity Id: " << entity->getComponent<CTransform>().pos.x << '\n';
		}
	}
}
