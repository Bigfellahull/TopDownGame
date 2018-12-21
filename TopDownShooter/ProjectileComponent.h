#pragma once

#include "Component.h"

struct ProjectileComponent : public Component
{
	static const ComponentType Type = 3;

	float damage;
	Entity firedByEntity;

	ProjectileComponent(Entity aFiredByEntity, float aDamage = 10.0f) :
		damage(aDamage),
		firedByEntity(aFiredByEntity) { }
};
