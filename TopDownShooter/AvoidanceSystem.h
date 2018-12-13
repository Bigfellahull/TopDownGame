#pragma once

#include "System.h"
#include "EntityManager.h"
#include "SimpleMath.h"

class SystemAvoidance : public System
{
public:
	SystemAvoidance(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};