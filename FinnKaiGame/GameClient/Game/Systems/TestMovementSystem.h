#pragma once
#include "ISystem.h"
#include "GameEngine.h"

class TestMovementSystem :
  public ISystem
{
public:
  TestMovementSystem(Scene* scene) : ISystem(scene) {}
  void operator()() override {

    auto view = GameEngine::instance()->entityManager()->view<CTransform>();
    for (auto [entity, transform] : view.each())
    {
      transform.prevPos = transform.pos;
      transform.pos += transform.velocity;
      //auto& gravity = entity->getComponent<CGravity>();

      if (transform.pos.y > GameEngine::instance()->WINDOW_HEIGHT || transform.pos.y < 0)
      {
        transform.velocity.y *= -1;
        transform.pos.y = static_cast<float>(GameEngine::instance()->WINDOW_HEIGHT);
      }

      if (transform.pos.x > GameEngine::instance()->WINDOW_WIDTH || transform.pos.x < 0)
      {
        transform.velocity.x *= -1;
        transform.pos.x = static_cast<float>(GameEngine::instance()->WINDOW_WIDTH);
      }

      //TODO physics system: transform.velocity.y += gravity.gravity;
    }
  }
};

