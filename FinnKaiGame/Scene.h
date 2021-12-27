#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include "ISystem.h"
#include "GameEngine.h"

class GameEngine;

class Scene
{
public:
  Scene() : m_paused(false) {}

  void update() {
    for (auto& system : m_systems)
    {
      if (!m_paused || !system->pausable)
        (*system)();
    }
  }

  void addSystem(std::unique_ptr<ISystem> system)
  {
    m_systems.push_back(std::move(system));
  }

protected:
  std::vector<std::unique_ptr<ISystem>> m_systems;
  bool m_paused;
};

