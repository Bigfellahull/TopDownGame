#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct FollowPlayerComponent : public Component
{
	static const ComponentType Type = 2;

	PlayerStatus* playerStatus;
	float movementSpeed;
	float drag;

	FollowPlayerComponent(PlayerStatus* aPlayerStatus, float aMovementSpeed, float aDrag) :
        playerStatus(aPlayerStatus),
		movementSpeed(aMovementSpeed),
		drag(aDrag) { }
};
