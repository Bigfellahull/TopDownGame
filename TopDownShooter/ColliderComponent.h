#pragma once

#include "Component.h"

struct ColliderComponent : public Component
{
	static const ComponentType Type;

	float radius;

	ColliderComponent(float aRadius) :
		radius(aRadius) { }
};

const ComponentType ColliderComponent::Type = 7;
