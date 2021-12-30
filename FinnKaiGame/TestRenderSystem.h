#pragma once
#include "ISystem.h"
#include "GameEngine.h"

class TestRenderSystem :
    public ISystem
{
public:
  TestRenderSystem() : ISystem() {  }

  void operator()() override;
};

