#include "..\Core\EntityManager.h"

void EntityManager::update()
{
	auto newEnd = std::remove_if(m_entities.begin(), m_entities.end(), [](auto& entity) { return !entity->alive(); });
	m_entities.erase(newEnd, m_entities.end());

	for (auto& iter : m_entityMap) {
		auto newEnd = std::remove_if(iter.second.begin(), iter.second.end(), [](auto& entity) { return !entity->alive(); });
		iter.second.erase(newEnd, iter.second.end());
	}

	m_entities.insert(m_entities.end(), m_toAdd.begin(), m_toAdd.end());
	m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(nextId++, tag));
	m_entities.push_back(entity);
	m_entityMap[tag].push_back(entity);
	return entity;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}
