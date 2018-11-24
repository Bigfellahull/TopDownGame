#pragma once

#include "Entity.h"
#include "Component.h"
#include "StepTimer.h"
#include <set>
#include <memory>

class Manager;

class System {
public:
	typedef std::shared_ptr<System> Ptr;

	explicit System(Manager& manager, bool isRender);

	virtual ~System();

	inline const ComponentTypeSet& GetRequiredComponents() const 
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
	inline void SetRequiredComponents(ComponentTypeSet&& requiredComponents) 
	{
		m_requiredComponents = std::move(requiredComponents);
	}

	Manager& m_manager;

private:
	bool m_isRender;
	ComponentTypeSet m_requiredComponents;
	std::set<Entity> m_matchingEntities;
};
