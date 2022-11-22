#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include "ISystem.h"
#include "Entity.h"

class ISystem;

class Scene
{
public:
  Scene() : m_paused(false) {}

  void update();

  template <typename T>
  void addSystem()
  {
	  m_systems.push_back(std::make_unique<T>(this));
  }

  entt::entity getPlayer() { return m_player; }

protected:
  std::vector<std::unique_ptr<ISystem>> m_systems;
  bool m_paused;
  entt::entity m_player;
  
};

