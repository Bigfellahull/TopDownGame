#pragma once

#include "Component.h"
#include "Texture2d.h"

struct ParticleEmmiterComponent : public Component
{
	static const ComponentType Type = 20;

	Texture2d* particleTexture;
	float sprayAngle;

	ParticleEmmiterComponent(Texture2d* aParticleTexture, float aSprayAngle) :
		particleTexture(aParticleTexture),
		sprayAngle(aSprayAngle) { }
};