#pragma once

#include "Component.h"
#include "SimpleMath.h"
#include "AssetManager.h"

struct ProjectileSourceComponent : public Component
{
	static const ComponentType Type = 4;

	AssetManager* assetManager;
	DirectX::SimpleMath::Vector2 aimDirection;
	float cooldownTime;
	float cooldownRemaining;
	
	ProjectileSourceComponent(AssetManager* aAssetManager) :
		assetManager(aAssetManager),
		aimDirection(DirectX::SimpleMath::Vector2(0, 0)),
		cooldownTime(0.10f),
		cooldownRemaining(0) { }
};
