#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemWander : public System
{
public:
	SystemWander(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};