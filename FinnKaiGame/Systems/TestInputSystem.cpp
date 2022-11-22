#include "TestInputSystem.h"
#include "GameEngine.h"
#include "AnimationComponent.h"
#include "imgui_impl_sdl.h"

void TestInputSystem::operator()()
{
    auto player = m_scene->getPlayer();
    auto view = GameEngine::instance()->entityManager()->view<std::string, CTransform, CAnimation>();
    SDL_Event event;

    int x, y;
    SDL_GetMouseState(&x, &y);
    for (auto [entity, tag, transform, animation] : view.each())
    {
        auto size = GameEngine::instance()->assetManager()->getTexture(animation.textureName)->getSize();
        x -= size[0] / 8;
        y -= size[1] / 2;
        if (transform.pos.x < x)
            transform.velocity.x = MAX_PLAYER_HORIZ_SPEED;
        else if (transform.pos.x > x)
            transform.velocity.x = -MAX_PLAYER_HORIZ_SPEED;
        if (transform.pos.y < y)
            transform.velocity.y = MAX_PLAYER_VERT_SPEED;
        else if (transform.pos.y > y)
            transform.velocity.y = -MAX_PLAYER_VERT_SPEED;
        while (SDL_PollEvent(&event) > 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
            case SDL_QUIT:
                GameEngine::instance()->quit();
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE: GameEngine::instance()->quit(); break;
                case SDLK_s:
                case SDLK_DOWN:
                    transform.velocity.y = MAX_PLAYER_VERT_SPEED; break;
                case SDLK_w:
                case SDLK_UP:
                    transform.velocity.y = -MAX_PLAYER_VERT_SPEED; break;
                case SDLK_a:
                case SDLK_LEFT:
                    if (animation.animation.getName() == "kai_standing")
                    {
                        animation.animation = *GameEngine::instance()->assetManager()->getAnimation("kai_running");
                        animation.animation.setFlip(SDL_FLIP_HORIZONTAL);
                    }
                    transform.velocity.x = -MAX_PLAYER_HORIZ_SPEED; break;
                case SDLK_d:
                case SDLK_RIGHT:
                    if (animation.animation.getName() == "kai_standing")
                        animation.animation = *GameEngine::instance()->assetManager()->getAnimation("kai_running");
                    transform.velocity.x = MAX_PLAYER_HORIZ_SPEED;
                    break;
                case SDLK_SPACE:
                    transform.velocity.y = -MAX_PLAYER_VERT_SPEED * 10;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_d:
                case SDLK_RIGHT:
                    animation.animation = *GameEngine::instance()->assetManager()->getAnimation("kai_standing");
                    transform.velocity.x = 0;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                {
                    animation.animation = *GameEngine::instance()->assetManager()->getAnimation("kai_standing");
                    animation.animation.setFlip(SDL_FLIP_HORIZONTAL);
                    transform.velocity.x = 0;
                    break;
                }
                case SDLK_w:
                case SDLK_s:
                case SDLK_UP:
                case SDLK_DOWN:
                    transform.velocity.y = 0; break;
                }
                break;
            }
        }
    }
}
