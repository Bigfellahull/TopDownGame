#pragma once

#include "Component.h"

struct AvoidanceComponent : public Component
{
	static const ComponentType Type = 9;

	AvoidanceComponent() { }
};
