#pragma once

#include "Component.h"
#include "SimpleMath.h"
#include "AssetManager.h"
#include "ParticleManager.h"

struct ProjectileSourceComponent : public Component
{
	static const ComponentType Type = 4;

	AssetManager* assetManager;
	DirectX::SimpleMath::Vector2 aimDirection;
	float cooldownTime;
	float cooldownRemaining;
	ParticleManager* particleManager;

	ProjectileSourceComponent(AssetManager* aAssetManager, ParticleManager* aParticleManager) :
		assetManager(aAssetManager),
		aimDirection(DirectX::SimpleMath::Vector2(0, 0)),
		cooldownTime(0.15f),
		cooldownRemaining(0),
		particleManager(aParticleManager) { }
};
