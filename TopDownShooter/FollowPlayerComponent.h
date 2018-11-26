#pragma once

#include "Component.h"

struct FollowPlayerComponent : public Component
{
	static const ComponentType Type = 2;

	Entity playerEntity;
	float movementSpeed;
	float drag;

	float timeToStart;

	FollowPlayerComponent(Entity aPlayerEntity, float aMovementSpeed, float aDrag) :
		playerEntity(aPlayerEntity),
		movementSpeed(aMovementSpeed),
		drag(aDrag),
		timeToStart(60) { }
};
