#pragma once
#include "ISystem.h"
#include "GameEngine.h"

class TestMovementSystem :
  public ISystem
{
public:
  TestMovementSystem() : ISystem() {}
  void operator()() override {
    auto entities = GameEngine::instance()->entityManager()->getEntities();
    for (auto& entity : entities)
    {
      auto& transform = entity->getComponent<CTransform>();
      transform.prevPos = transform.pos;
      transform.pos += transform.velocity;
      if (transform.pos.y > GameEngine::instance()->WINDOW_HEIGHT || transform.pos.y < 0)
      {
        transform.velocity.y *= -1;
        transform.pos.y = GameEngine::instance()->WINDOW_HEIGHT;
      }
      if (transform.pos.x > GameEngine::instance()->WINDOW_WIDTH || transform.pos.x < 0)
      {
        transform.velocity.x *= -1;
        transform.pos.x = GameEngine::instance()->WINDOW_WIDTH;
      }
    }
  }
};

