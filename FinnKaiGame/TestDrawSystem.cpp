#include "TestDrawSystem.h"

void TestDrawSystem::operator()()
{
	auto window = GameEngine::instance()->window();
	auto renderer = SDL_GetRenderer(window);
	
	auto entities = GameEngine::instance()->entityManager()->getEntities();

	for (auto& entity : entities)
	{
		SDL_Rect rect;
		rect.x = static_cast<int>(entity->getComponent<CTransform>().pos.x);
		rect.y = static_cast<int>(entity->getComponent<CTransform>().pos.y);
		rect.w = 10;
		rect.h = 10;

		auto& transform = entity->getComponent<CTransform>();
		transform.pos += transform.velocity;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
	SDL_RenderPresent(renderer);
	
}
