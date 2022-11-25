#include "TestSystem.h"
#include "TransformComponent.h"

void TestSystem::operator()()
{
	{
		auto view = GameEngine::instance()->entityManager()->view<CTransform>();
		for (auto [entity, transform] : view.each())
		{
			//std::cout << "Transform: {x: " << transform.pos.x << ", y: " << transform.pos.y << '\n';
		}
	}
}
