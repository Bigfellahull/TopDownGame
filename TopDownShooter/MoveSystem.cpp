#include "stdafx.h"

#include "MoveSystem.h"
#include "TranslationComponent.h"
#include "RegionComponent.h"

using namespace DirectX::SimpleMath;

SystemMove::SystemMove(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemMove::UpdateEntity(float dt, float totalTime, Entity entity)
{
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

    Vector2 delta = (0.5f * translation.acceleration * std::pow(dt, 2)) + (translation.velocity * dt);
    translation.position += delta;
    
    translation.velocity += translation.acceleration * dt;
    if (translation.velocity.LengthSquared() < 0.1f)
    {
        translation.velocity = Vector2::Zero;
    }

    // TODO: Fix this assumption of one region always existing.
    const RegionComponent& region = m_manager.GetComponentStore<RegionComponent>().GetComponents().begin()->second;
    translation.position.Clamp(region.min, region.max);

    if (translation.velocity.LengthSquared() > 0.0f)
    {
        translation.orientation = static_cast<float>(std::atan2(translation.velocity.y, translation.velocity.x));
    }
}