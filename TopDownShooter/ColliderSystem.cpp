#include "stdafx.h"

#include "ColliderSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "DestructableComponent.h"

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

	std::vector<Entity> nearTranslationEntities = m_manager.GetQuadTree()->Retrieve(translation.position);
    for (auto e : nearTranslationEntities)
    {
		ComponentStore<ColliderComponent>& colliderComponents = m_manager.GetComponentStore<ColliderComponent>();
		if (!colliderComponents.Has(e))
		{
			continue;
		}

        ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();
		if (e == entity || !translationComponents.Has(e))
        {
            continue;
        }

		if ((enemies.Has(e) && !enemies.Get(e).alive) ||
			(players.Has(e) && !players.Get(e).status->isAlive))
		{
			continue;
		}

		ColliderComponent& otherCollider = colliderComponents.Get(e);
        TranslationComponent& otherTranslation = translationComponents.Get(e);

        float radius = collider.radius + otherCollider.radius;
        if (Vector2::DistanceSquared(translation.position, otherTranslation.position) < std::pow(radius, 2))
        {
			// TODO: Move
            if (players.Has(entity))
            {
                PlayerComponent& player = players.Get(entity);
                player.status->isAlive = false;

				ComponentStore<DestructableComponent>& destructableComponents = m_manager.GetComponentStore<DestructableComponent>();
				for (auto& x : destructableComponents.GetComponents())
				{
					destructableComponents.Get(x.first).destroy = true;
				}
            }

			collider.hasCollided = true;
			collider.collidedWith = e;
        }
    }
}