#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemAttraction : public System
{
public:
	SystemAttraction(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};