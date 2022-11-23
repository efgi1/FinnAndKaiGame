#pragma once
#include "Scene.h"
#include "TestSystem.h"

class TestScene :
    public Scene
{
public:
  TestScene();

  const float kGravity = 0.0005;
};

