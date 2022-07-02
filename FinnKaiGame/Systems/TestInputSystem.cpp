#include "TestInputSystem.h"
#include "..\Core\GameEngine.h"
#include "..\Components\AnimationComponent.h"

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
                case SDLK_DOWN: player->getComponent<CTransform>().velocity.y = MAX_PLAYER_VERT_SPEED; break;
                case SDLK_UP: player->getComponent<CTransform>().velocity.y = -MAX_PLAYER_VERT_SPEED; break;
                case SDLK_LEFT: 
                    if (player->getComponent<CAnimation>().animation.getName() == "kai_standing")
                    {
                        player->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_running"), false);
                        player->getComponent<CAnimation>().animation.setFlip(SDL_FLIP_HORIZONTAL);
                    }
                    player->getComponent<CTransform>().velocity.x = -MAX_PLAYER_HORIZ_SPEED; break;
                case SDLK_RIGHT: 
                    if (player->getComponent<CAnimation>().animation.getName() == "kai_standing")
                        player->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_running"), false); 
                    player->getComponent<CTransform>().velocity.x = MAX_PLAYER_HORIZ_SPEED;
                    break;
                case SDLK_SPACE:
                    player->getComponent<CTransform>().velocity.y = -MAX_PLAYER_VERT_SPEED * 10;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:
                    player->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_standing"), false);
                    player->getComponent<CTransform>().velocity.x = 0;
                    break;
                case SDLK_LEFT:
                {
                    player->addComponent<CAnimation>(*GameEngine::instance()->assetManager()->getAnimation("kai_standing"), false);
                    player->getComponent<CAnimation>().animation.setFlip(SDL_FLIP_HORIZONTAL);
                    player->getComponent<CTransform>().velocity.x = 0;
                    break;
                }
                case SDLK_UP: case SDLK_DOWN: player->getComponent<CTransform>().velocity.y = 0; break;
            }
            break;
        }
    }
}
