#include "stdafx.h"

#include "System.h"
#include "EntityManager.h"

System::System(EntityManager& manager, bool isRender) :
	m_manager(manager), 
	m_isRender(isRender) { }

System::~System() { }

size_t System::UpdateEntities(float dt) 
{
	size_t updatedEntities = 0;

	for (auto entity = m_matchingEntities.begin();
		entity != m_matchingEntities.end();
		++entity) 
	{
		UpdateEntity(dt, *entity);
		++updatedEntities;
	}

	return updatedEntities;
}

size_t System::RenderEntities()
{
	size_t updatedEntities = 0;

	for (auto entity = m_matchingEntities.begin();
		entity != m_matchingEntities.end();
		++entity)
	{
		RenderEntity(*entity);
		++updatedEntities;
	}

	return updatedEntities;
}
