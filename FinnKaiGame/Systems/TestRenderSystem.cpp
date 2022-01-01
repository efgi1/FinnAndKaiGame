#include "TestRenderSystem.h"

void TestRenderSystem::operator()()
{
	auto window = GameEngine::instance()->window();
	auto surface = SDL_GetWindowSurface(window);
	auto renderer = SDL_GetRenderer(window);

	SDL_RenderClear(renderer);

	auto entities = GameEngine::instance()->entityManager()->getEntities();

	for (auto& entity : entities)
	{
		auto& animation = entity->getComponent<CAnimation>();
		auto transform = entity->getComponent<CTransform>();
		if (animation.implemented && transform.implemented)
		{
			auto spriteRect = animation.animation.getSprite();
			auto texture = GameEngine::instance()->assetManager()->getTexture("kai")->getSDLTexture();
			auto renderer = GameEngine::instance()->renderer();
			SDL_Rect renderRect;
			renderRect.w = static_cast<int>(spriteRect.w * transform.scale.x);
			renderRect.h = static_cast<int>(spriteRect.h * transform.scale.y);
			renderRect.x = static_cast<int>(transform.pos.x);
			renderRect.y = static_cast<int>(transform.pos.y);
			
			SDL_RenderCopyEx(renderer, texture, &spriteRect, &renderRect, 0, nullptr, animation.animation.getFlip());
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x0FF, 0xFF);
			entity->getComponent<CAnimation>().animation.update();
		}
	}
	SDL_RenderPresent(renderer);
	
}
