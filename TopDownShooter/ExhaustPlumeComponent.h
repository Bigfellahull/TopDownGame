#pragma once

#include "Component.h"
#include "Texture2d.h"

struct ExhaustPlumeComponent : public Component
{
	static const ComponentType Type = 15;

	Texture2d* particleTexture;
	Texture2d* particleGlowTexture;

	ExhaustPlumeComponent(Texture2d* aParticleTexture, Texture2d* aParticleGlowTexture) :
		particleTexture(aParticleTexture),
		particleGlowTexture(aParticleGlowTexture) { }
};
