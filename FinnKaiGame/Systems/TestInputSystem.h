#pragma once
#include "ISystem.h"
class TestInputSystem :
    public ISystem
{
public:
  TestInputSystem(Scene* scene) : ISystem(scene) {}
  void operator()() override;
  const float MAX_PLAYER_HORIZ_SPEED = 0.05f;
  const float MAX_PLAYER_VERT_SPEED = 0.05f;
};

