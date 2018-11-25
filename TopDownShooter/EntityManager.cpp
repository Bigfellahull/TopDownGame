#include "stdafx.h"

#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager() :
	m_lastEntity(InvalidEntity),
	m_entities(),
	m_componentStores(),
	m_systems() { }

EntityManager::~EntityManager() { }

void EntityManager::AddSystem(const std::shared_ptr<System>& systemPtr)
{
	if ((!systemPtr) || (systemPtr->GetRequiredComponents().empty())) 
	{
		throw std::runtime_error("System needs required Components");
	}
	// Simply copy the pointer (instead of moving it) to allow for multiple insertion of the same shared pointer.
	m_systems.push_back(systemPtr);
}

size_t EntityManager::RegisterEntity(const Entity entity)
{
	size_t associatedSystems = 0;

	auto foundEntity = m_entities.find(entity);
	if (m_entities.end() == foundEntity) 
	{
		throw std::runtime_error("The Entity does not exist");
	}

	std::set<ComponentType> entityComponents = (*foundEntity).second;

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system)
	{
		std::set<ComponentType> systemRequiredComponents = (*system)->GetRequiredComponents();
		// Check if all Components Required by the System are in the Entity (use sorted sets)
		if (std::includes(entityComponents.begin(), entityComponents.end(),
			systemRequiredComponents.begin(), systemRequiredComponents.end()))
		{
			(*system)->RegisterEntity(entity);
			++associatedSystems;
		}
	}

	return associatedSystems;
}

size_t EntityManager::UnregisterEntity(const Entity entity) {
	size_t associatedSystems = 0;

	auto foundEntity = m_entities.find(entity);
	if (m_entities.end() == foundEntity) 
	{
		throw std::runtime_error("The Entity does not exist");
	}

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system)
	{
		associatedSystems += (*system)->UnregisterEntity(entity);
	}

	return associatedSystems;
}

size_t EntityManager::UpdateEntities(float dt)
{
	size_t updatedEntities = 0;

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system) 
	{
		if ((*system)->IsRender())
		{
			continue;
		}

		updatedEntities += (*system)->UpdateEntities(dt);
	}

	// It's possible systems have queued entities to be removed.
	// So now remove them.
	DropEntities();

	return updatedEntities;
}

size_t EntityManager::RenderEntities()
{
	size_t updatedEntities = 0;

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system) 
	{
		if (!(*system)->IsRender())
		{
			continue;
		}

		updatedEntities += (*system)->RenderEntities();
	}

	return updatedEntities;
}

// TODO: This seems to be adding multiple entries for the same entity.
// I am fixing now by changing to a map, but we need to investigate why this is happening.
void EntityManager::QueueEntityForDrop(const Entity entity)
{
	if (m_tempEntitiesToDrop.end() == m_tempEntitiesToDrop.find(entity))
	{
		m_tempEntitiesToDrop.insert(entity);
	}
}

void EntityManager::DropEntities()
{
	for (auto e : m_tempEntitiesToDrop)
	{
		UnregisterEntity(e);

		for (auto const& c : m_componentStores)
		{
			c.second->Remove(e);
		}

		m_entities.erase(e);
	}

	m_tempEntitiesToDrop.clear();
}
