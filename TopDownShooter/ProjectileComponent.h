#pragma once

#include "Component.h"

struct ProjectileComponent : public Component
{
	static const ComponentType Type = 3;

	ProjectileComponent() { }
};
