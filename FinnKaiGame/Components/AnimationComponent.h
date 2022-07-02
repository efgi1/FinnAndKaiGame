#pragma once
#include "IComponent.h"
#include "..\Common\Animation.h"
class CAnimation :
    public IComponent
{
public:
  Animation animation;
  bool repeat = false;
  CAnimation() {}
  CAnimation(const Animation& animation, bool r)
    : animation(animation), repeat(r) {
  }
};

