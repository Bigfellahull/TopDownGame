#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct EnemyComponent : public Component
{
	static const ComponentType Type = 11;

	bool alive;
	float timeToStart;
	float initialTimeToStart;

	EnemyComponent(float aTimeToStart) :
		alive(false),
		timeToStart(aTimeToStart),
		initialTimeToStart(aTimeToStart) { }
};
