#include "stdafx.h"

#include "DebugRenderSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemDebugRender::SystemDebugRender(EntityManager& manager, Texture2d* debugTexture) :
    System(manager, true)
{
    std::set<ComponentType> requiredComponents;
    requiredComponents.insert(ColliderComponent::Type);
    requiredComponents.insert(TranslationComponent::Type);

    SetRequiredComponents(std::move(requiredComponents));

    m_debugTexture = debugTexture;
}

void SystemDebugRender::RenderEntity(Entity entity)
{
    RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
    ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);

    float width = render.texture->GetWidth();
    float height = render.texture->GetHeight();

    Vector2 normalisedVelocity = translation.velocity;
    normalisedVelocity.Normalize();
    Vector2 ahead = translation.position + (normalisedVelocity * 150.0f);
    Vector2 ahead2 = ahead * 0.5f;

    Vector2 edge = ahead - translation.position;
    float angle = (float)std::atan2(edge.y, edge.x);
    
    // Draw ahead vector
    render.spriteBatch.Draw(
        m_debugTexture->GetSrv(),
        Rectangle(translation.position.x, translation.position.y, edge.Length(), 1.0f),
        0,
        DirectX::Colors::Red,
        angle,
        Vector2(0, 0),
        DirectX::SpriteEffects_None,
        0);

#if 0
    // Attempt to draw collision radius
    float max = 2.0f * 3.142f;
    float step = max /10.0f;

    std::vector<Vector2> vec;
    for (float theta = 0.0f; theta < max; theta += step)
    {
        vec.push_back(Vector2((float)(collider.radius * std::cosf(theta), (float)(collider.radius * std::sinf(theta)))));
    }
    vec.push_back(Vector2((float)(collider.radius * std::cosf(0), (float)(collider.radius * std::sinf(0)))));

    for (int i = 1; i < vec.size(); i++)
    {
        auto point1 = vec[i - 1] + translation.position;
        auto point2 = vec[i] + translation.position;

        float distance = Vector2::Distance(point1, point2);
        float angle = (float)std::atan2(point2.y - point1.y, point2.x - point1.x);

        render.spriteBatch.Draw(m_debugTexture->GetSrv(),
            Rectangle(point1.x, point1.y, distance, 1.0f),
            0,
            DirectX::Colors::Blue,
            angle,
            Vector2(0, 0),
            DirectX::SpriteEffects_None,
            0);
    }
#endif
}