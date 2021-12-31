#include "TestInputSystem.h"
#include "GameEngine.h"
#include "AnimationComponent.h"

void TestInputSystem::operator()()
{
    auto entity = m_scene->m_player;
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            GameEngine::instance()->quit();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE: GameEngine::instance()->quit(); break;
                case SDLK_DOWN: entity->getComponent<CTransform>().velocity.y = 2; break;
                case SDLK_UP: entity->getComponent<CTransform>().velocity.y = -2; break;
                case SDLK_LEFT: entity->getComponent<CTransform>().velocity.x = -2; break;
                case SDLK_RIGHT: entity->getComponent<CTransform>().velocity.x = 2; break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:case SDLK_LEFT: entity->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_standing"), false); entity->getComponent<CTransform>().velocity.x = 0; break;
                case SDLK_UP: case SDLK_DOWN: entity->getComponent<CTransform>().velocity.y = 0; break;
            }
            break;
        }
    }
}
