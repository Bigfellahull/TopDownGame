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

void SystemDebugRender::DrawLine(DirectX::SpriteBatch& spriteBatch, DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTORF32 colour, int lineWidth)
{
    float length = Vector2::Distance(p1, p2);
    float angle = static_cast<float>(std::atan2(p2.y - p1.y, p2.x - p1.x));

    spriteBatch.Draw(m_debugTexture->GetSrv(),
        p1,
        nullptr,
        colour,
        angle,
        Vector2::Zero,
        Vector2(length, static_cast<float>(lineWidth)),
        DirectX::SpriteEffects_None,
        0.0f);
}

void SystemDebugRender::DrawPolygon(DirectX::SpriteBatch& spriteBatch, std::vector<DirectX::SimpleMath::Vector2> verts, int count, DirectX::XMVECTORF32 colour, int lineWidth)
{
    if (count > 0)
    {
        for (int i = 0; i < count - i; i++)
        {
            DrawLine(spriteBatch, verts[i], verts[i + 1], colour, lineWidth);
        }

        DrawLine(spriteBatch, verts[count - 1], verts[0], colour, lineWidth);
    }
}

void SystemDebugRender::DrawCircle(DirectX::SpriteBatch& spriteBatch, DirectX::SimpleMath::Vector2 centre, float radius, DirectX::XMVECTORF32 colour, int lineWidth, int segments)
{
    std::vector<Vector2> verts = std::vector<Vector2>(segments);

    float increment = 3.142f * 4.0f / segments;
    float theta = 0.0f;

    for (int i = 0; i < segments; i++)
    {
        verts[i] = centre + radius * Vector2(static_cast<float>(std::cosf(theta)), static_cast<float>(std::sinf(theta)));
        theta += increment;
    }

    DrawPolygon(spriteBatch, verts, segments, colour, lineWidth);
}

void SystemDebugRender::RenderEntity(Entity entity)
{
    RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
    TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
    ColliderComponent& collider = m_manager.GetComponentStore<ColliderComponent>().Get(entity);

    Vector2 normalisedVelocity = translation.velocity;
    normalisedVelocity.Normalize();

    Vector2 ahead = translation.position + (normalisedVelocity * 150.0f);
    Vector2 ahead2 = translation.position + (normalisedVelocity * (150.0f * 0.5f));
    
    // Draw ahead vectors
    DrawLine(render.spriteBatch, translation.position, ahead, DirectX::Colors::Red, 1);
    DrawCircle(render.spriteBatch, ahead, 4.0f, DirectX::Colors::Red, 2);
    DrawCircle(render.spriteBatch, ahead2, 4.0f, DirectX::Colors::Red, 2);

    // Draw collider
    DrawCircle(render.spriteBatch, translation.position, collider.radius, DirectX::Colors::LightGreen, 2);
}