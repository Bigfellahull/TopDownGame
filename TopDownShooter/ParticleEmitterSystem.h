#pragma once

#include "System.h"
#include "EntityManager.h"

class SystemParticleEmitter : public System
{
public:
	SystemParticleEmitter(EntityManager& manager);

	virtual void UpdateEntity(float dt, float totalTime, Entity entity);
};