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

void SystemCollider::UpdateEntity(float dt, float totalTime, Entity entity)
{
	ComponentStore<EnemyComponent>& enemyComponents = m_manager.GetComponentStore<EnemyComponent>();
	ComponentStore<PlayerComponent>& playerComponents = m_manager.GetComponentStore<PlayerComponent>();
	ComponentStore<ColliderComponent>& colliderComponents = m_manager.GetComponentStore<ColliderComponent>();
	ComponentStore<TranslationComponent>& translationComponents = m_manager.GetComponentStore<TranslationComponent>();

	if ((enemyComponents.Has(entity) && !enemyComponents.Get(entity).alive) ||
		(playerComponents.Has(entity) && !playerComponents.Get(entity).status->IsAlive()))
	{
		return;
	}

    ColliderComponent& collider = colliderComponents.Get(entity);
    TranslationComponent& translation = translationComponents.Get(entity);

	std::vector<Entity> nearTranslationEntities = m_manager.GetQuadTree()->Retrieve(translation.position);
    for (auto e : nearTranslationEntities)
    {		
		if (!colliderComponents.Has(e) || e == entity || !translationComponents.Has(e))
		{
			continue;
		}

		if ((enemyComponents.Has(e) && !enemyComponents.Get(e).alive) ||
			(playerComponents.Has(e) && !playerComponents.Get(e).status->IsAlive()))
		{
			continue;
		}

		ColliderComponent& otherCollider = colliderComponents.Get(e);
        TranslationComponent& otherTranslation = translationComponents.Get(e);

        float radius = collider.radius + otherCollider.radius;
        if (Vector2::DistanceSquared(translation.position, otherTranslation.position) < std::pow(radius, 2))
        {
			collider.hasCollided = true;
			collider.collidedWith = e;
        }
    }
}