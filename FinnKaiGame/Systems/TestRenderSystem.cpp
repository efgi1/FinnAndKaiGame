#include "TestRenderSystem.h"

void TestRenderSystem::operator()()
{
	auto window = GameEngine::instance()->window();
	auto surface = SDL_GetWindowSurface(window);
	auto renderer = GameEngine::instance()->renderer();
	if (window == nullptr || renderer == nullptr)
		return;

	SDL_RenderClear(renderer);

	auto view = GameEngine::instance()->entityManager()->view<CTransform, CAnimation>();

	for (auto [entity, transform, animation] : view.each())
	{
		auto spriteRect = animation.animation.getSprite();
		auto texture = GameEngine::instance()->assetManager()->getTexture(animation.textureName)->getSDLTexture();
		auto renderer = GameEngine::instance()->renderer();
		SDL_Rect renderRect;
		renderRect.w = static_cast<int>(spriteRect.w * transform.scale.x);
		renderRect.h = static_cast<int>(spriteRect.h * transform.scale.y);
		renderRect.x = static_cast<int>(transform.pos.x);
		renderRect.y = static_cast<int>(transform.pos.y);

		SDL_RenderCopyEx(renderer, texture, &spriteRect, &renderRect, 0, nullptr, animation.animation.getFlip());
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x000, 0xFF);
		animation.animation.update();
	}
	
}
