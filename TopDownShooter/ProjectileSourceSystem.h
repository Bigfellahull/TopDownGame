#pragma once

#include "System.h"
#include "EntityManager.h"
#include "ProjectileSourceComponent.h"
#include "TranslationComponent.h"
#include "MathHelper.h"
#include "RenderComponent.h"

class SystemProjectileSource : public System
{
public:
	SystemProjectileSource(EntityManager& manager) :
		System(manager, false)
	{
		std::set<ComponentType> requiredComponents;
		requiredComponents.insert(ProjectileSourceComponent::Type);
		requiredComponents.insert(TranslationComponent::Type);

		SetRequiredComponents(std::move(requiredComponents));
	}

	virtual void UpdateEntity(float dt, Entity entity)
	{
		ProjectileSourceComponent& projectile = m_manager.GetComponentStore<ProjectileSourceComponent>().Get(entity);
		TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
		
		// For now, was assume that will we always be on a renderable entity.
		// We could change this to put a pointer to spritebatch on the projectile source component.
		RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

		if (projectile.aimDirection.LengthSquared() > 1)
		{
			projectile.aimDirection.Normalize();
		}

		float aimLength = projectile.aimDirection.LengthSquared();
		if (aimLength > 1.0f)
		{
			projectile.aimDirection *= (1.0f / sqrt(aimLength));
		}

		if (projectile.aimDirection.LengthSquared() > 0 && projectile.cooldownRemaining <= 0)
		{
			projectile.cooldownRemaining = projectile.cooldownTime;

			float aimAngle = static_cast<float>(std::atan2(projectile.aimDirection.y, projectile.aimDirection.x));
			DirectX::SimpleMath::Quaternion aimQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0, 0, aimAngle);

			auto randomSpread = (MathHelper::Random(0.0f, 1.0f) * 0.02) + (MathHelper::Random(0.0f, 1.0f) * 0.02);
			DirectX::SimpleMath::Vector2 velocity = 1000.0f * 
				DirectX::SimpleMath::Vector2(static_cast<float>(std::cos(aimAngle + randomSpread)), static_cast<float>(std::sin(aimAngle + randomSpread)));

			DirectX::SimpleMath::Vector2 offset = DirectX::SimpleMath::Vector2::Transform(DirectX::SimpleMath::Vector2(40, -8), aimQuat);
			{
				Entity bullet = m_manager.CreateEntity();
				m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
				m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, projectile.assetManager->GetTexture(BulletAsset)));
				m_manager.AddComponent(bullet, ProjectileComponent());
				m_manager.AddComponent(bullet, ColliderComponent(2.0f));
				m_manager.RegisterEntity(bullet);
			}
			offset = DirectX::SimpleMath::Vector2::Transform(DirectX::SimpleMath::Vector2(40, 8), aimQuat);
			{
				Entity bullet = m_manager.CreateEntity();
				m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
				m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, projectile.assetManager->GetTexture(BulletAsset)));
				m_manager.AddComponent(bullet, ProjectileComponent());
				m_manager.AddComponent(bullet, ColliderComponent(2.0f));
				m_manager.RegisterEntity(bullet);
			}
		}

		if (projectile.cooldownRemaining > 0)
		{
			projectile.cooldownRemaining -= dt;
		}
	}
};