#include "stdafx.h"

#include "RenderSystem.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

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
        Vector2(width / 2, height / 2),
        render.scale,
        SpriteEffects::SpriteEffects_None,
        0);
}