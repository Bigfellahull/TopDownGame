#pragma once

#include "Component.h"

struct FollowPlayerComponent : public Component
{
	static const ComponentType Type;

	Entity playerEntity;
	float movementSpeed;
	float drag;

	FollowPlayerComponent(Entity aPlayerEntity, float aMovementSpeed, float aDrag) :
		playerEntity(aPlayerEntity),
		movementSpeed(aMovementSpeed),
		drag(aDrag) { }
};

const ComponentType FollowPlayerComponent::Type = 6;
