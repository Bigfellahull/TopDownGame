#include "stdafx.h"

#include "AvoidanceSystem.h"
#include "AvoidanceComponent.h"
#include "AvoidableComponent.h"
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

void SystemAvoidance::UpdateEntity(float dt, float totalTime, Entity entity)
{
	if (!m_manager.GetComponentStore<EnemyComponent>().Get(entity).alive)
	{
		return;
	}

	AvoidanceComponent& avoidance = m_manager.GetComponentStore<AvoidanceComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	
	const float maxLookAhead = 250.0f;

	// Recalculate ahead vectors
	Vector2 normalisedVelocity = translation.velocity;
	normalisedVelocity.Normalize();

	Vector2 ahead = normalisedVelocity * maxLookAhead;

	avoidance.ahead[0] = MathHelper::RotatePoint(translation.position + ahead, translation.position, -25) - translation.position;
	avoidance.ahead[1] = ahead;
	avoidance.ahead[2] = MathHelper::RotatePoint(translation.position + ahead, translation.position, 25) - translation.position;

	TranslationComponent* mostThreateningObstacle = nullptr;

#if _DEBUG
	avoidance.debugProjectVector.clear();
	avoidance.debugProjectedVector.clear();
#endif

	std::vector<Entity> nearTranslationEntities = m_manager.GetQuadTree()->Retrieve(translation.position);
	for (auto e : nearTranslationEntities)
	{
		ComponentStore<AvoidableComponent>& advoidables = m_manager.GetComponentStore<AvoidableComponent>();
		if (!advoidables.Has(e))
		{
			continue;
		}

		ColliderComponent& otherCollider = m_manager.GetComponentStore<ColliderComponent>().Get(e);
		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e);

		Vector2 projectVector = otherTranslation.position - translation.position;

#if _DEBUG
		avoidance.debugProjectVector.push_back(projectVector);
#endif		

		bool rayHit = false;
		for (int i = 0; i < 3; i++)
		{
			Vector2 normalisedAhead = avoidance.ahead[i];
			normalisedAhead.Normalize();

			Vector2 projectedVector = normalisedAhead * (avoidance.ahead[i].Dot(projectVector) / avoidance.ahead[i].Length());

#if _DEBUG
			avoidance.debugProjectedVector.push_back(projectedVector * (maxLookAhead / projectedVector.Length()));
#endif

			if (projectedVector.LengthSquared() > std::pow(maxLookAhead, 2))
			{
				continue;
			}

			if (Vector2::DistanceSquared(otherTranslation.position, translation.position + projectedVector) <= std::pow(otherCollider.avoidanceRadius, 2))
			{
				rayHit = true;
				break;
			}
		}

		if (rayHit || Vector2::DistanceSquared(otherTranslation.position, translation.position) <= std::pow(otherCollider.avoidanceRadius, 2))
		{
			if (!mostThreateningObstacle || 
				(Vector2::DistanceSquared(translation.position, otherTranslation.position) < Vector2::DistanceSquared(translation.position, mostThreateningObstacle->position)))
			{
				mostThreateningObstacle = &otherTranslation;
			}
		}
	}

	if (mostThreateningObstacle)
	{
		Vector2 obstacleFuturePosition = mostThreateningObstacle->position + mostThreateningObstacle->velocity * 0.3f;

		Vector2 avoidanceForce = translation.position - mostThreateningObstacle->position;
		avoidanceForce.Normalize();

		float normalisedLength = avoidanceForce.LengthSquared();
		if (normalisedLength > 1.0f)
		{
			avoidanceForce *= (1.0f / sqrt(normalisedLength));
		}

		translation.acceleration += (avoidanceForce * 8000.0f);
	}
}
