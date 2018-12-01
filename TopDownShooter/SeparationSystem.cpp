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

	const std::unordered_map<Entity, SeparationComponent>& otherSeparates = m_manager.GetComponentStore<SeparationComponent>().GetComponents();
	for (auto e : otherSeparates)
	{
		if (e.first == entity)
		{
			continue;
		}
		
		ColliderComponent& otherCollider = m_manager.GetComponentStore<ColliderComponent>().Get(e.first);
		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e.first);

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
