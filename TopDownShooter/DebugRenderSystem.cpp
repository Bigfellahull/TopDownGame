#include "stdafx.h"

#include "DebugRenderSystem.h"
#include "ColliderComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "AvoidanceComponent.h"
#include "SeparationComponent.h"
#include "HealthComponent.h"

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
#if _DEBUG
	if (ShowDebugVisuals)
	{
		Vector2 normalisedVelocity = translation.velocity;
		normalisedVelocity.Normalize();

		Vector2 normalisedAcceleration = translation.acceleration;
		normalisedAcceleration.Normalize();

		Vector2 scaledVelocity = normalisedVelocity * 100.0f;
		Vector2 scaledAcceleration = normalisedAcceleration * 100.0f;

		GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + scaledVelocity, DirectX::Colors::Red, 1);
		GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + scaledAcceleration, DirectX::Colors::Orange, 1);

		ComponentStore<AvoidanceComponent>& avoidanceComponents = m_manager.GetComponentStore<AvoidanceComponent>();
		if (avoidanceComponents.Has(entity))
		{
			AvoidanceComponent& avoidance = avoidanceComponents.Get(entity);

			for (int i = 0; i < 3; i++)
			{
				GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + avoidance.ahead[i], DirectX::Colors::Purple, 1);
			}

			for (size_t i = 0; i < avoidance.debugProjectVector.size(); ++i)
			{
				GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + avoidance.debugProjectVector[i], DirectX::Colors::DarkGreen, 1);
			}

			for (size_t i = 0; i < avoidance.debugProjectedVector.size(); ++i)
			{
				GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + avoidance.debugProjectedVector[i], DirectX::Colors::Blue, 1);
			}
		}

		ComponentStore<SeparationComponent>& separationComponents = m_manager.GetComponentStore<SeparationComponent>();
		if (separationComponents.Has(entity))
		{
			SeparationComponent& separation = separationComponents.Get(entity);

			for (size_t i = 0; i < separation.debugProjectVector.size(); ++i)
			{
				GraphicsHelper::DrawLine(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, translation.position + separation.debugProjectVector[i], DirectX::Colors::DarkGreen, 1);
			}
		}

		// Draw colliders
		GraphicsHelper::DrawCircle(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, collider.radius, DirectX::Colors::LightGreen, 1);
		GraphicsHelper::DrawCircle(render.spriteBatch, m_debugTexture->GetSrv(), translation.position, collider.avoidanceRadius, DirectX::Colors::LightPink, 1);
	}

	if (render.spriteFont && ShowDebugText)
	{
		wchar_t entityNumber[50];
		swprintf_s(entityNumber, L"Entity - %d\n", entity);
		render.spriteFont->DrawString(&render.spriteBatch, entityNumber, translation.position + Vector2(0.0f, 15.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 0.6f);

		wchar_t entityPosition[50];
		swprintf_s(entityPosition, L"Position - x:%f,y:%f\n", translation.position.x, translation.position.y);
		render.spriteFont->DrawString(&render.spriteBatch, entityPosition, translation.position + Vector2(0.0f, 35.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 0.6f);

		wchar_t entityVelocity[50];
		swprintf_s(entityVelocity, L"Velocity - x:%f,y:%f\n", translation.velocity.x, translation.velocity.y);
		render.spriteFont->DrawString(&render.spriteBatch, entityVelocity, translation.position + Vector2(0.0f, 55.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 0.6f);

		wchar_t entityAcceleration[50];
		swprintf_s(entityAcceleration, L"Acceleration - x:%f,y:%f\n", translation.acceleration.x, translation.acceleration.y);
		render.spriteFont->DrawString(&render.spriteBatch, entityAcceleration, translation.position + Vector2(0.0f, 75.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 0.6f);

		ComponentStore<HealthComponent>& healthComponents = m_manager.GetComponentStore<HealthComponent>();
		if (healthComponents.Has(entity))
		{
			wchar_t entityHealth[50];
			swprintf_s(entityHealth, L"Health - %f\n", healthComponents.Get(entity).hitPoints);
			render.spriteFont->DrawString(&render.spriteBatch, entityHealth, translation.position + Vector2(0.0f, 95.0f), Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 0.6f);
		}
	}
#endif
}