#pragma once
#include "ISystem.h"
class TestInputSystem :
    public ISystem
{
public:
  TestInputSystem() : ISystem() {}
  void operator()() override;
};

