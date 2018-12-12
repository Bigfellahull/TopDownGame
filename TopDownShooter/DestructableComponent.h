#pragma once

#include "Component.h"
#include "ParticleManager.h"

struct DestructableComponent : public Component
{
	static const ComponentType Type = 14;

	ParticleManager* particleManger;
	Texture2d* particleTexture;
	float particleSpeed;
	bool destroy;
	
	DestructableComponent(ParticleManager* aParticleManger, Texture2d* aParticleTexture, float aParticleSpeed = 11.0f) :
		particleManger(aParticleManger),
		particleTexture(aParticleTexture),
		particleSpeed(aParticleSpeed),
		destroy(false) { }
};
