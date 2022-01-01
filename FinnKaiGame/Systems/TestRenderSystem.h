#pragma once
#include "ISystem.h"
#include "GameEngine.h"

class TestRenderSystem :
    public ISystem
{
public:
  TestRenderSystem(Scene* scene) : ISystem(scene) {  }

  void operator()() override;
};

