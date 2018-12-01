#include "stdafx.h"

#include "ColliderSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

using namespace DirectX::SimpleMath;

SystemCollider::SystemCollider(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(ColliderComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemCollider::UpdateEntity(float dt, Entity entity)
{
	ComponentStore<EnemyComponent>& enemies = m_manager.GetComponentStore<EnemyComponent>();
	ComponentStore<PlayerComponent>& players = m_manager.GetComponentStore<PlayerComponent>();

	if ((enemies.Has(entity) && !enemies.Get(entity).alive) || 
		(players.Has(entity) && !players.Get(entity).status->isAlive))
	{
		return;
	}

    ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

    const std::unordered_map<Entity, ColliderComponent>& otherColliders = m_manager.GetComponentStore<ColliderComponent>().GetComponents();
    for (auto e : otherColliders)
    {
        ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();
		
		if (e.first == entity || !translationComponents.Has(e.first))
        {
            continue;
        }

		if ((enemies.Has(e.first) && !enemies.Get(e.first).alive) ||
			(players.Has(e.first) && !players.Get(e.first).status->isAlive))
		{
			continue;
		}

        TranslationComponent& otherTranslation = translationComponents.Get(e.first);

        float radius = collider.radius + e.second.radius;
        if (Vector2::DistanceSquared(translation.position, otherTranslation.position) < std::pow(radius, 2))
        {
            if (players.Has(entity))
            {
                PlayerComponent& player = players.Get(entity);
                player.status->isAlive = false;
            }

            m_manager.QueueEntityForDrop(entity);
        }
    }
}