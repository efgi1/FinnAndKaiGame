#include "TestRenderSystem.h"

void TestRenderSystem::operator()()
{
	auto window = GameEngine::instance()->window();
	auto renderer = SDL_GetRenderer(window);
	
	auto entities = GameEngine::instance()->entityManager()->getEntities();

	for (auto& entity : entities)
	{
		auto animation = entity->getComponent<CAnimation>();
		auto transform = entity->getComponent<CTransform>();
		if (animation.implemented && transform.implemented)
		{
			auto spriteRect = entity->getComponent<CAnimation>().animation.getSprite();
			auto texture = GameEngine::instance()->assetManager()->getTexture("kai");
			auto renderer = GameEngine::instance()->renderer();
			SDL_Rect renderRect;
			renderRect.w = static_cast<int>(spriteRect.w * transform.scale.x);
			renderRect.h = static_cast<int>(spriteRect.h * transform.scale.y);
			renderRect.x = static_cast<int>(transform.pos.x);
			renderRect.y = static_cast<int>(transform.pos.y);
			SDL_RenderCopy(renderer, texture, &spriteRect, &renderRect);
			entity->getComponent<CAnimation>().animation.update();
		}
	}
	SDL_RenderPresent(renderer);
	
}
