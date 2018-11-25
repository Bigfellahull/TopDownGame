#pragma once

#include "System.h"
#include "EntityManager.h"
#include "ProjectileComponent.h"
#include "RegionComponent.h"
#include "TranslationComponent.h"

class SystemProjectile : public System
{
public:
	SystemProjectile(EntityManager& manager) :
		System(manager, false)
	{
		std::set<ComponentType> requiredComponents;
		requiredComponents.insert(ProjectileComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity)
	{
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

		// Is there a better way? Store pointer on component?
		const RegionComponent& region = m_manager.GetComponentStore<RegionComponent>().GetComponents().begin()->second;

		if (translation.position.x <= region.min.x || translation.position.x >= region.max.x ||
			translation.position.y <= region.min.y || translation.position.y >= region.max.y)
		{
			m_manager.QueueEntityForDrop(entity);
			return;
		}
	}
};