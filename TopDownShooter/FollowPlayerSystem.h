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

		// This might be a hack job. Look to improve?
		// Will need to fix when additional enemies are added.
		if (follow.timeToStart > 0.0f)
		{
			follow.timeToStart--;

			ComponentStore<RenderComponent>& renderComponents = m_manager.GetComponentStore<RenderComponent>();
			if (renderComponents.Has(entity))
			{
				RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
				render.colour = DirectX::Colors::White * (1.0f - static_cast<float>(follow.timeToStart) / 60.0f);
				render.scale = 1.6f - (follow.timeToStart / 60.f);
			}
		}
		else
		{
			if (follow.timeToStart > -10.0f)
			{
				ComponentStore<RenderComponent>& renderComponents = m_manager.GetComponentStore<RenderComponent>();
				if (renderComponents.Has(entity))
				{
					RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
					render.colour = DirectX::Colors::White;
					render.scale = 1.0f;
				}
				follow.timeToStart = -20.0f;
			}			

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
	}
};