#pragma once

#include "Component.h"
#include "ParticleManager.h"

struct ExhaustPlumeComponent : public Component
{
	static const ComponentType Type = 15;

	ParticleManager* particleManger;
	Texture2d* particleTexture;
	Texture2d* particleGlowTexture;

	ExhaustPlumeComponent(ParticleManager* aParticleManger, Texture2d* aParticleTexture, Texture2d* aParticleGlowTexture) :
		particleManger(aParticleManger),
		particleTexture(aParticleTexture),
		particleGlowTexture(aParticleGlowTexture) { }
};
