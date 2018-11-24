#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct ProjectileComponent : public Component
{
	static const ComponentType Type;

	DirectX::SimpleMath::Vector2 aimDirection;
	float cooldownFrames;
	float cooldownRemaining;

	ProjectileComponent(DirectX::SimpleMath::Vector2 aAimDirection) :
		aimDirection(aAimDirection),
		cooldownFrames(5),
		cooldownRemaining(0) { }
};

const ComponentType ProjectileComponent::Type = 3;
