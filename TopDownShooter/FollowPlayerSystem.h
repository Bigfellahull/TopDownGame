#pragma once

#include "System.h"
#include "EntityManager.h"
#include "TranslationComponent.h"
#include "FollowPlayerComponent.h"

class SystemFollowPlayer : public System
{
public:
	SystemFollowPlayer(EntityManager& manager) :
		System(manager, false)
	{
		std::set<ComponentType> requiredComponents;
		requiredComponents.insert(FollowPlayerComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity)
	{
		FollowPlayerComponent& follow = m_manager.GetComponentStore<FollowPlayerComponent>().Get(entity);
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
				
		TranslationComponent& playerTranslation = m_manager.GetComponentStore<TranslationComponent>().Get(follow.playerEntity);

		DirectX::SimpleMath::Vector2 delta = playerTranslation.position - translation.position;

		if (delta.Length() > 50.0f)
		{
			delta.Normalize();

			float accLength = delta.LengthSquared();
			if (accLength > 1.0f)
			{
				delta *= (1.0f / sqrt(accLength));
			}

			translation.acceleration = (delta * follow.movementSpeed) + (translation.velocity * -follow.drag);
		}
		else
		{
			translation.acceleration = translation.velocity * -follow.drag;
		}
	}
};