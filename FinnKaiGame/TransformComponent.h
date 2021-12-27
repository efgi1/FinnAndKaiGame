#pragma once
#include <glm/vec2.hpp>
#include "IComponent.h"

class CTransform : public IComponent
{
public:
  glm::vec2 pos = { 0.0, 0.0 };
  glm::vec2 prevPos = { 0.0, 0.0 };
  glm::vec2 scale = { 1.0, 1.0 };
  glm::vec2 velocity = { 0.0, 0.0 };
  float angle = 0;

  CTransform() {}
  CTransform(const glm::vec2& p)
    : pos(p) {}
  CTransform(const glm::vec2& p, const glm::vec2& v, const glm::vec2& sc, float a)
    : pos(p), prevPos(p), velocity(v), scale(sc), angle(a) {}

};