#pragma once
#include "IComponent.h"
#include "Animation.h"
class CAnimation :
    public IComponent
{
public:
  Animation animation;
  bool repeat = false;
  std::string textureName;
  CAnimation() {}
  CAnimation(std::string textureName, const Animation& animation, bool r)
    : textureName(textureName), animation(animation), repeat(r) {
  }
};

