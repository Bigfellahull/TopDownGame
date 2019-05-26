#include "stdafx.h"

#include "ExhaustPlumeSystem.h"
#include "ExhaustPlumeComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "ParticleComponent.h"
#include "ColliderComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemExhaustPlume::SystemExhaustPlume(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(ExhaustPlumeComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(RenderComponent::Type);
	
	SetRequiredComponents(std::move(requiredComponents));
}

void SystemExhaustPlume::UpdateEntity(float dt, float totalTime, Entity entity)
{
	ExhaustPlumeComponent& exhaustPlume = m_manager.GetComponentStore<ExhaustPlumeComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

	if (translation.velocity.LengthSquared() < 1.0f)
	{
		return;
	}

	float orientation = std::atan2(translation.velocity.y, translation.velocity.x);
	Quaternion rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, orientation);

	Vector2 baseVelocity = translation.velocity * (-150.0f / translation.velocity.Length());
	Vector2 perpendicularVelocity = Vector2(baseVelocity.y, -baseVelocity.x) * (0.5f * std::sinf(totalTime * 10.0f));

	Vector4 colour1 = Vector4(0.78f, 0.15f, 0.04f, 1.0f);
	Vector4 colour2 = Vector4(1.0f, 0.73f, 0.12f, 1.0f);
	Vector2 position = translation.position + Vector2::Transform(Vector2(-34.0f, 0.0f), rotation);

	float alpha = 0.7f;

	Vector2 velocityMid = baseVelocity + MathHelper::NextVector2(0.0f, 1.0f);

	// TODO: Make time based. In slow mode this creates too many particles!
	if (!DisableParticles)
	{
		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocityMid, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleTexture, render.spriteFont, 1, 1, Vector4::One * alpha));
			m_manager.AddComponent(particle, ParticleComponent(10.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}
		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocityMid, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleGlowTexture, render.spriteFont, 1, 1, colour2 * alpha));
			m_manager.AddComponent(particle, ParticleComponent(10.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}

		Vector2 velocitySide1 = baseVelocity + perpendicularVelocity + MathHelper::NextVector2(0.0f, 0.3f);
		Vector2 velocitySide2 = baseVelocity - perpendicularVelocity + MathHelper::NextVector2(0.0f, 0.3f);

		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocitySide1, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleTexture, render.spriteFont, 1, 1, Vector4::One * alpha));
			m_manager.AddComponent(particle, ParticleComponent(15.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}
		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocitySide1, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleGlowTexture, render.spriteFont, 1, 1, colour1 * alpha));
			m_manager.AddComponent(particle, ParticleComponent(15.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}
		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocitySide2, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleTexture, render.spriteFont, 1, 1, Vector4::One * alpha));
			m_manager.AddComponent(particle, ParticleComponent(15.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}
		{
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(position, velocitySide2, orientation));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, exhaustPlume.particleGlowTexture, render.spriteFont, 1, 1, colour1 * alpha));
			m_manager.AddComponent(particle, ParticleComponent(15.0f));
			m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
			m_manager.RegisterEntity(particle);
		}
	}
}