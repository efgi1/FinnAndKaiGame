#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "entt/entt.hpp"

#include "..\Common\Entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
public:
	EntityManager() : nextId(0) {}

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);

private:
	entt::registry m_registry;
	EntityVec m_entities;
	EntityVec m_toAdd;
	std::map<std::string, EntityVec> m_entityMap;
	size_t nextId;
};

