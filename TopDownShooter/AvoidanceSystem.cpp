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
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
    ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);

	Vector2 normalisedVelocity = translation.velocity;
	normalisedVelocity.Normalize();
    Vector2 ahead = translation.position + (normalisedVelocity * 150.0f);
	Vector2 ahead2 = ahead * 0.5f;

	TranslationComponent* mostThreateningObstancle = nullptr;

	const std::unordered_map<Entity, ColliderComponent>& otherColliders = m_manager.GetComponentStore<ColliderComponent>().GetComponents();
	for (auto e : otherColliders)
	{
		if (e.first == entity || m_manager.GetComponentStore<PlayerComponent>().Has(e.first))
		{
			continue;
		}

        float radius = collider.radius + e.second.radius;
		TranslationComponent& otherTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(e.first);
		if ((Vector2::DistanceSquared(otherTranslation.position, ahead) <= radius * radius) ||
			(Vector2::DistanceSquared(otherTranslation.position, ahead2) <= radius * radius) ||
            (Vector2::DistanceSquared(otherTranslation.position, translation.position) <= radius * radius))
		{
			if (!mostThreateningObstancle || 
				(Vector2::DistanceSquared(translation.position, otherTranslation.position) < Vector2::DistanceSquared(translation.position, mostThreateningObstancle->position)))
			{
				mostThreateningObstancle = &otherTranslation;
			}
		}
	}

	if (mostThreateningObstancle)
	{
		Vector2 avoidanceForce = ahead - mostThreateningObstancle->position;
		avoidanceForce.Normalize();

		translation.acceleration = (avoidanceForce * 10000.0f) + (translation.velocity);
	}
}