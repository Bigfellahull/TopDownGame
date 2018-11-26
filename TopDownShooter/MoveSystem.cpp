#include "stdafx.h"

#include "MoveSystem.h"
#include "TranslationComponent.h"
#include "RegionComponent.h"

SystemMove::SystemMove(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemMove::UpdateEntity(float dt, Entity entity)
{
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

    DirectX::SimpleMath::Vector2 delta = (0.5f * translation.acceleration * std::pow(dt, 2)) + (translation.velocity * dt);
    translation.position += delta;
    translation.velocity += translation.acceleration * dt;

    // Is there a better way? Store pointer on component?
    const RegionComponent& region = m_manager.GetComponentStore<RegionComponent>().GetComponents().begin()->second;
    translation.position.Clamp(region.min, region.max);

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