#pragma once
#include "ISystem.h"
class TestInputSystem :
    public ISystem
{
public:
  TestInputSystem(Scene* scene) : ISystem(scene) {}
  void operator()() override;
};

