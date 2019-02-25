#pragma once

#include "Component.h"

struct ParticleComponent : public Component
{
	static const ComponentType Type = 18;

	float duration;
	float percentLife;

	ParticleComponent(float aDuration) :
		duration(aDuration),
		percentLife(1.0f) { }
};
