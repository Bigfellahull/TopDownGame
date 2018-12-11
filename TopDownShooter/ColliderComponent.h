#pragma once

#include "Component.h"

struct ColliderComponent : public Component
{
	static const ComponentType Type = 1;

	float radius;
	float avoidanceRadius;

	float hasCollided;
	Entity collidedWith;

	ColliderComponent(float aRadius, float aAvoidanceRadius) :
		radius(aRadius),
		avoidanceRadius(aAvoidanceRadius),
		hasCollided(false),
		collidedWith(InvalidEntity) { }
};
