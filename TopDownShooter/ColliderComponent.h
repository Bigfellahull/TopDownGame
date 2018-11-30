#pragma once

#include "Component.h"

struct ColliderComponent : public Component
{
	static const ComponentType Type = 1;

	float radius;
	float avoidanceRadius;

	ColliderComponent(float aRadius, float aAvoidanceRadius) :
		radius(aRadius),
		avoidanceRadius(aAvoidanceRadius) { }
};
