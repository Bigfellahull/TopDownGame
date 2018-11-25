#pragma once

#include "System.h"
#include "EntityManager.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"

class SystemCollider : public System
{
public:
	SystemCollider(EntityManager& manager) :
		System(manager, false)
	{
		std::set<ComponentType> requiredComponents;
		requiredComponents.insert(ColliderComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);
		
		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(float dt, Entity entity)
	{
		ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

		const std::unordered_map<Entity, ColliderComponent>& otherColliders = m_manager.GetComponentStore<ColliderComponent>().GetComponents();
		for (auto e : otherColliders)
		{
			ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();

			if (e.first == entity || !translationComponents.Has(e.first))
			{
				// Don't collide with self!
				continue;
			}

			TranslationComponent& otherTranslation = translationComponents.Get(e.first);
			float radius = collider.radius + e.second.radius;
			if (DirectX::SimpleMath::Vector2::DistanceSquared(translation.position, otherTranslation.position) < radius * radius)
			{
				m_manager.QueueEntityForDrop(entity);
				m_manager.QueueEntityForDrop(e.first);
			}
		}
	}
};