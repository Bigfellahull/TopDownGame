#pragma once

#include "System.h"
#include "Manager.h"
#include "PositionComponent.h"

class SystemMove : public System 
{
public:
	SystemMove(Manager& manager) :
		System(manager, false) 
	{
		ComponentTypeSet requiredComponents;
		requiredComponents.insert(PositionComponent::Type);
		
		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity) 
	{
		PositionComponent& position = m_manager.GetComponentStore<PositionComponent>().Get(entity);

		position.x += 3.0f * static_cast<float>(timer.GetElapsedSeconds());
		position.y += 3.0f * static_cast<float>(timer.GetElapsedSeconds());
	}
};