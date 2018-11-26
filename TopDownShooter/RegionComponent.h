#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct RegionComponent : public Component
{
	static const ComponentType Type = 5;

	DirectX::SimpleMath::Vector2 min;
	DirectX::SimpleMath::Vector2 max;

	RegionComponent(DirectX::SimpleMath::Vector2 aMin, DirectX::SimpleMath::Vector2 aMax) :
		min(aMin),
		max(aMax) { }
};
