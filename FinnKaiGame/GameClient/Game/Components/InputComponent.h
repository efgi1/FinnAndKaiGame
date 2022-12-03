#pragma once
#pragma once
#include <glm/vec2.hpp>
#include "IComponent.h"

class CInput : public IComponent
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool canShoot = true;

    CInput() {}

};