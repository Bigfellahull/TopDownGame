#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct SeparationComponent : public Component
{
	static const ComponentType Type = 10;

	std::vector<DirectX::SimpleMath::Vector2> debugProjectVector;

	SeparationComponent() :
		debugProjectVector() { }
};
