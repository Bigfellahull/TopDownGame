#pragma once

#include "Component.h"
#include "ParticleManager.h"
#include "Camera.h"

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

	// TODO: Refactor this to somewhere more sensible
	Camera* camera;
	
	DestructableComponent(
		ParticleManager* aParticleManger, 
		Texture2d* aParticleTexture,
		Camera* aCamera,
		float aParticleSpeed = 500.0f, 
		int aNumberOfParticles = 200, 
		bool aRandomColour = true, 
		DirectX::SimpleMath::Vector4 aColour = DirectX::SimpleMath::Vector4::Zero) :
		particleManger(aParticleManger),
		particleTexture(aParticleTexture),
		particleSpeed(aParticleSpeed),
		numberOfParticles(aNumberOfParticles),
		destroy(false),
		randomColour(aRandomColour),
		colour(aColour),
		camera(aCamera) { }
};
