#pragma once

#include "Component.h"
#include "SimpleMath.h"

struct ProjectileSourceComponent : public Component
{
	static const ComponentType Type;

	AssetManager* assetManager;
	DirectX::SimpleMath::Vector2 aimDirection;
	float cooldownTime;
	float cooldownRemaining;

	ProjectileSourceComponent(AssetManager* aAssetManager) :
		assetManager(aAssetManager),
		aimDirection(DirectX::SimpleMath::Vector2(0, 0)),
		cooldownTime(0.15f),
		cooldownRemaining(0) { }
};

const ComponentType ProjectileSourceComponent::Type = 3;
