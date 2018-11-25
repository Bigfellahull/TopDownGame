#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct ProjectileSourceComponent : public Component
{
	static const ComponentType Type;

	DirectX::SimpleMath::Vector2 aimDirection;
	float cooldownFrames;
	float cooldownRemaining;

	ProjectileSourceComponent() :
		aimDirection(DirectX::SimpleMath::Vector2(0, 0)),
		cooldownFrames(5),
		cooldownRemaining(0) { }
};

const ComponentType ProjectileSourceComponent::Type = 3;
