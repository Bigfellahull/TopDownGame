#include "stdafx.h"

#include "DestructableSystem.h"
#include "DestructableComponent.h"
#include "RenderComponent.h"
#include "TranslationComponent.h"
#include "ParticleComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SystemDestructable::SystemDestructable(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(DestructableComponent::Type);
	requiredComponents.insert(RenderComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemDestructable::UpdateEntity(float dt, float totalTime, Entity entity)
{
	DestructableComponent& destructable = m_manager.GetComponentStore<DestructableComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);
	
	if (destructable.destroy)
	{
		m_manager.QueueEntityForDrop(entity);

		if (destructable.camera)
		{
			destructable.camera->Trauma(1.5f);
		}
		
		Vector4 colour;
		
		if (destructable.randomColour)
		{
			float hue1 = MathHelper::Random(0.0f, 6.0f);
			float hue2 = std::fmod(hue1 + MathHelper::Random(0.0f, 2.0f), 6.0f);

			Vector4 colour1 = ColourUtility::HsvToColour(hue1, 0.8f, 1.0f);
			Vector4 colour2 = ColourUtility::HsvToColour(hue2, 0.8f, 1.0f);

			colour = Vector4::Lerp(colour1, colour2, MathHelper::Random(0.0f, 1.0f));
		}
		else
		{
			colour = destructable.colour;
		}

		for (int i = 0; i < destructable.numberOfParticles; i++)
		{
			float speed = destructable.particleSpeed * (1.0f - 1 / MathHelper::Random(1.0f, 10.0f));
			
			Vector2 velocity = MathHelper::NextVector2(speed, speed);
						
			Entity particle = m_manager.CreateEntity();
			m_manager.AddComponent(particle, TranslationComponent(translation.position + Vector2(10, 10), velocity, 1.0f));
			m_manager.AddComponent(particle, RenderComponent(render.spriteBatch, destructable.particleTexture, render.spriteFont, 1, 1, colour));
			m_manager.AddComponent(particle, ParticleComponent(25.0f));
			m_manager.RegisterEntity(particle);
		}
	}
}
