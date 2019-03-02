#include "stdafx.h"

#include "ParticleEmitterSystem.h"
#include "ParticleEmitterComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "ParticleComponent.h"
#include "ColliderComponent.h"

using namespace DirectX::SimpleMath;

SystemParticleEmitter::SystemParticleEmitter(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(ParticleEmmiterComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(RenderComponent::Type);
	
	SetRequiredComponents(std::move(requiredComponents));
}

void SystemParticleEmitter::UpdateEntity(float dt, float totalTime, Entity entity)
{
	ParticleEmmiterComponent& particleEmitter = m_manager.GetComponentStore<ParticleEmmiterComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
	
	if (((int)(totalTime * 1000) / 250) % 2 == 0)
	{
		Vector2 sprayVelocity = MathHelper::Random(12.0f, 15.0f) * Vector2(static_cast<float>(std::cosf(particleEmitter.sprayAngle)), static_cast<float>(std::sinf(particleEmitter.sprayAngle)));
		Vector4 colour = ColourUtility::HsvToColour(5.0f, 0.5f, 0.8f);

		Vector2 velocity = MathHelper::NextVector2(3.0f, 7.0f);

		Vector2 position = translation.position + 2.0f * Vector2(sprayVelocity.y, -sprayVelocity.x) + velocity;

		Entity particle = m_manager.CreateEntity();
		m_manager.AddComponent(particle, TranslationComponent(position, velocity, 0.0f));
		m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, particleEmitter.particleTexture, render.spriteFont, 1, 1, colour));
		m_manager.AddComponent(particle, ParticleComponent(190.0f));
		m_manager.AddComponent(particle, ColliderComponent(2.0f, 2.0f));
		m_manager.RegisterEntity(particle);
	}

	particleEmitter.sprayAngle -= 3.142f * 2.0f / 50.0f;
}