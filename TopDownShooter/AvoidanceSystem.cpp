#include "stdafx.h"

#include "AvoidanceSystem.h"
#include "AvoidanceComponent.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "PlayerComponent.h"

#include "RenderComponent.h"

using namespace DirectX::SimpleMath;

SystemAvoidance::SystemAvoidance(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(AvoidanceComponent::Type);
	requiredComponents.insert(ColliderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemAvoidance::UpdateEntity(float dt, Entity entity)
{
	AvoidanceComponent& avoidance = m_manager.GetComponentStore<AvoidanceComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	
	const float maxLookAhead = 300.0f;

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

	const std::unordered_map<Entity, ColliderComponent>& otherColliders = m_manager.GetComponentStore<ColliderComponent>().GetComponents();
	for (auto e : otherColliders)
	{
		if (e.first == entity || m_manager.GetComponentStore<PlayerComponent>().Has(e.first))
		{
			continue;
		}

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

		if (Vector2::DistanceSquared(otherTranslation.position, translation.position + projectedVector) <= std::pow(e.second.avoidanceRadius, 2) ||
			Vector2::DistanceSquared(otherTranslation.position, translation.position) <= std::pow(e.second.avoidanceRadius, 2))
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
		avoidance.avoiding = true;

		Vector2 avoidanceForce = avoidance.ahead - mostThreateningObstancle->position;
		avoidanceForce.Normalize();

		translation.acceleration += (avoidanceForce * 8000.0f);
	}
	else
	{
		avoidance.avoiding = false;
	}
}