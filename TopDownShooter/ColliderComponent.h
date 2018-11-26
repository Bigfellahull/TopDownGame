#pragma once

#include "Component.h"

struct ColliderComponent : public Component
{
	static const ComponentType Type = 1;

	float radius;

	ColliderComponent(float aRadius) :
		radius(aRadius) { }
};
