#include "stdafx.h"

#include "AvoidanceSystem.h"
#include "AvoidanceComponent.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "ProjectileComponent.h"
#include "EnemyComponent.h"

using namespace DirectX::SimpleMath;

SystemAvoidance::SystemAvoidance(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(AvoidanceComponent::Type);
	requiredComponents.insert(ColliderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(EnemyComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemAvoidance::UpdateEntity(float dt, Entity entity)
{
	EnemyComponent& enemy = m_manager.GetComponentStore<EnemyComponent>().Get(entity);

	if (!enemy.alive)
	{
		return;
	}

	AvoidanceComponent& avoidance = m_manager.GetComponentStore<AvoidanceComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	
	const float maxLookAhead = 350.0f;

	// Recalculate ahead vectors
	Vector2 normalisedVelocity = translation.velocity;
	normalisedVelocity.Normalize();
	avoidance.ahead = normalisedVelocity * maxLookAhead;

	Vector2 normalisedAhead = avoidance.ahead;
	normalisedAhead.Normalize();

	TranslationComponent* mostThreateningObstancle = nullptr;

#if _DEBUG
	avoidance.debugProjectVector.clear();
	avoidance.debugProjectedVector.clear();
#endif

	// We only avoid projectiles for now...
	const std::unordered_map<Entity, ProjectileComponent>& projectiles = m_manager.GetComponentStore<ProjectileComponent>().GetComponents();
	for (auto e : projectiles)
	{
		ColliderComponent& otherCollider = m_manager.GetComponentStore<ColliderComponent>().Get(e.first);
		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e.first);
				
		Vector2 projectVector = otherTranslation.position - translation.position;
		Vector2 projectedVector = normalisedAhead * (avoidance.ahead.Dot(projectVector) / avoidance.ahead.Length());

		if (projectedVector.Length() > maxLookAhead)
		{
#if _DEBUG
			projectedVector *= (maxLookAhead / projectedVector.Length());
#endif
			continue;
		}

		if (Vector2::DistanceSquared(otherTranslation.position, translation.position + projectedVector) <= std::pow(otherCollider.avoidanceRadius, 2) ||
			Vector2::DistanceSquared(otherTranslation.position, translation.position) <= std::pow(otherCollider.avoidanceRadius, 2))
		{
			if (!mostThreateningObstancle ||
				(Vector2::DistanceSquared(translation.position, otherTranslation.position) < Vector2::DistanceSquared(translation.position, mostThreateningObstancle->position)))
			{
				mostThreateningObstancle = &otherTranslation;
			}
		}

#if _DEBUG
		avoidance.debugProjectVector.push_back(projectVector);
		avoidance.debugProjectedVector.push_back(projectedVector);
#endif
	}

	if (mostThreateningObstancle)
	{
		Vector2 avoidanceForce = avoidance.ahead - mostThreateningObstancle->position;
		avoidanceForce.Normalize();

		translation.acceleration += (avoidanceForce * 1000.0f);
	}
}
