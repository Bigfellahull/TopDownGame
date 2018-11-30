#pragma once

#include "Component.h"

struct SeparationComponent : public Component
{
	static const ComponentType Type = 10;

	SeparationComponent() { }
};
