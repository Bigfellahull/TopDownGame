#pragma once

#include "System.h"
#include "EntityManager.h"
#include "ProjectileComponent.h"
#include "TranslationComponent.h"
#include "MathHelper.h"
#include "RenderComponent.h"

class SystemProjectile : public System
{
public:
	SystemProjectile(EntityManager& manager) :
		System(manager, false)
	{
		ComponentTypeSet requiredComponents;
		requiredComponents.insert(ProjectileComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(DX::StepTimer const& timer, Entity entity)
	{
		ProjectileComponent& projectile = m_manager.GetComponentStore<ProjectileComponent>().Get(entity);
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
		
		// HACK FOR TEMP BULLETS
		RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

		float dt = static_cast<float>(timer.GetElapsedSeconds());

		if (projectile.aimDirection.LengthSquared() > 1)
		{
			projectile.aimDirection.Normalize();
		}

		if (projectile.aimDirection.LengthSquared() > 0 && projectile.cooldownRemaining <= 0)
		{
			projectile.cooldownRemaining = projectile.cooldownFrames;

			float aimAngle = static_cast<float>(std::atan2(projectile.aimDirection.y, projectile.aimDirection.x));
			DirectX::SimpleMath::Quaternion aimQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0, 0, aimAngle);

			auto randomSpread = (MathHelper::Random(0.0f, 1.0f) * 0.02) + (MathHelper::Random(0.0f, 1.0f) * 0.02);
			DirectX::SimpleMath::Vector2 velocity = 2000.0f * 
				DirectX::SimpleMath::Vector2(static_cast<float>(std::cos(aimAngle + randomSpread)), static_cast<float>(std::sin(aimAngle + randomSpread)));

			DirectX::SimpleMath::Vector2 offset = DirectX::SimpleMath::Vector2::Transform(DirectX::SimpleMath::Vector2(40, -8), aimQuat);
			{
				Entity bullet = m_manager.CreateEntity();
				m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
				m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, render.texture, render.resource));
				m_manager.RegisterEntity(bullet);
			}
			offset = DirectX::SimpleMath::Vector2::Transform(DirectX::SimpleMath::Vector2(40, 8), aimQuat);
			{
				Entity bullet = m_manager.CreateEntity();
				m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
				m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, render.texture, render.resource));
				m_manager.RegisterEntity(bullet);
			}
		}

		if (projectile.cooldownRemaining > 0)
		{
			projectile.cooldownRemaining--;
		}
	}
};