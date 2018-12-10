#include "stdafx.h"

#include "DebugRenderSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "AvoidanceComponent.h"
#include "SeparationComponent.h"

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
            
    // Draw velocity and accerlation
    Vector2 normalisedVelocity = translation.velocity;
    normalisedVelocity.Normalize();

    Vector2 normalisedAcceleration = translation.acceleration;
    normalisedAcceleration.Normalize();

    Vector2 scaledVelocity = normalisedVelocity * 100.0f;
	Vector2 scaledAcceleration = normalisedAcceleration * 100.0f;
    
	DrawLine(render.spriteBatch, translation.position, translation.position + scaledVelocity, DirectX::Colors::Red, 1);
    DrawLine(render.spriteBatch, translation.position, translation.position + scaledAcceleration, DirectX::Colors::Orange, 1);

    ComponentStore<AvoidanceComponent>& avoidanceComponents = m_manager.GetComponentStore<AvoidanceComponent>();
    if (avoidanceComponents.Has(entity))
    {
        AvoidanceComponent& avoidance = avoidanceComponents.Get(entity);

		for (int i = 0; i < 3; i++)
		{
			DrawLine(render.spriteBatch, translation.position, translation.position + avoidance.ahead[i], DirectX::Colors::Purple, 1);
		}

		for (size_t i = 0; i < avoidance.debugProjectVector.size(); ++i)
		{
			DrawLine(render.spriteBatch, translation.position, translation.position + avoidance.debugProjectVector[i], DirectX::Colors::DarkGreen, 1);
		}

		for (size_t i = 0; i < avoidance.debugProjectedVector.size(); ++i)
		{
			DrawLine(render.spriteBatch, translation.position, translation.position + avoidance.debugProjectedVector[i], DirectX::Colors::Blue, 1);
		}
    }

	ComponentStore<SeparationComponent>& separationComponents = m_manager.GetComponentStore<SeparationComponent>();
	if (separationComponents.Has(entity))
	{
		SeparationComponent& separation = separationComponents.Get(entity);

		for (size_t i = 0; i < separation.debugProjectVector.size(); ++i)
		{
			DrawLine(render.spriteBatch, translation.position, translation.position + separation.debugProjectVector[i], DirectX::Colors::DarkGreen, 1);
		}
	}
	
    // Draw colliders
    DrawCircle(render.spriteBatch, translation.position, collider.radius, DirectX::Colors::LightGreen, 1);
	DrawCircle(render.spriteBatch, translation.position, collider.avoidanceRadius, DirectX::Colors::LightPink, 1);

	if (render.spriteFont)
	{
		wchar_t entityNumber[50];
		swprintf_s(entityNumber, L"%d\n", entity);
		render.spriteFont->DrawString(&render.spriteBatch, entityNumber, translation.position + Vector2(0.0f, 15.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 1.0f);
	}
}