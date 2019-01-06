#include "stdafx.h"

#include "ParticleManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

ParticleManager::ParticleManager(Rectangle worldBounds) :
	m_worldBounds(worldBounds),
	m_particles(2000)
{

}

void ParticleManager::CreateParticle(
	Texture2d* texture,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 velocity,
	DirectX::SimpleMath::Vector4 colour,
	float duration,
	DirectX::SimpleMath::Vector2 scale,
	float theta)
{
	int index;

	int particleCapacity = m_particles.GetCapacity();

	if (m_particles.count == particleCapacity)
	{
		index = 0;
		m_particles.start = (m_particles.start + 1) % particleCapacity;
	}
	else
	{
		index = m_particles.count++;
	}

	Particle& particle = m_particles[index];

	particle.texture = texture;
	particle.position = position;
	particle.velocity = velocity;
	particle.scale = scale;
	particle.colour = colour;
	particle.orientation = theta;
	particle.duration = duration;
	particle.percentLife = 1.0f;
}

void ParticleManager::Update(float dt)
{
	int removalCount = 0;

	for (int i = 0; i < m_particles.count; i++)
	{
		Particle& particle = m_particles[i];

		particle.position += particle.velocity * dt;
		particle.orientation = static_cast<float>(std::atan2(particle.velocity.y, particle.velocity.x));

		float speed = particle.velocity.Length();
		float alpha = std::min(1.0f, std::min(particle.percentLife * 2, speed * 1.0f));
		alpha *= alpha;

		particle.colour.w = alpha;
		particle.scale.x = 1.0f * std::min(std::min(1.0f, 0.2f * speed + 0.1f), alpha);

		if (particle.position.x < m_worldBounds.x)
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
		}

		if (std::abs(particle.velocity.x) + std::abs(particle.velocity.y) < 0.00000000001f)
		{
			particle.velocity = Vector2::Zero;
		}

		particle.velocity *= 0.97f;
		
		particle.percentLife -= 1.0f / particle.duration;

		std::swap(m_particles[i - removalCount], m_particles[i]);

		if (particle.percentLife < 0)
		{
			removalCount++;
		}
	}

	m_particles.count -= removalCount;
}

void ParticleManager::Draw(SpriteBatch& spriteBatch)
{
	for (int i = 0; i < m_particles.count; i++)
	{
		Particle& p = m_particles[i];

		Vector2 origin = Vector2(
			static_cast<float>(p.texture->GetWidth() / 2),
			static_cast<float>(p.texture->GetHeight() / 2));

		spriteBatch.Draw(
			p.texture->GetSrv(), 
			p.position, 
			nullptr, 
			p.colour, 
			p.orientation, 
			origin, 
			p.scale, 
			SpriteEffects::SpriteEffects_None, 
			0.0f);
	}
}

int ParticleManager::GetNumberOfParticles()
{
	return m_particles.count;
}

void ParticleManager::ClearParticles()
{
	m_particles.count = 0;
}
