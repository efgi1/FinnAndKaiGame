#pragma once
#include "..\Common\ISystem.h"
#include "..\Core\GameEngine.h"

class TestRenderSystem :
    public ISystem
{
public:
  TestRenderSystem(Scene* scene) : ISystem(scene) {  }

  void operator()() override;
};

