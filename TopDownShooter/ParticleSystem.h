#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemParticle : public System
{
public:
	SystemParticle(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};