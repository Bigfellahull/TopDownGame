#include "stdafx.h"

#include "ParticleSystem.h"
#include "ParticleComponent.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "RegionComponent.h"

using namespace DirectX::SimpleMath;

SystemParticle::SystemParticle(EntityManager& manager) :
	System(manager, false)
{
	std::set<ComponentType> requiredComponents;
	requiredComponents.insert(ParticleComponent::Type);
	requiredComponents.insert(TranslationComponent::Type);
	requiredComponents.insert(RenderComponent::Type);

	SetRequiredComponents(std::move(requiredComponents));
}

void SystemParticle::UpdateEntity(float dt, float totalTime, Entity entity)
{
	ParticleComponent& particle = m_manager.GetComponentStore<ParticleComponent>().Get(entity);
	TranslationComponent& translation = m_manager.GetComponentStore<TranslationComponent>().Get(entity);
	RenderComponent& render = m_manager.GetComponentStore<RenderComponent>().Get(entity);

	translation.acceleration = Vector2::Zero;

	float speed = translation.velocity.Length();
	float alpha = std::min(1.0f, std::min(particle.percentLife * 2, speed * 1.0f));
	alpha *= alpha;

	render.colour.w = alpha;
	render.scale.x = 1.0f * std::min(std::min(1.0f, 0.2f * speed + 0.1f), alpha);

	// TODO: Bounce particles.
	/*if (particle.position.x < m_worldBounds.x)
	{
		particle.velocity.x = std::abs(particle.velocity.x);
	}
	else if (particle.position.x > m_worldBounds.width)
	{
		particle.velocity.x = -std::abs(particle.velocity.x);
	}

	if (particle.position.y < m_worldBounds.y)
	{
		particle.velocity.y = std::abs(particle.velocity.y);
	}
	else if (particle.position.y > m_worldBounds.height)
	{
		particle.velocity.y = -std::abs(particle.velocity.y);
	}*/

	// TODO: Take into account dt (slow mode).
	particle.percentLife -= 1.0f / particle.duration;

	if (particle.percentLife < 0)
	{
		m_manager.QueueEntityForDrop(entity);
	}
}