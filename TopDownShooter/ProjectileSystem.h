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

		// Can this be improved?
		ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);
		const std::unordered_map<Entity, FollowPlayerComponent>& enemies = m_manager.GetComponentStore<FollowPlayerComponent>().GetComponents();
		for (auto e : enemies)
		{
			TranslationComponent& enemyTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e.first);
			ColliderComponent& enemyCollider = m_manager.GetComponentStore<ColliderComponent>().Get(e.first);
			
			float radius = collider.radius + enemyCollider.radius;
			if (DirectX::SimpleMath::Vector2::DistanceSquared(enemyTranslation.position, translation.position) < radius * radius)
			{
				m_manager.QueueEntityForDrop(entity);
				m_manager.QueueEntityForDrop(e.first);
			}
		}
	}
};