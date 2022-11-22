#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "entt/entt.hpp"

#include "Entity.h"


typedef std::vector<entt::entity> EntityVec;

class EntityManager : public entt::registry
{
};

