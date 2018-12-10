#include "stdafx.h"

#include "SeparationSystem.h"
#include "SeparationComponent.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "EnemyComponent.h"

using namespace DirectX::SimpleMath;

SystemSeparation::SystemSeparation(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(SeparationComponent::Type);
	requiredComponents.insert(ColliderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(EnemyComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemSeparation::UpdateEntity(float dt, Entity entity)
{
	if (!m_manager.GetComponentStore<EnemyComponent>().Get(entity).alive)
	{
		return;
	}

	ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

	Vector2 separationForce = Vector2::Zero;
	int neighbourCount = 0;

	ComponentStore<SeparationComponent>& separationComponents = m_manager.GetComponentStore<SeparationComponent>();

	SeparationComponent& separator = separationComponents.Get(entity);

#if _DEBUG
	separator.debugProjectVector.clear();
#endif

	std::vector<Entity> nearTranslationEntities = m_manager.GetQuadTree()->Retrieve(translation.position);
	for (auto e : nearTranslationEntities)
	{
		if (e == entity)
		{
			continue;
		}

		if (!separationComponents.Has(e))
		{
			continue;
		}

		ColliderComponent& otherCollider = m_manager.GetComponentStore<ColliderComponent>().Get(e);
		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e);

#if _DEBUG
		separator.debugProjectVector.push_back(otherTranslation.position - translation.position);
#endif

		float radius = collider.avoidanceRadius + otherCollider.avoidanceRadius;
		if (Vector2::DistanceSquared(otherTranslation.position, translation.position) <= std::pow(radius, 2))
		{
			separationForce += otherTranslation.position - translation.position;
			neighbourCount++;
		}
	}

	if (neighbourCount > 0)
	{
		separationForce /= static_cast<float>(neighbourCount);
		separationForce *= -1.0f;
	}

	separationForce.Normalize();

	translation.acceleration += (separationForce * 1000.0f);
}
