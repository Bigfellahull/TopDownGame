#pragma once

#include "System.h"
#include "EntityManager.h"
#include "TranslationComponent.h"

class SystemMove : public System 
{
public:
	SystemMove(EntityManager& manager) :
		System(manager, false) 
	{
		ComponentTypeSet requiredComponents;
		requiredComponents.insert(TranslationComponent::Type);
		
		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity) 
	{
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
		
		float dt = static_cast<float>(timer.GetElapsedSeconds());		
		
		DirectX::SimpleMath::Vector2 delta = (0.5f * translation.acceleration * std::pow(dt, 2)) + (translation.velocity * dt);
		translation.position += delta;
		translation.velocity += translation.acceleration * dt;

		/*if (translation.acceleration.LengthSquared() > 1)
		{
			translation.acceleration.Normalize();
		}
		translation.velocity = 8.0f * translation.acceleration;
		translation.position += translation.velocity;*/

		if (translation.velocity.LengthSquared() > 0)
		{
			translation.orientation = static_cast<float>(std::atan2(translation.velocity.y, translation.velocity.x));
		}
	}
};