#include "stdafx.h"

#include "RenderSystem.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"

SystemRender::SystemRender(EntityManager& manager) :
    System(manager, true)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(RenderComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));
}

void SystemRender::RenderEntity(Entity entity)
{
    RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);

    float width = render.texture->GetWidth();
    float height = render.texture->GetHeight();

    render.spriteBatch.Draw(
        render.texture->GetSrv(),
        translation.position,
        nullptr,
        render.colour,
        translation.orientation,
        DirectX::SimpleMath::Vector2(width / 2, height / 2),
        render.scale,
        DirectX::SpriteEffects::SpriteEffects_None,
        0);
}