#pragma once

#include <tuple>
#include <string>
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "InputComponent.h"
#include "GravityComponent.h"
#include "entt/entt.hpp"

class EntityManager;

typedef std::tuple<
	CTransform,
	CAnimation,
	CInput,
	CGravity
> ComponentTuple;

class Entity
{
	friend class EntityManager;

public:
	void destroy() { m_alive = false; }
	size_t id() const { return m_id; }
	bool alive() const { return m_alive; }
	const std::string& tag() const { return m_tag; }

  template <typename T>
  bool hasComponent() const
  {
    return std::get<T>().has;
  }

  template <typename T, typename... TArgs>
  void addComponent(TArgs&&... mArgs)
  {
	GameEngine::instance()->entityManager()->emplace<T>(*this, std::forward<TArgs>(mArgs)...);
    auto& component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.implemented = true;
  }

  template<typename T>
  T& getComponent()
  {
    return std::get<T>(m_components);
  }

  template<typename T>
  void removeComponent()
  {
    getComponent<T>() = T();
  }

private:
	bool m_alive;
	std::string m_tag;
	size_t m_id;
  ComponentTuple m_components;

	Entity(const size_t& id, const std::string& tag) : m_id(id), m_tag(tag), m_alive(true) {}
};

