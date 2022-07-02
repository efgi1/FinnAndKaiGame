#pragma once
#include "..\Common\ISystem.h"
class TestAnimationSystem :
    public ISystem
{
public:
  void operator()() override;
};

