#pragma once
#include "ISystem.h"
class TestAnimationSystem :
    public ISystem
{
public:
  void operator()() override;
};

