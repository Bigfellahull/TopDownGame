#pragma once

#include "CircularArray.h"
#include "Texture2d.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"

class ParticleManager
{
public:
	struct Particle
	{
		Texture2d* texture;
		DirectX::SimpleMath::Vector2 position;
		DirectX::SimpleMath::Vector2 velocity;
		DirectX::SimpleMath::Vector2 scale;
		DirectX::SimpleMath::Vector4 colour;
		float orientation;
		float duration;
		float percentLife;

		Particle() :
			texture(),
			position(DirectX::SimpleMath::Vector2(0, 0)),
			velocity(DirectX::SimpleMath::Vector2(0, 0)),
			colour(DirectX::SimpleMath::Vector4(1, 1, 1, 1)),
			scale(DirectX::SimpleMath::Vector2(1, 1)),
			orientation(0.0f),
			duration(100.0f),
			percentLife(1.0f) { }
	};

	ParticleManager(DirectX::SimpleMath::Rectangle worldBounds);
	
	void CreateParticle(
		Texture2d* texture, 
		DirectX::SimpleMath::Vector2 position, 
		DirectX::SimpleMath::Vector2 velocity, 
		DirectX::SimpleMath::Vector4 colour,
		float duration, 
		DirectX::SimpleMath::Vector2 scale,
		float theta = 0);
	void Update(float dt);
	void Draw(DirectX::SpriteBatch& spriteBatch);
	int GetNumberOfParticles();
private:
	DirectX::SimpleMath::Rectangle m_worldBounds;
	CircularArray<Particle> m_particles;
};