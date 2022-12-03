#pragma once
#include <glm/vec2.hpp>
#include "IComponent.h"

class CGravity : public IComponent
{
public:
    float gravity;

    CGravity() : gravity(0.f) {}
    CGravity(const float& gravity)
        : gravity(gravity) {}

};