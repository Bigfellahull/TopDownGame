#include "stdafx.h"

#include "ProjectileSourceSystem.h"
#include "ProjectileComponent.h"
#include "ProjectileSourceComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "AvoidableComponent.h"
#include "MathHelper.h"

using namespace DirectX::SimpleMath;

SystemProjectileSource::SystemProjectileSource(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(ProjectileSourceComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(RenderComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemProjectileSource::UpdateEntity(float dt, Entity entity)
{
    ProjectileSourceComponent& projectile = m_manager.GetComponentStore<ProjectileSourceComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
    RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

    projectile.aimDirection.Normalize();
    
    float aimLength = projectile.aimDirection.LengthSquared();
    if (aimLength > 1.0f)
    {
        projectile.aimDirection *= (1.0f / sqrt(aimLength));
    }

    if (projectile.aimDirection.LengthSquared() > 0 && projectile.cooldownRemaining <= 0)
    {
        projectile.cooldownRemaining = projectile.cooldownTime;

        float aimAngle = static_cast<float>(std::atan2(projectile.aimDirection.y, projectile.aimDirection.x));

        Quaternion aimQuat = Quaternion::CreateFromYawPitchRoll(0, 0, aimAngle);

        double randomSpread = (MathHelper::Random(0.0f, 1.0f) * 0.02) + (MathHelper::Random(0.0f, 1.0f) * 0.02);

        Vector2 velocity = 1000.0f *
            Vector2(static_cast<float>(std::cos(aimAngle + randomSpread)), static_cast<float>(std::sin(aimAngle + randomSpread)));

        Vector2 offset = Vector2::Transform(Vector2(40, -8), aimQuat);
        {
            Entity bullet = m_manager.CreateEntity();
            m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
            m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, projectile.assetManager->GetTexture(BulletAsset), render.spriteFont));
            m_manager.AddComponent(bullet, ProjectileComponent());
			m_manager.AddComponent(bullet, AvoidableComponent());
            m_manager.AddComponent(bullet, ColliderComponent(4.0f, 40.0f));
            m_manager.RegisterEntity(bullet);
        }

        offset = Vector2::Transform(Vector2(40, 8), aimQuat);
        {
            Entity bullet = m_manager.CreateEntity();
            m_manager.AddComponent(bullet, TranslationComponent(translation.position + offset, velocity, aimAngle));
            m_manager.AddComponent(bullet, RenderComponent(render.spriteBatch, projectile.assetManager->GetTexture(BulletAsset), render.spriteFont));
            m_manager.AddComponent(bullet, ProjectileComponent());
			m_manager.AddComponent(bullet, AvoidableComponent());
            m_manager.AddComponent(bullet, ColliderComponent(4.0f, 40.0f));
            m_manager.RegisterEntity(bullet);
        }
    }

    if (projectile.cooldownRemaining > 0)
    {
        projectile.cooldownRemaining -= dt;
    }
}