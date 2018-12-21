#pragma once

#include "Component.h"
#include "PlayerStatus.h"

struct EnemyComponent : public Component
{
	static const ComponentType Type = 11;

	int points;
	bool alive;
	float timeToStart;
	float initialTimeToStart;

	EnemyComponent(float aTimeToStart, int aPoints = 10) :
		points(aPoints),
		alive(false),
		timeToStart(aTimeToStart),
		initialTimeToStart(aTimeToStart) { }
};
