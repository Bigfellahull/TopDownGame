#include "stdafx.h"

#include "Manager.h"
#include <algorithm>

Manager::Manager() :
	m_lastEntity(InvalidEntity),
	m_entities(),
	m_componentStores(),
	m_systems() { }

Manager::~Manager() { }

void Manager::AddSystem(const System::Ptr& systemPtr) 
{
	if ((!systemPtr) || (systemPtr->GetRequiredComponents().empty())) 
	{
		throw std::runtime_error("System needs required Components");
	}
	// Simply copy the pointer (instead of moving it) to allow for multiple insertion of the same shared pointer.
	m_systems.push_back(systemPtr);
}

size_t Manager::RegisterEntity(const Entity entity)
{
	size_t associatedSystems = 0;

	auto foundEntity = m_entities.find(entity);
	if (m_entities.end() == foundEntity) 
	{
		throw std::runtime_error("The Entity does not exist");
	}

	auto entityComponents = (*foundEntity).second;

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system)
	{
		auto systemRequiredComponents = (*system)->GetRequiredComponents();
		// Check if all Components Required by the System are in the Entity (use sorted sets)
		if (std::includes(entityComponents.begin(), entityComponents.end(),
			systemRequiredComponents.begin(), systemRequiredComponents.end()))
		{
			// Register the matching Entity
			// TODO shall throw in case of failure!
			(*system)->RegisterEntity(entity);
			++associatedSystems;
		}
	}

	return associatedSystems;
}

size_t Manager::UnregisterEntity(const Entity entity) {
	size_t associatedSystems = 0;

	auto foundEntity = m_entities.find(entity);
	if (m_entities.end() == foundEntity) 
	{
		throw std::runtime_error("The Entity does not exist");
	}

	auto entityComponents = (*foundEntity).second;

	for (auto system = m_systems.begin();
		system != m_systems.end();
		++system)
	{
		associatedSystems += (*system)->UnregisterEntity(entity);
	}

	return associatedSystems;
}

size_t Manager::UpdateEntities(DX::StepTimer const& timer) 
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

		updatedEntities += (*system)->UpdateEntities(timer);
	}

	return updatedEntities;
}

size_t Manager::RenderEntities()
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
