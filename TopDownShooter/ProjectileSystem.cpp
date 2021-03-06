#include "stdafx.h"

#include "ProjectileSystem.h"
#include "ProjectileComponent.h"
#include "TranslationComponent.h"
#include "RegionComponent.h"
#include "DestructableComponent.h"

using namespace DirectX::SimpleMath;

SystemProjectile::SystemProjectile(EntityManager& manager) :
    System(manager, false)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(ProjectileComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(DestructableComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemProjectile::UpdateEntity(float dt, float totalTime, Entity entity)
{
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	DestructableComponent& destructable = m_manager.GetComponentStore<DestructableComponent>().Get(entity);

    const RegionComponent& region = m_manager.GetComponentStore<RegionComponent>().GetComponents().begin()->second;

    if (translation.position.x <= region.min.x || translation.position.x >= region.max.x ||
        translation.position.y <= region.min.y || translation.position.y >= region.max.y)
    {
		destructable.destroy = true;
        return;
    }
}