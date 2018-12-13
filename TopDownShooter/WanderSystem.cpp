#include "stdafx.h"

#include "WanderSystem.h"
#include "TranslationComponent.h"
#include "WanderComponent.h"
#include "EnemyComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemWander::SystemWander(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(WanderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(EnemyComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemWander::UpdateEntity(float dt, float totalTime, Entity entity)
{
	WanderComponent& wander = m_manager.GetComponentStore<WanderComponent>().Get(entity);

	if (!m_manager.GetComponentStore<EnemyComponent>().Get(entity).alive)
	{
		return;
	}

	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

	Vector2 circleCentre = translation.velocity;
	circleCentre.Normalize();
	circleCentre *= 20.0f;

	Vector2 displacement{ 0, -1 };
	displacement *= 500.0f;

	float displacementLength = displacement.Length();
	displacement.x = std::cosf(wander.wanderAngle) * displacementLength;
	displacement.y = std::sinf(wander.wanderAngle) * displacementLength;

	wander.wanderAngle += MathHelper::Random(0.0f, 1.0f) * 2.0f - 2.0f * 0.5f;

	Vector2 delta = circleCentre + displacement;

	if (delta.LengthSquared() > std::pow(50.0f, 2))
	{
		delta.Normalize();

		float normalisedLength = delta.LengthSquared();
		if (normalisedLength > 1.0f)
		{
			delta *= (1.0f / sqrt(normalisedLength));
		}

		translation.acceleration = (delta * wander.movementSpeed) + (translation.velocity * -wander.drag);
	}
	else
	{
		translation.acceleration = translation.velocity * -wander.drag;
	}

	if (translation.acceleration.LengthSquared() < 0.1f)
	{
		translation.acceleration = Vector2::Zero;
	}
}