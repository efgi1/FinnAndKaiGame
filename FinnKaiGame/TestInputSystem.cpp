#include "TestInputSystem.h"
#include "GameEngine.h"

void TestInputSystem::operator()()
{
  SDL_Event event;
  while (SDL_PollEvent(&event) > 0)
  {
    auto entities = GameEngine::instance()->entityManager()->getEntities("test");
    switch (event.type)
    {
    case SDL_QUIT:
      GameEngine::instance()->quit();
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        GameEngine::instance()->quit();
        break;
      case SDLK_DOWN:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.y = 2;
        }
        break;
      case SDLK_UP:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.y = -2;
        }
        break;
      case SDLK_LEFT:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.x = -2;
        }
        break;
      case SDLK_RIGHT:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.x = 2;
        }
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym)
      {
      case SDLK_RIGHT:
      case SDLK_LEFT:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.x = 0;
        }
        break;
      case SDLK_UP:
      case SDLK_DOWN:
        for (auto& entity : entities)
        {
          entity->getComponent<CTransform>().velocity.y = 0;
        }
        break;
      }
      break;
    }
  }
}
