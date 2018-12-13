#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemSeparation : public System
{
public:
	SystemSeparation(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};