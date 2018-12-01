#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct WanderComponent : public Component
{
	static const ComponentType Type = 13;

	float movementSpeed;
	float drag;
	float wanderAngle;

	WanderComponent(float aMovementSpeed, float aDrag) :
		movementSpeed(aMovementSpeed),
		drag(aDrag),
		wanderAngle(0.0f) { }
};
