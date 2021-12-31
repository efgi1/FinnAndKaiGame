#include "TestInputSystem.h"
#include "GameEngine.h"
#include "AnimationComponent.h"

void TestInputSystem::operator()()
{
    auto player = m_scene->getPlayer();
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
                case SDLK_DOWN: player->getComponent<CTransform>().velocity.y = 2; break;
                case SDLK_UP: player->getComponent<CTransform>().velocity.y = -2; break;
                case SDLK_LEFT: player->getComponent<CTransform>().velocity.x = -2; break;
                case SDLK_RIGHT: player->getComponent<CTransform>().velocity.x = 2; break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:case SDLK_LEFT: player->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_standing"), false); player->getComponent<CTransform>().velocity.x = 0; break;
                case SDLK_UP: case SDLK_DOWN: player->getComponent<CTransform>().velocity.y = 0; break;
            }
            break;
        }
    }
}
