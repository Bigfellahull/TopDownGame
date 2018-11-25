#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct ProjectileComponent : public Component
{
	static const ComponentType Type;

	ProjectileComponent() { }
};

const ComponentType ProjectileComponent::Type = 5;
