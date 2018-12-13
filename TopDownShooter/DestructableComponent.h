#pragma once

#include "Component.h"
#include "ParticleManager.h"

struct DestructableComponent : public Component
{
	static const ComponentType Type = 14;

	ParticleManager* particleManger;
	Texture2d* particleTexture;
	float particleSpeed;
	int numberOfParticles;
	bool destroy;
	bool randomColour;
	DirectX::SimpleMath::Vector4 colour;
	
	DestructableComponent(
		ParticleManager* aParticleManger, 
		Texture2d* aParticleTexture, 
		float aParticleSpeed = 11.0f, 
		int aNumberOfParticles = 200, 
		bool aRandomColour = true, 
		DirectX::SimpleMath::Vector4 aColour = DirectX::SimpleMath::Vector4::Zero) :
		particleManger(aParticleManger),
		particleTexture(aParticleTexture),
		particleSpeed(aParticleSpeed),
		numberOfParticles(aNumberOfParticles),
		destroy(false),
		randomColour(aRandomColour),
		colour(aColour) { }
};
