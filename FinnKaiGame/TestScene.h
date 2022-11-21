#pragma once
#include "Common\Scene.h"
#include "Systems\TestSystem.h"

class TestScene :
    public Scene
{
public:
  TestScene();

  const float kGravity = 0.0005;
};

