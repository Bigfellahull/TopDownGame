#pragma once

#include "Component.h"

struct HealthComponent : public Component
{
	static const ComponentType Type = 16;

	float hitPoints;

	HealthComponent(float aHitPoints) :
		hitPoints(aHitPoints) { }
};
