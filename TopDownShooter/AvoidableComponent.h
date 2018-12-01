#pragma once

#include "Component.h"

struct AvoidableComponent : public Component
{
	static const ComponentType Type = 12;

	AvoidableComponent() { }
};
