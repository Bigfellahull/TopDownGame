#pragma once

#include "Entity.h"
#include "Component.h"
#include "StepTimer.h"
#include <set>
#include <memory>

class EntityManager;

class System {
public:
	explicit System(EntityManager& manager, bool isRender);

	virtual ~System();

	inline const std::set<ComponentType>& GetRequiredComponents() const
	{
		return m_requiredComponents;
	}

	inline bool RegisterEntity(Entity entity) 
	{
		return m_matchingEntities.insert(entity).second;
	}

	inline size_t UnregisterEntity(Entity entity) 
	{
		return m_matchingEntities.erase(entity);
	}

	inline bool HasEntity(Entity entity) const 
	{
		return (m_matchingEntities.end() != m_matchingEntities.find(entity));
	}

	size_t UpdateEntities(DX::StepTimer const& timer);

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity) { }

	size_t RenderEntities();

	virtual void RenderEntity(Entity entity) { }

	bool IsRender() { return m_isRender; }

protected:
	inline void SetRequiredComponents(std::set<ComponentType>&& requiredComponents)
	{
		m_requiredComponents = std::move(requiredComponents);
	}

	EntityManager& m_manager;

private:
	bool m_isRender;
	std::set<ComponentType> m_requiredComponents;
	std::set<Entity> m_matchingEntities;
};
