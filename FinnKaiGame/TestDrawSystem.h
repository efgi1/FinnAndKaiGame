#pragma once
#include "ISystem.h"
#include "GameEngine.h"

class TestDrawSystem :
    public ISystem
{
public:
  TestDrawSystem() : ISystem() {  }

  void operator()() override;
};

