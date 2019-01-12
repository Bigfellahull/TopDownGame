#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemEntityContainer : public System
{
public:
	SystemEntityContainer(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};