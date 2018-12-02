#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct WanderComponent : public Component
{
	static const ComponentType Type = 13;

	PlayerStatus* playerStatus;
	float movementSpeed;
	float drag;
	float wanderAngle;

	WanderComponent(PlayerStatus* aPlayerStatus, float aMovementSpeed, float aDrag) :
		playerStatus(aPlayerStatus),
		movementSpeed(aMovementSpeed),
		drag(aDrag),
		wanderAngle(0.0f) { }
};
